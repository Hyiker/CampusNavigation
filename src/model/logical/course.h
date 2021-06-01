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
    std::string course_name;
    Id physical_id;

    friend class CourseHub;

   public:
    Course() = default;
    Course(std::string name);
    void init(Id, std::vector<std::string>&) override;
    ~Course() = default;
    Id get_physical_id();
};

#endif