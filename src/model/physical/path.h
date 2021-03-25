#ifndef MODEL_PHYSICAL_PATH_H
#define MODEL_PHYSICAL_PATH_H
#include <memory>
#include "model/physical/building.h"
#include "model/physical/physical_model.h"

using Distance = uint32_t;
class Path : public PhysicalModel {
   private:
    // the two building it connects
    Id model1_id = -1, model2_id = -1;
    Distance distance;

   protected:
    Path(std::string);

   public:
    Path();
    // return the model id if succeed else -1
    Id connect_to(std::shared_ptr<PhysicalModel>);
    Distance get_distance();
};

#endif  // !MODEL_PHYSICAL_PATH_H