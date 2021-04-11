#include "model/physical/path.h"
using namespace std;
Path::Path(string name) : PhysicalModel{""}, model1_id{-1}, model2_id{-1} {
}
Path::Path() : Path{""} {
}
Id Path::connect_to(shared_ptr<PhysicalModel> pm) {
    if (this->model1_id == -1) {
        return this->model1_id = pm->get_id();
    } else if (this->model2_id == -1) {
        return this->model2_id = pm->get_id();
    }

    return -1;
}
Distance Path::get_distance() {
    return this->distance;
}

void Path::init(Id id, std::vector<std::string>& params) {
    // id,type,name,id1,id2,distance
    this->set_id(id);
    if (params.size() < 6) {
        // TODO: throw error here
        return;
    }
    this->set_name(params[2]);
    this->model1_id = stoi(params[3]);
    this->model2_id = stoi(params[4]);
    this->distance = stoi(params[5]);
}

pair<Id, Id> Path::get_connections() {
    return make_pair(this->model1_id, this->model2_id);
}