#include "courseHub.h"

CourseHub::CourseHub() {
}

CourseHub::~CourseHub() {
}

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
