#include "model/logical/logical_model.h"
#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>
#include "model/logical/courseHub.h"
#include "model/model.h"
#include "model/model_hub.h"

using namespace std;
TEST(courseTest, testCourseParser) {
    string course_names[5] = {"2019211318班,数据结构课程设计,周四8:00", "2019211318班,数据结构课程设计,周四9:00",
                              "2019211319班,数据结构课程设计,周四10:00", "2019211319班,数据结构课程设计,周四11:00",
                              "2019211318班,数据结构课程设计,周四12:00"};
    Id physicalId[5] = {2, 4, 6, 7, 8};
    shared_ptr<Course> courses[5];
    CourseHub test_courseHub;
    for (auto i = 0; i < 5; i++) {
        courses[i] = test_courseHub.add(course_names[i], physicalId[i]);
    }
    std::unordered_map<shared_ptr<Course>, Id> expected_result = {{courses[0], 2}, {courses[1], 4}, {courses[4], 8}};
    auto test_result_map = test_courseHub.search("2019211318班,数据结构课程设计");
    EXPECT_EQ(test_result_map, expected_result);
    EXPECT_EQ(test_courseHub.remove("2019211318班,数据结构课程设计"), 0);
    EXPECT_EQ(test_courseHub.remove("2019211311班,数据结构课程设计"), -1);
    expected_result = {};
    test_result_map = test_courseHub.search("2019211318班-数据结构课程设计");
    EXPECT_EQ(test_result_map, expected_result);
}