#include <iostream>
#include "boot/bootloader.h"
#include "logger/logger.h"
#include "model/model_hub.h"
int main(int argc, const char** argv) {
    std::string path = "../test/test_data/algo/model.csv";
    Logger::init("stderr", FileMode::OVERRIDE, LogLevel::DEBUG);
    /*if (argc < 2) {
        Logger::error("没有输入模型文件路径");
    }*/
    std::shared_ptr<ModelHub> modelHub = BootLoader::load_model_hub(path);
    return 0;
}