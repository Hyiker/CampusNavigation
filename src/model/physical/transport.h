#ifndef TRANSPORT_H
#define TRANSPORT_H
#include <chrono>
#include "model/physical/path.h"
class Transport : public Path {
   private:
    StdTime from, to;
    // speed scale for the transport
    // default is 1.0
    float speed;
   public:
    Transport(std::string, StdTime, StdTime, float = 1.0);
};

#endif