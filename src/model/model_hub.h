#ifndef MODEL_HUB_H
#define MODEL_HUB_H
#include <utility>
#include "model/model.h"
#include "model/physical/physical_model.h"
constexpr Id DEFAULT_ID_BEGIN = 0;
class ModelHub {
   private:
    std::unordered_map<Id, std::shared_ptr<Model>> model_map;
    Id gen;
    Id generate_id();

   public:
    ModelHub(Id = DEFAULT_ID_BEGIN);
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
};
#endif