#ifndef COURSE_H
#define COURSE_H
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include "logical_model.h"

using std::string;

class Course : public LogicalModel {
   protected:
    string _class;
    string time;
    string courseName;

    friend class CourseHub;

   public:
    Course() = default;
    Course(string name);  // 2019211318班,数据结构课程设计,周四8:00
    void init(Id, std::vector<std::string>&) override;
    ~Course() = default;
    bool operator==(const Course&);
};

#endif