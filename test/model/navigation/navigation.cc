#include <boot/bootloader.h>
#include <gtest/gtest.h>
#include "error/errors.hpp"
#include "model/model.h"
#include "model/model_hub.h"
#include "model/physical/building.h"
#include "model/physical/path.h"
using namespace std;

TEST(nav, nav_test) {
    std::string path = "test/test_data/algo/model.csv";
    Logger::init("stderr", FileMode::OVERRIDE, LogLevel::DEBUG);
    std::shared_ptr<ModelHub> modelHub = BootLoader::load_model_hub(path);
    std::vector<std::shared_ptr<Model>> expectRes;
    expectRes.push_back(modelHub->get(0));
    expectRes.push_back(modelHub->get(31));
    expectRes.push_back(modelHub->get(3));
    expectRes.push_back(modelHub->get(20));
    expectRes.push_back(modelHub->get(5));
    expectRes.push_back(modelHub->get(27));
    expectRes.push_back(modelHub->get(7));

    auto realRes = modelHub->navigate(0, 7, 1);
    for (auto i = 0; i < realRes.size(); i++) {
        EXPECT_EQ(expectRes[i], realRes[i]);
    }
}
