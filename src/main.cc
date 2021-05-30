#include <iostream>
#include <memory>
#include "boot/bootloader.h"
#include "cli/cli.hpp"
#include "logger/logger.h"
#include "model/logical/course.h"
#include "model/model_hub.h"
#include "server/server.h"

int main(int argc, const char** argv) {
    
    std::string path = "../test/test_data/algo/model.csv";
    Logger::init("stderr", FileMode::OVERRIDE, LogLevel::DEBUG);
    std::shared_ptr<ModelHub> model_hub = BootLoader::load_model_hub(path);
    server_init(model_hub);
    return 0;
}