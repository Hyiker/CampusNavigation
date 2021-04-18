#ifndef COURSE_H
#define COURSE_H
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include "model/logical/logical_model.h"


class Course : public LogicalModel {
   protected:
    std::string _class;
    std::string time;
    std::string courseName;
    Id physicalId;

    friend class CourseHub;

   public:
    Course() = default;
    Course(std::string name);  // 2019211318班,数据结构课程设计,周四8:00
    void init(Id, std::vector<std::string>&) override;
    ~Course() = default;
    Id getPhysicalId();
};

#endif