#pragma once
#include <eris/firm/PriceFirm.hpp>
#include <eris/agent/CircularPosAgent.hpp>
#include <eris/Position.hpp>
#include <eris/Optimize.hpp>
#include <eris/Good.hpp>

using eris::Position;
using eris::SharedMember;
using eris::Good;
using eris::Bundle;

namespace ccity {

/** Circular city firm.  Each period the firm contemplates increasing or decreasing price, and
 * moving left or right, taking any of those steps if beneficial, staying put otherwise.
 *
 * If the firm earns negative profits below a certain threshold in a period, it leaves the market
 * with a particular probability based on the magnitude of the loss.
 */
class Firm : public eris::firm::PriceFirm, public eris::agent::CircularPosAgent,
        public virtual eris::interopt::OptApply {
    public:
        /** Constructs a firm that starts at the given position and price.  The firm is on a 0-1
         * circular interval, where 0 and 1 are connected (and thus the same point).
         */ 
        Firm(const Position &p, const SharedMember<Good> &money, const double &initial_price);

        /// Struct containing both price and profit
        struct { Bundle price, profit; } price_profit;

        /** Returns a struct consisting of the optimal price and profit at the given location, using
         * the current locations and prices of all other firms.
         */
        price_profit price_at(const double &location);

        /** Calculates the effects of taking a step left, taking a step right, or staying put, and
         * picks the best option.  Then, having decided on a position, it calculates the profits
         * that would result from increasing price, from decreasing price, or from leaving price
         * alone, and changes price accordingly.  Movement (of both position and price) accelerates
         * if successive steps are in the same direction, and decelerates if step directions
         * fluctuates.
         */
        virtual void interOptimize() override;

        /** Takes the move(s) calculated in interOptimize().
         */
        virtual void interApply() override;

    private:
        eris_id_t money_;
        double price_;
};

}
