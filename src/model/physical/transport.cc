#include "model/physical/transport.h"
using std::string;
Transport::Transport(string name, StdTime from, StdTime to, float speed)
    : PhysicalPath{name}, from{from}, to{to}, speed{speed} {
}
void Transport::init(Id id, std::vector<std::string>& params) {
    // id,type,name,id1,id2,distance,speed,from,to
    this->set_id(id);
    if (params.size() < 9) {
        // TODO: throw error here
        return;
    }
    this->set_name(params[2]);
    this->model1_id = stoi(params[3]);
    this->model2_id = stoi(params[4]);
    this->distance = stoi(params[5]);
    this->speed = stof(params[6]);
    // TODO: assign from and to
}