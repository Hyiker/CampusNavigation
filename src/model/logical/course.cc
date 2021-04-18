#include "course.h"

Course::Course(string name) : LogicalModel{name} {
    int nums = std::count(name.begin(), name.end(), '-');
    if (nums == 2) {
        int separator[2] = {0};
        separator[0] = name.find('-');
        separator[1] = name.rfind('-');
        this->_class = name.substr(0, separator[0] - 1);
        this->courseName = name.substr(separator[0] + 1, separator[1] - separator[0] - 1);
        this->time = name.substr(separator[1] + 1);
    } else if (nums == 1) {
        int separator = name.find('-');
        this->_class = name.substr(0, separator - 1);
        this->courseName = name.substr(separator + 1);
        this->time = "";
    }
}

void Course::init(Id id, std::vector<std::string>& params) {
    this->set_id(id);
    if (params.size() == 5) {
        this->_class = params[2];
        this->courseName = params[3];
        this->time = "";
    } else if (params.size() == 5) {
        this->_class = params[2];
        this->courseName = params[3];
        this->time = params[4];
    } else {
        // TODO: throw error here
        return;
    }
}

bool Course::operator==(const Course& courseB) {
    if (this->time == "" || courseB.time == "") {
        if (this->_class == courseB._class && this->courseName == courseB.courseName) {
            return 1;
        }
    } else if (this->_class == courseB._class && this->courseName == courseB.courseName && this->time == courseB.time) {
        return 1;
    }
    return 0;
}