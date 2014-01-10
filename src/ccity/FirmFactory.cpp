#include "ccity/FirmFactory.hpp"

using eris::Bundle;

namespace ccity {

FirmFactory::FirmFactory(Bundle threshold, unsigned int max_entrants, unsigned int max_firms)
    : threshold_(threshold), max_entrants_(max_entrants), max_firms_(max_firms) {}

void FirmFactory::interOptimize() {
    //auto firms = 
    if (max_entrants_ > 0 and max_firms_ > 0) { }
}

void FirmFactory::interApply() {
}

}
