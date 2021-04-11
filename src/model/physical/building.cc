#include "model/physical/building.h"
using namespace std;
Building::Building(string name) : PhysicalModel{name} {
}
void Building::init(Id id, vector<string>& params) {
    this->set_id(id);
    // TODO
    this->set_name(params[3]);
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