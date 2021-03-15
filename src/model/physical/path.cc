#include "model/physical/path.h"
using std::shared_ptr;
Path::Path() : PhysicalModel{""}, model1_id{-1}, model2_id{-1} {
}
Id Path::connect_to(shared_ptr<PhysicalModel> pm) {
    if (this->model1_id == -1) {
        return this->model1_id = pm->get_id();
    } else if (this->model2_id == -1) {
        return this->model2_id = pm->get_id();
    }

    return -1;
}
Distance Path::get_distance() {
    return this->distance;
}