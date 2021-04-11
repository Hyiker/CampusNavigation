#include "model/physical/building.h"
#include "error/errors.hpp"
using std::shared_ptr, std::string, std::unordered_set;
Building::Building(string name) : PhysicalModel{name} {
}
Id Building::connect_to(std::shared_ptr<PhysicalModel> pm) {
    if (auto path = std::static_pointer_cast<Path>(pm)) {
        this->connections.insert(path->get_id());
        return path->get_id();
    } else {
        throw InvalidTypeException(std::string("连接的目标必须是Path"));
    }
}
unordered_set<Id>& Building::get_connections() {
    return this->connections;
}