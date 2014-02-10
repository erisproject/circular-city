#include "ccity/FirmFactory.hpp"
#include <map>
#include <set>
#include <eris/Position.hpp>

using eris::Bundle;
using eris::SharedMember;

namespace ccity {

// Compares ccity::Firm positions
struct poscompare {
    inline bool operator() (const SharedMember<ccity::Firm> &a, const SharedMember<ccity::Firm> &b) {
        return (a->position()[0] < b->position()[0]); }
};

FirmFactory::FirmFactory(Bundle threshold, unsigned int max_entrants, unsigned int max_firms)
    : threshold_(threshold), max_entrants_(max_entrants), max_firms_(max_firms) {}

void FirmFactory::interOptimize() {
    auto firms = simulation()->agents<ccity::Firm>();
    unsigned int max_entry = max_entrants_;

    // Check to see if the city is full (or close to full), and if so, update max_entry appropriately.
    if (max_firms_ > 0 and firms.size() < max_firms_) {
        unsigned int firm_space = max_firms_ - firms.size();
        if (firm_space < max_entry) max_entry = firm_space;
    }

    if (max_entry == 0) return; // City is full: nothing to do

    std::multimap<double, double> profit; // Maps profits to positions

    // Consider all midpoint positions between pairs of adjacent firms and calculate the entry
    // profit at that midpoint.

    // FIXME: if only one firm, consider one point on the opposite side of the city

    // First we need to sort all firms by their position
    std::multiset<SharedMember<ccity::Firm>, poscompare> firms_sorted(firms.begin(), firms.end());

    auto it = firms_sorted.begin();
    auto it_end = firms_sorted.end();
    auto it_next = ++it;
    while (it != it_end) {
        double left = (*it)->position()[0];
        double right = it_next == it_end
            ? (*(firms_sorted.begin()))->position()[0] + (bound_right_ - bound_left_) // Wrapping
            : (*it_next)->position()[0];

        Position p({(left + right) * 0.5});
        (*it)->wrap(p);

        // Consider entering here:
        double entry = p[0];
        if (entry == left or entry == right)
            continue; // If we ended exactly at the position of one of the firms don't enter

        double profit;
    }
}

void FirmFactory::interApply() {
}

}
