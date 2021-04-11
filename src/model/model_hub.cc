#include "model/model_hub.h"
#include <boost/format.hpp>
#include "error/errors.hpp"
using std::shared_ptr, std::vector, std::pair, std::make_pair;
Id ModelHub::generate_id() {
    return (this->gen)++;
}
ModelHub::ModelHub(Id begin) : gen(begin) {
}
Id ModelHub::add(shared_ptr<Model> model_ptr) {
    Id id = this->generate_id();
    this->model_map.insert({id, model_ptr});
    return id;
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
