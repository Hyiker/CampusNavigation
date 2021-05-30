#include "model/physical/building.h"
#include "error/errors.hpp"
#include "model/physical/path.h"
using namespace std;
Building::Building(string name) : PhysicalModel{name} {
}
void Building::init(Id id, vector<string>& params) {
    this->set_id(id);
    // TODO
    if (params.size() < 7) {
        // TODO: throw error here
        return;
    }
    this->set_name(params[2]);
    this->coordinate.push_back(stod(params[3]));
    this->coordinate.push_back(stod(params[4]));
    this->width = stod(params[5]);
    this->height = stod(params[6]);
}
Id Building::connect_to(std::shared_ptr<PhysicalModel> pm) {
    if (auto path = std::static_pointer_cast<PhysicalPath>(pm)) {
        this->connections.insert(path->get_id());
        return path->get_id();
    } else {
        throw InvalidTypeException(std::string("连接的目标必须是Path"));
    }
}
unordered_set<Id>& Building::get_connections() {
    return this->connections;
}

double Building::get_width()
{
    return this->width;
}

double Building::get_height()
{
    return this->height;
}

std::vector<double> Building::get_coordinate()
{
    return this->coordinate;
}