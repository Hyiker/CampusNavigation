#include <iostream>
#include <memory>
#include "boot/bootloader.h"
#include "cli/cli.hpp"
#include "logger/logger.h"
#include "model/logical/course.h"
#include "model/model_hub.h"
#include "server/server.h"

int main(int argc, const char** argv) {
    
    /*
    std::string path = "../test/test_data/algo/model.csv";
    Cli cli(path);
    cli.init();
    cli.loop();
    */
    

    server_init();
    return 0;
}