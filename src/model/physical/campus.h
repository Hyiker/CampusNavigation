#ifndef CAMPUS_H
#define CAMPUS_H
#include <unordered_set>
#include "model/physical/physical_model.h"
// Campus is an object representing a location "The gate of a campus"
// it is designed for campus bus transport
class Campus : PhysicalModel {
   private:
    std::unordered_set<Id> connections;

   public:
   // could only connect to paths
    Id connect_to(std::shared_ptr<PhysicalModel>);
};
#endif