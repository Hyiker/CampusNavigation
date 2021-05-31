#ifndef MODEL_PHYSICAL_BUILDING_H
#define MODEL_PHYSICAL_BUILDING_H
#include <memory>
#include <string>
#include <unordered_set>
#include "model/physical/physical_model.h"
class Building : public PhysicalModel {
   private:
    // ids of the path connected
    std::unordered_set<Id> connections;
    std::vector<double> coordinate;
    double width;
    double height;
    std::string shape;

   protected:
    Id connect_to(std::shared_ptr<PhysicalModel>);

   public:
    Building() = default;
    // building param format:
    // id,type,name
    void init(Id, std::vector<std::string>&) override;
    Building(std::string);
    std::unordered_set<Id>& get_connections();
    double get_width();
    double get_height();
    std::string get_shape();
    std::vector<double> get_coordinate();
};

#endif  // !MODEL_PHYSICAL_BUILDING_H