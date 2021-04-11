#include <iostream>
#include "boot/bootloader.h"
#include "logger/logger.h"
int main(int argc, const char** argv) {
    Logger::init("stderr", FileMode::OVERRIDE, LogLevel::DEBUG);
    if (argc < 2) {
        Logger::error("没有输入模型文件路径");
    }

    BootLoader::load_model_hub(argv[1]);
    return 0;
}