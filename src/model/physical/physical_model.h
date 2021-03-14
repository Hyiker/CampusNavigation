#ifndef PHYSICAL_MODEL_H
#define PHYSICAL_MODEL_H
#include "model/model.h"

class PhysicalModel : public Model {
   protected:
    PhysicalModel() = default;
    PhysicalModel(std::string);
    virtual ~PhysicalModel() = default;
};

#endif