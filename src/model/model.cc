#include "model/model.h"
using std::shared_ptr, std::vector, std::string;

Model::Model(string name) : name{name} {
}
void Model::set_id(Id id) {
    this->id = id;
}
Id Model::get_id() {
    return this->id;
}
void Model::set_name(string name) {
    this->name = name;
}
string Model::get_name() {
    return this->name;
}
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
        // TODO: throws error here
        return nullptr;
    }
    return this->model_map[id];
}
vector<shared_ptr<Model>> ModelHub::get(std::vector<Id>& ids) {
    vector<shared_ptr<Model>> result;
    for (auto& id : ids) {
        result.push_back(this->get(id));
    }
    return result;
}