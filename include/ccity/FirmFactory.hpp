#pragma once
#include <eris/Bundle.hpp>
#include "ccity/Firm.hpp"

using eris::Position;
using eris::Bundle;

namespace ccity {

/** Circular city firm factory.  This looks to see if any firm would earn profits above a given
 * fixed level by entering at any midpoint between existing adjacent firms.  If so, it creates a new
 * Simulation firm (between periods); if not it takes no action.
 */
class FirmFactory : public eris::Member, public virtual eris::interopt::OptApply {
    public:
        /** Creates a new FirmFactory that creates firms if the newly created firm would earn a
         * profit of at least `threshold`.  At most `max_entrants` (defaults to 1) are created per
         * period, and at most `max_firms` firms may exist at once (defaults to 0, which means
         * unlimited).
         */
        FirmFactory(Bundle threshold, unsigned int max_entrants = 1, unsigned int max_firms = 0);

        /** Look at all the positions halfway between two adjacent firms, and calculates the profit
         * that a firm at that position could earn.  A new firm will be created at whichever
         * positions earn the largest profits, up to `max_entrants`, but only if the profit levels
         * exceeds `threshold`.
         */
        void interOptimize() override;

        /** Creates firms (if appropriate) at the position calculated in interOptimize().
         */
        void interApply() override;

    private:
        Bundle threshold_;
        unsigned int max_entrants_, max_firms_;
        
};

}
