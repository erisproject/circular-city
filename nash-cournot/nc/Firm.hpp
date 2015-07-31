#pragma once
#include <eris/Firm.hpp>
#include <random>

namespace nc {

/** Nash-Cournot firm.  This is a simple object that sometimes optimizes by adjusting quantities up
 * or down by a small amount; if the adjustment increases profits, the change is kept for future
 * periods, otherwise it is reverted.
 *
 * Optimizing can be controlled by setting to `optimize` member: when false, the firm does no
 * quantity adjustments.
 */
class Firm : public eris::Agent, public virtual eris::intraopt::Apply, public virtual eris::interopt::Apply {
    public:
        double quantity = 0; ///< Current quantity level
        double profit = 0; ///< Most recent profit
        /** True if this firm is allowed to optimize this period (the optimizing will also extend
         * into the following inter-period phase, but this variable is only used during
         * intraApply()).
         */
        bool optimize = false;
        double epsilon = 0.01; ///< The adjustment amount when optimizing
        std::bernoulli_distribution flip_negative; ///< Used to determine whether to use +epsilon or -epsilon

        /// If `optimize` is true, adjust current quantity by `epsilon`
        virtual void intraApply() override;
        /** If we optimized last period, when advancing to the next period we either keep the
         * quantity adjustment, or revert it, based on whether the adjustment increased or decreased
         * profits.
         */
        virtual void interApply() override;

    private:
        // The values before adjusting (only used in a `optimize = true` period):
        double last_quantity_ = 0, last_profit_ = 0;
        // Basically the same as optimize, but stored separately (in case optimize changes between
        // optimize/finish phases).
        bool optimizing_ = false;
};

}
