#include "model/logical/course.h"

Course::Course(string name) : LogicalModel{name} {
    int separator[2] = {0};
    separator[0] = name.find('-');
    separator[1] = name.rfind('-');
    Class = name.substr(0, 10);
    courseName = name.substr(separator[0] + 1, separator[1] - separator[0] - 1);
    time = name.substr(separator[1] + 1);
}

Id Course::getPhysicalModel() {
    return -1;
}