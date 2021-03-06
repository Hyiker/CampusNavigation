#include "model/physical/path.h"
#include "model/physical/building.h"
using namespace std;
PhysicalPath::PhysicalPath(string name) : PhysicalModel{""}, model1_id{-1}, model2_id{-1} {
}
PhysicalPath::PhysicalPath() : PhysicalPath{""} {
}
Id PhysicalPath::connect_to(shared_ptr<PhysicalModel> pm) {
    if (this->model1_id == -1) {
        return this->model1_id = pm->get_id();
    } else if (this->model2_id == -1) {
        return this->model2_id = pm->get_id();
    }

    return -1;
}
Distance PhysicalPath::get_distance() {
    return this->distance;
}

void PhysicalPath::init(Id id, std::vector<std::string>& params) {
    // id,type,name,id1,id2,distance,congestion,bicycle_able
    this->set_id(id);
    if (params.size() < 9) {
        // TODO: throw error here
        return;
    }
    this->set_name(params[2]);
    this->model1_id = stoi(params[3]);
    this->model2_id = stoi(params[4]);
    this->distance = stod(params[5]);
    this->congestion_rate = stod(params[6]);
    this->bicycle_able = stoi(params[7]);
    for (int i = 8; i < params.size(); i+=2)
    {
        this->anchors.push_back({stod(params[i]),stod(params[i+1])});      
    }
}

pair<Id, Id> PhysicalPath::get_connections() {
    return make_pair(this->model1_id, this->model2_id);
}

double PhysicalPath::get_congestion_rate() {
    return this->congestion_rate;
}

bool PhysicalPath::get_bicycle_able() {
    return this->bicycle_able;
}

vector<std::vector<double>> PhysicalPath::get_anchors()
{
    return this->anchors;
}