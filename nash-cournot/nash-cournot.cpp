#include <eris/Simulation.hpp>
#include <eris/Good.hpp>
#include <eris/Random.hpp>
#include <eris/intraopt/Callback.hpp>

#include "nc/Firm.hpp"

#include <iostream>
#include <cstdio>
#include <stdexcept>

using eris::Simulation;
using eris::Random;

// Function used by the 

// Parse a value of the templated type.  Throws an exception if the given string is not entirely
// parseable as the requested type.
template<typename T> T parse(const std::string &str) {
    size_t endpos;
    T result;
    if (typeid(T) == typeid(double)) result = std::stod(str, &endpos);
    else if (typeid(T) == typeid(float)) result = std::stof(str, &endpos);
    else if (typeid(T) == typeid(long)) result = std::stol(str, &endpos);
    else if (typeid(T) == typeid(unsigned long)) result = std::stoul(str, &endpos);
    else throw std::invalid_argument(std::string("Internal error: don't know how to parse a ") + typeid(T).name());
    if (endpos != str.length()) throw std::invalid_argument("`" + str + "' is not a valid value (trailing garbage found)");
    return result;
}

int main(int argc, char **argv) {
    double demand_intercept = 50.0,
           demand_slope = -2.0,
           cost = 1.0,
           epsilon = 0.01,
           initialQ = std::numeric_limits<double>::quiet_NaN();
    unsigned long num_firms = 2, firms_selected = 1, periods = 1000;

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
#define IFARG(SWITCH, TYPE, VAR, FAIL_CONDITION) \
        if (arg == "-" #SWITCH) {\
            if (++i >= argc) throw std::invalid_argument("-" #SWITCH " requires an argument"); \
            TYPE SWITCH = parse<TYPE>(argv[i]); \
            if (FAIL_CONDITION) throw std::invalid_argument(std::to_string(SWITCH) + " is not a valid value for '-" #SWITCH "a' argument"); \
            VAR = SWITCH; \
        }
        IFARG(a, double, demand_intercept, not std::isfinite(a) or a <= 0)
        else IFARG(b, double, demand_slope, not std::isfinite(b) or b == 0)
        else IFARG(c, double, cost, not std::isfinite(c) or c < 0)
        else IFARG(Q, double, initialQ, not std::isfinite(Q) or Q < 0)
        else IFARG(e, double, epsilon, not std::isfinite(e) or e <= 0)
        else IFARG(n, unsigned long, num_firms, n == 0)
        else IFARG(s, unsigned long, firms_selected, s == 0)
        else IFARG(t, unsigned long, periods, t == 0)
        else if (arg == "--help" or arg == "-h") {
            std::cout << "Nash-Cournot simulator.\n\nOptions supported:\n" <<
                "-a VALUE   -- specifies the P(Q) demand intercept (defaults to 50)\n" <<
                "-b VALUE   -- specifies the P(Q) demand slope (defaults to -2).  Positive values\n" <<
                "              will be negated (i.e. 2 and -2 are the same)\n" <<
                "-c VALUE   -- specified the firms' marginal cost (defaults to 1)\n" <<
                "-Q VALUE   -- specifies the initial aggregate quantity.  The default is to draw\n" <<
                "              this from U[0, z] where z is the quantity were P = marginal cost.\n" <<
                "-e VALUE   -- the default adjustment amount, epsilon (defaults to 0.01)\n" <<
                "-n INTEGER -- specifies the number of firms (defaults to 2)\n" <<
                "-s INTEGER -- specifies the number of firms allowed to optimize each period\n" <<
                "              (defaults to 1)\n" <<
                "-t INTEGER -- the number of periods to run the model (defaults to 1000)\n" <<
                "\n\n";
            std::exit(0);
        }
        else {
            throw std::invalid_argument("Unknown argument: " + arg);
        }
    }

    auto P = [&](double Q) -> double {
        if (Q < 0) throw std::domain_error("Cannot calculate demand price for a negative quantity");
        double p = demand_intercept + demand_slope * Q;
        if (p < 0) p = 0;
        return p;
    };

    // If given a positive "b", make it negative
    if (demand_slope > 0) demand_slope = -demand_slope;


    const double Q_monopoly = cost >= demand_intercept ? 0 : (demand_intercept - cost) / (2 * -demand_slope);
    const double P_monopoly = P(Q_monopoly);
    const double Q_cournot = cost >= demand_intercept ? 0 : num_firms * (demand_intercept - cost) / (-demand_slope * (num_firms+1));
    const double P_cournot = P(Q_cournot);

    auto &rng = Random::rng();
    // Initial conditions: draw aggregate quantity from U[0, M] where M is the quantity where price
    // drops to marginal cost, i.e. (a - c) / b

    if (std::isnan(initialQ)) {
        if (cost >= demand_intercept) {
            throw std::runtime_error("Unable to run: cost exceeds maximum price (if this is intentional, specify an initial aggregate quantity with -Q)");
        }
        initialQ = std::uniform_real_distribution<double>(0, (demand_intercept - cost) / -demand_slope)(rng);
        std::cout << "Initial Q: " << initialQ << "\n";
    }


    std::cout << "Running model with:" <<
        "\n\tDemand:  P(Q) = " << demand_intercept << " - " << -demand_slope << " Q" <<
        "\n\tCost:    c(q) = " << cost <<
        "\n\tFirms:   N = " << num_firms <<
        "\n\nTheory predictions:\n" <<
        "\n\tMonop.:  Q = " << Q_monopoly << ", P = " << P_monopoly <<
        "\n\tCournot: Q = " << Q_cournot << ", P = " << P_cournot <<
        "\n\nSimulation parameters:\n" <<
      u8"\n\tEpsilon: ε = " << epsilon << "; " << firms_selected << " optimizing firms per period" <<
        "\n\tPeriods: t = " << periods <<
        "\n";

    auto sim = Simulation::create();

    for (unsigned int i = 0; i < num_firms; i++) {
        sim->spawn<nc::Firm>();
    }
    auto firms = sim->agents<nc::Firm>();

    // Now split up quantities according to an equal uniform distribution draw, then rescale amounts
    // so that the total adds up to initialQ
    double total = 0;
    std::uniform_real_distribution<double> unif01(0, 1);
    for (auto &f : firms) {
        f->quantity = unif01(rng);
        total += f->quantity;
    }
    // Update firm quantity and profits for the initial setup.  For quantities, we currently have a
    // bunch of U[0,1] draws: we need rescale them so that they sum up to initialQ.
    double scale = initialQ / total;
    for (auto &f : firms) f->quantity *= scale;

    double price;
    double aggQ;
    double aggProfit;

    // Create an intra-finish callback that calculates the market price and then updates quantity
    // and profit of each firm.
    auto market_update = [&]() {
        aggQ = 0;
        aggProfit = 0;
        for (auto &f : firms) aggQ += f->quantity;
        price = P(aggQ);

        // If the implied price is negative, truncate it to 0 (assume that demand is infinite at p=0)
        if (price < 0) price = 0;

        for (auto &f : firms) {
            f->profit = (price - cost) * f->quantity;
            aggProfit += f->profit;
        }
    };

    // Set up a callback to recalculate profits each period
    sim->spawn<eris::intraopt::FinishCallback>(market_update);

    while (sim->t() < periods) {

        if (sim->t() % 20 == 0)
            std::cout << u8"    t      Q        P        Π     Q_Monop  P_Monop  Q_Cour.  P_Cour.\n" <<
                         u8"   ---  -------  -------  -------  -------  -------  -------  -------" << std::endl;

        // Randomly select firms by shuffle the list of firms, then the first `firms_selected` in
        // the shuffled list get to optimize, the rest don't.
        std::shuffle(firms.begin(), firms.end(), rng);
        for (unsigned long f = 0; f < firms.size(); f++) firms[f]->optimize = f < firms_selected;

        sim->run();

        std::printf("%6d  %7.3f  %7.3f  %7.3f  %7.3f  %7.3f  %7.3f  %7.3f\n",
                sim->t(), aggQ, price, aggProfit, Q_monopoly, P_monopoly, Q_cournot, P_cournot); 
    }
}
