#include "model/model_hub.h"
#include <boost/format.hpp>
#include <iostream>
#include "error/errors.hpp"
#include "model/logical/course.h"
#include "model/physical/building.h"
#include "model/physical/campus.h"
#include "model/physical/path.h"
#include "model/physical/transport.h"
using namespace std;

shared_ptr<Model> ModelHub::construct_with_list(string& model_type, vector<string>& params) {
    if (params.size() < 2) {
        // TODO: throw error here
        return nullptr;
    }

    // TODO: implement me
    if (Logger::get_instance() != nullptr) {
        auto fmt = boost::str(boost::format("adding %1% to hub") % model_type);
        Logger::debug(fmt);
    }
    Id id = stoi(params[0]);

    shared_ptr<Model> model_ptr;
    if (model_type == BUILDING_STR) {
        model_ptr = make_shared<Building>();
    } else if (model_type == CAMPUS_STR) {
        model_ptr = make_shared<Campus>();
    } else if (model_type == PATH_STR) {
        model_ptr = make_shared<Path>();
    } else if (model_type == TRANSPORT_STR) {
        model_ptr = make_shared<Transport>();
    } else if (model_type == COURSE_STR) {
        model_ptr = make_shared<Course>();
    }

    model_ptr->init(id, params);
    this->add(model_ptr);

    return model_ptr;
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
        throw InvalidIdException(boost::str(boost::format("未找到id%1%对应的模型") % id));
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

Id ModelHub::search_name(string name) {
    for (auto it = this->model_map.begin(); it != this->model_map.end(); it++) {
        if (it->second->get_name() == name) {
            return it->first;
        }
    }
    return -1;
}