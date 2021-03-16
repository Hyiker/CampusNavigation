#ifndef MODEL_PHYSICAL_BUILDING_H
#define MODEL_PHYSICAL_BUILDING_H
#include <memory>
#include <string>
#include <unordered_set>
#include "model/physical/path.h"
#include "model/physical/physical_model.h"
class Building : public PhysicalModel {
   private:
    // ids of the path connected
    std::unordered_set<Id> connections;

   protected:
    Id connect_to(std::shared_ptr<PhysicalModel>);

   public:
    Building(std::string);
    std::unordered_set<Id>& get_connections();
};

#endif  // !MODEL_PHYSICAL_BUILDING_H