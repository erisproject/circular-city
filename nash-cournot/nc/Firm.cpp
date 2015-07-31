#include "nc/Firm.hpp"
#include <eris/Random.hpp>

using eris::Random;

namespace nc {

void Firm::intraApply() {
    optimizing_ = optimize;
    if (optimizing_) {
        last_quantity_ = quantity;
        last_profit_ = profit;

        double adjust = flip_negative(Random::rng()) ? -epsilon : epsilon;

        quantity += adjust;

        // We can't adjust to something less than 0: if that would happen, truncate to 0.
        if (quantity < 0) quantity = 0;
    }
}

void Firm::interApply() {
    if (optimizing_) {
        if (profit <= last_profit_) {
            // Revert:
            quantity = last_quantity_;
            // The following may not be true if other firms also adjusted (i.e. our actual profit if
            // we'd used `last_quantity_` this period might be something else), but it's the best we
            // have:
            profit = last_profit_;
        }
        // Otherwise profits increased, so keep the new quantity.

        optimizing_ = false; // Done with this optimization
    }
}



}
