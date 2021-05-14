#include <iostream>
#include <memory>
#include "boot/bootloader.h"
#include "cli/cli.hpp"
#include "logger/logger.h"
#include "model/logical/course.h"
#include "model/model_hub.h"
int main(int argc, const char** argv) {
    std::string path = "../test/test_data/algo/model.csv";
    Cli cli(path);
    cli.init();
    cli.loop();

    return 0;
}