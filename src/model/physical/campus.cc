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

void Campus::init(Id id, std::vector<std::string>& params) {
    this->set_id(id);
    if (params.size() < 3) {
        // TODO: throw error here
        return;
    }
    this->set_name(params[2]);
}