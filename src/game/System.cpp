#include "System.h"
#include <flgl/logger.h>
LOG_MODULE(sys);

SystemList::SystemList() {
    memset(systems, 0x00, MAX_SYSTEMS * sizeof(system_t));
}

system_t& SystemList::next_system() {
    return systems[top++];
}

SystemList::SystemIt::SystemIt(SystemList& l, bool d) : list(l) {
    done = i = d;
}

SystemList::SystemIt& SystemList::SystemIt::operator++() {
    done = i == (MAX_SYSTEMS-1);
    i++;
    return *this;
}

SystemList::SystemIt SystemList::begin() {
    return SystemIt(*this);
}

SystemList::SystemIt SystemList::end() {
    return SystemIt(*this, 1);
}

