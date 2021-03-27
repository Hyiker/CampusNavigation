#include "courseHub.h"

Course::Course(string name) {
    int separator[2] = {0};
    separator[0] = name.find('-');
    separator[1] = name.rfind('-');
    this->Class = name.substr(0, 10);
    this->courseName = name.substr(separator[0] + 1, separator[1] - separator[0] - 1);
    this->time = name.substr(separator[1] + 1);
}

bool Course::operator==(const Course& courseB) {
    if (this->Class == courseB.Class && this->courseName == courseB.courseName && this->time == courseB.time) {
        return 1;
    }
    return 0;
}

CourseHub::CourseHub() {
}

CourseHub::~CourseHub() {
}

/*CourseHub::~CourseHub() {
    std::ofstream outFile;
    outFile.open("/mnt/c/Users/16078/Desktop/CampusNavigation/src/model/logical/courseList.csv", std::ios::out);
    auto it = this->course_map.begin();
    while (it != course_map.end()) {
        outFile << it->first->Class << ',' << it->first->courseName << ',' << it->first->time << ',' << it->second;
        it++;
    }
}*/

Id CourseHub::getPhysicalId(std::shared_ptr<Course> course_ptr) {
    auto p = this->have(course_ptr);
    if (!p) {
        return -1;
    }
    return course_map[p];
}

Id CourseHub::add(std::shared_ptr<Course> course_ptr, Id physicalId) {
    this->course_map.insert({course_ptr, physicalId});
    return physicalId;
}

std::shared_ptr<Course> CourseHub::have(std::shared_ptr<Course> course_ptr) {
    auto it = this->course_map.begin();
    while (it != this->course_map.end()) {
        if (*(it->first) == *(course_ptr)) {
            return it->first;
        }
        it++;
    }
    return nullptr;
}

Id CourseHub::remove(std::shared_ptr<Course> course_ptr) {
    auto p = this->have(course_ptr);
    if (!p) {
        return -1;
    }
    Id physicalId = this->course_map[p];
    this->course_map.erase(p);
    return physicalId;
}
