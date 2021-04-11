#ifndef TRANSPORT_H
#define TRANSPORT_H
#include <chrono>
#include "model/physical/path.h"
class Transport : public Path {
   protected:
    StdTime from, to;
    // speed scale for the transport
    // default is 1.0
    float speed;

   public:
    Transport() = default;
    Transport(std::string, StdTime, StdTime, float = 1.0);
    // format:
    // id,type,name,id1,id2,distance,speed,from,to
    void init(Id, std::vector<std::string>&) override;
};

#endif