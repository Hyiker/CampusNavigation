#ifndef MODEL_LOGICAL_COURSE_H
#define MODEL_LOGICAL_COURSE_H
#include <chrono>
#include <string>
#include "model/logical/logical_model.h"
#include "model/physical/building.h"

using std::string;

class Course : public LogicalModel {
   public:
    string Class;
    string time;
    string courseName;

   protected:
    Id getPhysicalModel();

   public:
    Course(string name);  // 2019211318班-数据结构课程设计-周四8:00
    ~Course() = default;
};

#endif