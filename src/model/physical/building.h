#ifndef MODEL_PHYSICAL_BUILDING_H
#define MODEL_PHYSICAL_BUILDING_H
#include <memory>
#include <string>
#include <vector>
#include "../model.h"
#include "./path.h"
class Building {
   private:
    Id id;
    std::string name;
    std::vector<std::shared_ptr<Path>> connections;

   public:
    Building() = default;
    Building(Id, std::string);
    ~Building();
};

#endif  // !MODEL_PHYSICAL_BUILDING_H