#include <boot/bootloader.h>
#include <gtest/gtest.h>
#include "error/errors.hpp"
#include "model/model.h"
#include "model/model_hub.h"
#include "model/physical/building.h"
#include "model/physical/path.h"
using namespace std;

TEST(nav, nav_test) {
    std::string path = "../test/test_data/algo/model.csv";
    std::shared_ptr<ModelHub> modelHub = BootLoader::load_model_hub(path);

    std::vector<std::pair<std::shared_ptr<Model>, Distance>> res;

    EXPECT_EQ(modelHub->navigate(0, 7), res);
}
