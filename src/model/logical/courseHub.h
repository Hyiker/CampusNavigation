#ifndef COURSE_HUB_H
#define COURSE_HUB_H
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "course.h"
#include "logical_model.h"

using std::string;

class CourseHub {
   private:
    std::unordered_map<std::shared_ptr<Course>, Id> course_map;

   public:
    Id add(std::shared_ptr<Course>, Id);
    std::shared_ptr<Course> add(string, Id);
    int remove(std::shared_ptr<Course>);
    int remove(string);
    bool have(std::shared_ptr<Course>);
    std::unordered_map<std::shared_ptr<Course>, Id> search(string);
    Id getPhysicalId(std::shared_ptr<Course>);
    CourseHub();
    ~CourseHub();
};

#endif
