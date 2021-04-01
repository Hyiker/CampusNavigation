#include "model/physical/campus.h"
#include "model/physical/path.h"
using std::shared_ptr;
Id Campus::connect_to(shared_ptr<PhysicalModel> pm) {
    if (auto path = std::static_pointer_cast<Path>(pm)) {
        if (this->connections.count(pm->get_id()) != -1) {
            return -1;
        } else {
            return pm->get_id();
        }
    } else {
        // TODO: throws an error
        return -1;
    }
}