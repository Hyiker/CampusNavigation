#include "model/model.h"
#include <gtest/gtest.h>
#include "model/physical/building.h"
#include "model/physical/path.h"
using namespace std;
class Env : public ::testing::Test {
   protected:
    virtual void SetUp() {
        auto b1 = std::make_shared<Path>();
        b1->set_id(mh.add(b1));
        pid = b1->get_id();
    }
    ModelHub mh;
    Id pid;
};
TEST_F(Env, modelhub_building_test) {
    string building_names[5] = {"学一", "教室食堂一", "教师食堂二", "雁南", "雁北"};
    for (int i = 0; i < 5; i++) {
        auto b1 = std::make_shared<Building>(building_names[i]);
        b1->set_id(mh.add(b1));
    }
    for (uint32_t i = 0; i < 5u; i++) {
        ASSERT_EQ(mh.get(i + 1)->get_name(), building_names[i]);
    }
}
TEST_F(Env, modelhub_path_test) {
    auto b1 = std::make_shared<Path>();
    b1->set_id(mh.add(b1));
}
TEST_F(Env, connection_test) {
    string building_names[5] = {"学一", "教室食堂一", "教师食堂二", "雁南", "雁北"};
    for (int i = 0; i < 5; i++) {
        auto b1 = std::make_shared<Building>(building_names[i]);
        b1->set_id(mh.add(b1));
    }
    for (uint32_t i = 0; i < 5u; i++) {
        ASSERT_EQ(mh.get(i + 1)->get_name(), building_names[i]);
    }
    auto p = reinterpret_pointer_cast<Path>(mh.get(pid));
    ASSERT_EQ(p->connect(0u), 0u);
    p->connect(1u);
    auto x1 = reinterpret_pointer_cast<Building>(mh.get(1u));
    auto j1 = reinterpret_pointer_cast<Building>(mh.get(2u));
    x1->connect(p->get_id());
    j1->connect(p->get_id());
    auto con = x1->get_connections();
    ASSERT_EQ(mh.get(con)[0]->get_id(), p->get_id());
}