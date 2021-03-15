#ifndef PHYSICAL_MODEL_H
#define PHYSICAL_MODEL_H
#include "model/model.h"
class PhysicalModel : public Model {
   protected:
    PhysicalModel() = default;
    PhysicalModel(std::string);
    // connect self to a specific Physical Model
    // return the connected model id if success else -1
   public:
    virtual Id connect_to(std::shared_ptr<PhysicalModel>) = 0;
    ~PhysicalModel() = default;
};

#endif