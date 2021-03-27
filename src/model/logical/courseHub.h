#ifndef COURSE_HUB_H
#define COURSE_HUB_H
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "logical_model.h"

using std::string;

class Course {
   protected:
    string Class;
    string time;
    string courseName;

    friend class CourseHub;

   public:
    Course(string name);  // 2019211318班-数据结构课程设计-周四8:00
    ~Course() = default;
    bool operator==(const Course&);
};

class CourseHub : public LogicalModel {
   private:
    std::unordered_map<std::shared_ptr<Course>, Id> course_map;

   public:
    Id add(std::shared_ptr<Course>, Id);
    Id remove(std::shared_ptr<Course>);
    std::shared_ptr<Course> have(std::shared_ptr<Course>);
    Id getPhysicalId(std::shared_ptr<Course>);
    CourseHub();
    ~CourseHub();
};

#endif
