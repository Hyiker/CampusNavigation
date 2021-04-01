#ifndef LOGICAL_MODEL_H
#define LOGICAL_MODEL_H
#include <memory>
#include "model/model.h"
#include "model/model_hub.h"
#include "model/physical/physical_model.h"

class LogicalModel : public Model {
   protected:
    LogicalModel() = default;
    LogicalModel(std::string);

   public:
    friend class ModelHub;
    ~LogicalModel() = default;
};

#endif