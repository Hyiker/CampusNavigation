#include "model/physical/path.h"
Path::Path() : PhysicalModel{""}, model1_id{-1}, model2_id{-1} {
}
Id Path::connect(Id id) {
    if (this->model1_id == -1) {
        this->model1_id = id;
    } else if (this->model2_id == -1) {
        this->model2_id = id;
    } else {
        return -1;
    }
    return id;
}