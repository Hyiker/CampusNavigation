#include "courseHub.h"

Course::Course(string name) {
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
    return course_map[course_ptr];
}

Id CourseHub::add(std::shared_ptr<Course> course_ptr, Id physicalId) {
    this->course_map.insert({course_ptr, physicalId});
    return physicalId;
}

std::shared_ptr<Course> CourseHub::add(string name, Id physicalId) {
    auto s = std::make_shared<Course>(name);
    this->course_map.insert({s, physicalId});
    return s;
}

bool CourseHub::have(std::shared_ptr<Course> course_ptr) {
    return this->course_map.count(course_ptr) != 0;
}

int CourseHub::remove(std::shared_ptr<Course> course_ptr) {
    if (!this->have(course_ptr)) {
        return -1;
    }
    this->course_map.erase(course_ptr);
    return 0;
}

std::unordered_map<std::shared_ptr<Course>, Id> CourseHub::search(string name) {
    std::unordered_map<std::shared_ptr<Course>, Id> ret_map;
    auto s = std::make_shared<Course>(name);
    for (auto it = this->course_map.begin(); it != this->course_map.end(); it++) {
        if (*s == *(it->first)) {
            ret_map.insert(*it);
        }
    }
    return ret_map;
}

int CourseHub::remove(string name) {
    auto result_map = this->search(name);
    std::unordered_map<std::shared_ptr<Course>, Id> empty_map;
    if (result_map == empty_map) {
        return -1;
    }
    for (auto it = result_map.begin(); it != result_map.end(); it++) {
        this->remove(it->first);
    }
    return 0;
}
