#include <eris/Eris.hpp>
#include <eris/Simulation.hpp>
#include <eris/Good.hpp>
#include <eris/consumer/Polynomial.hpp>

#include <iostream>

using namespace eris;
using eris::consumer::Polynomial;

int main() {
    Eris<Simulation> sim;

    auto money = sim->create<Good::Continuous>("Money");
    auto x = sim->create<Good::Continuous>("x");

    // The slopes from the Q = A + B P demand function
    constexpr double demand_const = 50.0;
    constexpr double demand_slope = -2.0;

    // Convert the slopes back into a quadratic utility function
    constexpr double u_quad = 1/(2.0*demand_slope);
    constexpr double u_lin  = demand_const * 2 * -u_quad;

    std::cout << "Demand: Q = " << demand_const << " - " << -demand_slope << " P\n";
    std::cout << "u = n + " << u_lin << "x - " << -u_quad << "x^2\n";

    auto consumer = sim->create<Polynomial>(std::map<eris_id_t, std::vector<double>> {{money, {1}}, {x, {u_lin, u_quad}}});

    std::cout << "Consumer utility at n=700, x=30: " << consumer->utility(Bundle({{ money, 700 }, { x, 30 }})) << "\n";

    // Need a FirmFactory that enters if existing firms earned positive profits last period

    // A random firms need to exit if firm profits were negative last period
}
