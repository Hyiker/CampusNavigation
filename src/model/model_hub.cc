#include "model/model_hub.h"
#include <algorithm>
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
        auto fmt = boost::str(boost::format("adding %1% to hub") % (model_type + " " + params[0]));
        Logger::debug(fmt);
    }
    Id id = stoi(params[0]);

    shared_ptr<Model> model_ptr;
    if (model_type == BUILDING_STR) {
        model_ptr = make_shared<Building>();
    } else if (model_type == CAMPUS_STR) {
        model_ptr = make_shared<Campus>();
    } else if (model_type == PATH_STR) {
        model_ptr = make_shared<PhysicalPath>();
    } else if (model_type == TRANSPORT_STR) {
        model_ptr = make_shared<Transport>();
    } else if (model_type == COURSE_STR) {
        model_ptr = make_shared<Course>();
    }

    model_ptr->init(id, params);

    if (model_type == PATH_STR) {
        auto path = static_pointer_cast<PhysicalPath>(model_ptr);
        this->nav.add_edge(path->get_connections().first, path->get_connections().second, path->get_distance(),
                           path->get_bicycle_able(), path->get_congestion_rate());
        this->nav.add_edge(path->get_connections().second, path->get_connections().first, path->get_distance(),
                           path->get_bicycle_able(), path->get_congestion_rate());
    }

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

static int model_cmp(std::pair<Id, std::shared_ptr<Model>> a, std::pair<Id, std::shared_ptr<Model>> b) {
    return a.first < b.first;
}

std::vector<std::pair<Id, std::shared_ptr<Model>>> ModelHub::search_name(string name) {
    std::vector<std::pair<Id, std::shared_ptr<Model>>> ret;
    for (auto it = this->model_map.begin(); it != this->model_map.end(); it++) {
        if (it->second->get_name().find(name) != -1) {
            ret.push_back(*it);
            continue;
        }
    }
    sort(ret.begin(), ret.end(), model_cmp);
    return ret;
}

std::shared_ptr<Model> ModelHub::find_edge(Id model_1, Id model_2, int method) {
    auto shortest_path = std::make_shared<PhysicalPath>();
    shortest_path = nullptr;
    for (auto it = this->model_map.begin(); it != this->model_map.end(); it++) {
        if (auto path = std::static_pointer_cast<PhysicalPath>(it->second)) {
            if (path->get_connections() == std::pair<Id, Id>(model_1, model_2) ||
                path->get_connections() == std::pair<Id, Id>(model_2, model_1)) {
                if (shortest_path == nullptr) {
                    shortest_path = path;
                } else {
                    switch (method) {
                        case 1:
                            if (path->get_distance() < shortest_path->get_distance()) {
                                shortest_path = path;
                            }
                            break;
                        case 2:
                            if (path->get_distance() * path->get_congestion_rate() <
                                shortest_path->get_distance() * shortest_path->get_congestion_rate()) {
                                shortest_path = path;
                            }
                            break;
                        case 3:
                            if (path->get_distance() * path->get_bicycle_able() <
                                shortest_path->get_distance() * shortest_path->get_bicycle_able()) {
                                shortest_path = path;
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
    return shortest_path;
}

std::pair<std::vector<int>,double> ModelHub::navigate(Id model_1, Id model_2, int method) {
    std::vector<int> ret;
    if (!this->have(model_1) || !this->have(model_2)) {
        throw InvalidIdException("导航目的地或者起始地无效!");
    }

    auto _model_1 = this->get(model_1);
    auto _model_2 = this->get(model_2);
    auto distance = this->nav.navigate(model_1, model_2, method);
    auto route = this->nav.get_route();
    for (auto it = route.begin(); it + 1 != route.end(); it++) {
        if (*it != *(it + 1)) {
            ret.push_back(this->get(*it)->get_id());
            ret.push_back(this->find_edge(*it, *(it + 1), method)->get_id());
        }
    }
    ret.push_back(this->get(route[route.size() - 1])->get_id());
    return {ret,distance};
}

std::vector<std::pair<int,double>> ModelHub::search_near_model(std::shared_ptr<Model> start_pos, int distance) {
    std::vector<std::pair<int,double>> ret;
    auto id_list = this->nav.search(start_pos->get_id(), distance);
    for (auto it = id_list.begin(); it != id_list.end(); it++) {
        if (it->first != start_pos->get_id()) {
            ret.push_back(*it);
        }
    }
    return ret;
}