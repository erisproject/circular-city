#include "ccity/Firm.hpp"

namespace ccity {

Firm::Firm(const Position &p, const SharedGood<Good> &money, const double &initial_price) :
    money_(money), price_(initial_price) {}

Firm::price_profit Firm::price_at(const double &location) {
    // Get the pool of potential buyers
}

}
