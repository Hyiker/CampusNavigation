#ifndef TRANSPORT_H
#define TRANSPORT_H
#include <chrono>
#include "model/physical/path.h"

class Transport: public Path {
    private:
        std::chrono::time_point<std::chrono::system_clock> available_duration[2];
    public:
};

#endif