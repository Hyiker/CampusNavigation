#ifndef MODEL_PHYSICAL_PATH_H
#define MODEL_PHYSICAL_PATH_H
#include <memory>
#include <utility>
#include "model/physical/physical_model.h"

using Distance = double;
class PhysicalPath : public PhysicalModel {
   private:
   protected:
    // the two building it connects
    Id model1_id = -1, model2_id = -1;
    PhysicalPath(std::string);
    Distance distance;
    bool bicycle_able;
    // return the model id if succeed else -1
    Id connect_to(std::shared_ptr<PhysicalModel>);

   public:
    PhysicalPath();
    Distance get_distance();
    // format:
    // id,type,name,id1,id2,distance
    void init(Id, std::vector<std::string>&) override;
    std::pair<Id, Id> get_connections();
};

#endif  // !MODEL_PHYSICAL_PATH_H