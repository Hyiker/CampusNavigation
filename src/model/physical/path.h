#ifndef MODEL_PHYSICAL_PATH_H
#define MODEL_PHYSICAL_PATH_H
#include <memory>
#include "../model.h"
#include "./building.h"

class Path : Model {
   private:
    // the two building it connects
    Id model1_id, model2_id;

   public:
    Path();
    // return the model id if succeed else -1
    Id connect(Id);
};

#endif  // !MODEL_PHYSICAL_PATH_H