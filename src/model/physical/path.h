#ifndef MODEL_PHYSICAL_PATH_H
#define MODEL_PHYSICAL_PATH_H
#include <memory>
#include "model/physical/building.h"
#include "model/physical/physical_model.h"

class Path : public PhysicalModel {
   private:
    // the two building it connects
    Id model1_id = -1, model2_id = -1;

   public:
    Path();
    // return the model id if succeed else -1
    Id connect(Id);
};

#endif  // !MODEL_PHYSICAL_PATH_H