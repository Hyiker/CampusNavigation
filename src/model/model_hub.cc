#include "model/model_hub.h"
#include <boost/format.hpp>
#include "model/physical/building.h"
#include "model/physical/campus.h"
#include "model/physical/path.h"
#include "model/physical/service.h"
#include "model/physical/transport.h"
using namespace std;

shared_ptr<Model> ModelHub::construct_with_list(string& model_type, vector<string>& params) {
    // TODO: implement me
    if (Logger::get_instance() != nullptr) {
        auto fmt = boost::str(boost::format("adding %1% to hub") % model_type);
        Logger::debug(fmt);
    }
    Id id = stoi(params[0]);
    if (model_type == BUILDING_STR) {
        auto building_ptr = make_shared<Building>();
        building_ptr->init(id, params);
    } else if (model_type == CAMPUS_STR) {
        auto campus_ptr = make_shared<Campus>();
        campus_ptr->init(id, params);
    } else if (model_type == PATH_STR) {
        auto path_ptr = make_shared<Path>();
        path_ptr->init(id, params);
    } else if (model_type == TRANSPORT_STR) {
        auto transport_ptr = make_shared<Transport>();
        transport_ptr->init(id, params);
    }

    return nullptr;
}
Id ModelHub::add(shared_ptr<Model> model_ptr) {
    this->model_map.insert({model_ptr->get_id(), model_ptr});
    return model_ptr->get_id();
}
Id ModelHub::remove(Id id) {
    if (this->model_map.count(id) == 0) {
        return -1;
    }

    this->model_map.erase(id);
    return id;
}
bool ModelHub::have(Id id) {
    return this->model_map.count(id) != 0;
}
shared_ptr<Model> ModelHub::get(Id id) {
    if (this->model_map.count(id) == 0) {
        // TODO: throws error here
        return nullptr;
    }
    return this->model_map[id];
}
vector<shared_ptr<Model>> ModelHub::get(vector<Id>& ids) {
    vector<shared_ptr<Model>> result;
    for (auto& id : ids) {
        result.push_back(this->get(id));
    }
    return result;
}

std::pair<Id, Id> ModelHub::connect(std::shared_ptr<PhysicalModel> pm1, std::shared_ptr<PhysicalModel> pm2) {
    pm1->connect_to(pm2);
    pm2->connect_to(pm1);
    return make_pair(pm1->get_id(), pm2->get_id());
}
