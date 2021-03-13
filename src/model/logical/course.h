#ifndef COURSE_H
#define COURSE_H
#include <chrono>
#include "model/model.h"

class Course : public Model {
    private:
        std::chrono::time_point<std::chrono::system_clock> start, end;
        
};

#endif