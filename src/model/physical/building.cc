#include "./building.h"
using std::string, std::vector;
Building::Building(string name): Model{name}{
    
}
void Building::connect(Id path_id){
    this->connections.push_back(path_id);
}
vector<Id> Building::get_connections(){
    return this->connections;
}