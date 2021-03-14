#include "model/physical/building.h"
using std::string, std::vector;
Building::Building(string name) : PhysicalModel{name} {
}
void Building::connect(Id path_id) {
    this->connections.push_back(path_id);
}
vector<Id> Building::get_connections() {
    return this->connections;
}