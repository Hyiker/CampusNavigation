#include <gtest/gtest.h>
#include "model/model.h"
#include "model/model_hub.h"
#include "model/physical/building.h"
#include "model/physical/path.h"
using namespace std;
class Env : public ::testing::Test {
   protected:
    virtual void SetUp() {
        auto b1 = std::make_shared<Path>();
        b1->set_id(11);
        mh.add(b1);
        auto t = std::static_pointer_cast<Path>(mh.get(11));
        pid = b1->get_id();
    }
    ModelHub mh;
    Id pid;
};
TEST_F(Env, modelhub_building_test) {
    string building_names[5] = {"学一", "教室食堂一", "教师食堂二", "雁南", "雁北"};
    for (int i = 0; i < 5; i++) {
        auto b1 = std::make_shared<Building>(building_names[i]);
        b1->set_id(i);
        mh.add(b1);
    }
    for (uint32_t i = 0; i < 5u; i++) {
        ASSERT_EQ(mh.get(i)->get_name(), building_names[i]);
    }
}
TEST_F(Env, modelhub_path_test) {
    auto b1 = std::make_shared<Path>();
    b1->set_id(45);
    mh.add(b1);
}
TEST_F(Env, connection_test) {
    string building_names[5] = {"学一", "教室食堂一", "教师食堂二", "雁南", "雁北"};
    shared_ptr<Building> buildings[5];
    for (int i = 0; i < 5; i++) {
        auto b1 = std::make_shared<Building>(building_names[i]);
        b1->set_id(i);
        auto nid = mh.add(b1);
        buildings[i] = b1;
    }

    auto p = static_pointer_cast<Path>(mh.get(pid));
    auto con1 = mh.connect(buildings[2], p);
    auto con2 = mh.connect(buildings[1], p);
    ASSERT_EQ(con1.first, 2u);
    ASSERT_EQ(con2.first, 1u);
    auto x1 = static_pointer_cast<Building>(mh.get(2u));
    auto j1 = static_pointer_cast<Building>(mh.get(1u));
    // get the first connection path in x1
    auto con = *(x1->get_connections().begin());
    ASSERT_EQ(mh.get(con)->get_id(), p->get_id());
}