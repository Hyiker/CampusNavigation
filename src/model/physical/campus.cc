#include "model/physical/campus.h"
#include "error/errors.hpp"
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
        throw InvalidTypeException(std::string("连接的目标必须是Path"));
        return -1;
    }
}