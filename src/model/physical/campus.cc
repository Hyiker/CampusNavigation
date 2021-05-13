#include "model/physical/campus.h"
#include "error/errors.hpp"
#include "model/physical/path.h"
using std::shared_ptr;
Id Campus::connect_to(shared_ptr<PhysicalModel> pm) {
    if (auto path = std::static_pointer_cast<PhysicalPath>(pm)) {
        if (this->connections.count(pm->get_id()) != -1) {
            return -1;
        } else {
            return pm->get_id();
        }
    } else {
        throw InvalidTypeException(std::string("连接的目标必须是Path"));
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