#include <iostream>
#include <memory>
#include "boot/bootloader.h"
#include "logger/logger.h"
#include "model/logical/course.h"
#include "model/model_hub.h"
int main(int argc, const char** argv) {
    std::string path = "../test/test_data/algo/model.csv";
    Logger::init("stderr", FileMode::OVERRIDE, LogLevel::DEBUG);
    /*if (argc < 2) {
        Logger::error("没有输入模型文件路径");
    }*/
    std::shared_ptr<ModelHub> modelHub = BootLoader::load_model_hub(path);
    auto test = modelHub->navigate(0, 7, 1);
    for (auto it = test.begin(); it != test.end(); it++) {
        std::cout << (*it)->get_id() << ' ';
    }

    return 0;
}