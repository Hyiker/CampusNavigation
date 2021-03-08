#ifndef MODEL_PHYSICAL_PATH_H
#define MODEL_PHYSICAL_PATH_H
#include <memory>
#include "../model.h"
#include "./building.h"

class Path {
   private:
    Id id;
    // the two building it connects
    std::shared_ptr<Building> building1, building2;

   public:
    Path() = default;
    Path(Id, std::shared_ptr<Building>, std::shared_ptr<Building>);
    ~Path();
};

#endif  // !MODEL_PHYSICAL_PATH_H