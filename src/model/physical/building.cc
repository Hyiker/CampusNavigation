#include "model/physical/building.h"
#include "error/errors.hpp"
using namespace std;
Building::Building(string name) : PhysicalModel{name} {
}
void Building::init(Id id, vector<string>& params) {
    this->set_id(id);
    // TODO
    if (params.size() < 3) {
        // TODO: throw error here
        return;
    }

    this->set_name(params[2]);
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