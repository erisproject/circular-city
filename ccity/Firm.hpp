#include <eris/firm/PriceFirm.hpp>
#include <eris/agent/CircularPosAgent.hpp>
#include <eris/Position.hpp>
#include <eris/Optimize.hpp>

using eris::Position;

namespace ccity {

/** Circular city firm.  Each period the firm contemplates increasing or decreasing price, and
 * moving left or right, taking any of those steps if beneficial, staying put otherwise.
 *
 * If the firm earns negative profits in a period, it leaves the market with a particular
 * probability (but only one firm may leave per period).
 */
class Firm : public eris::firm::PriceFirm, public eris::agent::CircularPosAgent,
        public virtual eris::interopt::OptApply {
    public:
        /** Constructs a firm that starts at the given position and price.  The firm is on a 0-1
         * circular interval, where 0 and 1 are connected (and thus the same point).
         */ 
        Firm(const Position &p, const double &initial_price);

        /** Calculates the effects of taking a step left, taking a step right, or staying put, and
         * picks the best option.  Then, having decided on a position, it calculates the profits
         * that would result from increasing price, from decreasing price, or from leaving price
         * alone, and changes price accordingly.  Movement accelerates if successive steps are in
         * the same direction, and decelerates if step direction fluctuates.
         */
        virtual void interOptimize();

        /** Takes the move calculated in interOptimize().
         */
        virtual void interApply();

}
