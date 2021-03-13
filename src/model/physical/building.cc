#include "model/physical/building.h"
using std::shared_ptr, std::string, std::unordered_set;
Building::Building(string name) : PhysicalModel{name} {
}
Id Building::connect_to(std::shared_ptr<PhysicalModel> pm) {
    if (auto path = std::static_pointer_cast<Path>(pm)) {
        this->connections.insert(path->get_id());
        return path->get_id();
    } else {
        // TODO: throw error here
        return -1;
    }
}
unordered_set<Id>& Building::get_connections() {
    return this->connections;
}