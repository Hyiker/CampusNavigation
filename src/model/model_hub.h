#ifndef MODEL_HUB_H
#define MODEL_HUB_H
#include <utility>
#include "logger/logger.h"

#include "algo/navigation.h"
#include "model/logical/logical_model.h"
#include "model/model.h"
#include "model/physical/physical_model.h"

using Distance = double;
class PhysicalModel;
class ModelHub {
   private:
    std::unordered_map<Id, std::shared_ptr<Model>> model_map;
    navigation nav;

   public:
    ModelHub() = default;
    // construct a model ptr with specified type and param list
    // then add it to the model hub, connecting edges
    std::shared_ptr<Model> construct_with_list(std::string&, std::vector<std::string>&);
    Id add(std::shared_ptr<Model>);
    // return the removed id if exists
    // or -1 if failed to find or remove the model
    Id remove(Id);
    // true if hub has the corresponding model
    bool have(Id);
    std::shared_ptr<Model> get(Id);
    // the sequence is proved to be the same
    std::vector<std::shared_ptr<Model>> get(std::vector<Id>&);
    // connect two Physical Models
    std::pair<Id, Id> connect(std::shared_ptr<PhysicalModel>, std::shared_ptr<PhysicalModel>);
    std::vector<std::pair<Id, std::shared_ptr<Model>>> search_name(std::string);
    std::pair<std::vector<int>,double> navigate(Id, Id, int);
    std::vector<std::pair<int,double>> search_near_model(std::shared_ptr<Model>,int);
    // 1: walk
    // 2: crowdness
    // 3: bicycle
    std::shared_ptr<Model> find_edge(Id, Id, int);
};

#endif