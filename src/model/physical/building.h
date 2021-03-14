#ifndef MODEL_PHYSICAL_BUILDING_H
#define MODEL_PHYSICAL_BUILDING_H
#include <memory>
#include <string>
#include <vector>
#include "model/physical/path.h"
#include "model/physical/physical_model.h"
class Building : public PhysicalModel {
   private:
    // ids of the path connected
    std::vector<Id> connections;

   public:
    Building(std::string);
    void connect(Id id);
    std::vector<Id> get_connections();
};

#endif  // !MODEL_PHYSICAL_BUILDING_H