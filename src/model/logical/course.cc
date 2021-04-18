#include "model/logical/course.h"

using std::string;

Course::Course(string name) : LogicalModel{name} {
}

void Course::init(Id id, std::vector<std::string>& params) {
    this->set_id(id);
    if (params.size() == 4) {
        this->set_name(params[2]);
        string name = params[2];
        int separator[2] = {0};
        separator[0] = name.find('-');
        separator[1] = name.rfind('-');
        this->_class = name.substr(0, separator[0] - 1);
        this->courseName = name.substr(separator[0] + 1, separator[1] - separator[0] - 1);
        this->time = name.substr(separator[1] + 1);
        this->physicalId = stoi(params[3]);
    } else {
        // TODO: throw error here
        return;
    }
}

Id Course::getPhysicalId()
{
    return this->physicalId;
}