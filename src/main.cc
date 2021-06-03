#include <boost/program_options.hpp>
#include <csignal>
#include <iostream>
#include <memory>
#include <string>
#include "boot/bootloader.h"
#include "cli/cli.hpp"
#include "logger/logger.h"
#include "model/logical/course.h"
#include "model/model_hub.h"
#include "server/server.h"

extern "C" {
    void signal_handler(int sig){
        if(sig == SIGINT){
            delete Logger::get_instance();
            exit(0);
        }
    }
}
int main(int argc, const char** argv) {
    namespace po = boost::program_options;
    signal(SIGINT, signal_handler);

    std::string map_path = "./model.csv";
    std::string log_path = "stderr";
    int idx = 1;

    if (argc >= 3) {
        while (idx < argc - 1) {
            if (std::string(argv[idx]) == "-p" || std::string(argv[idx]) == "--path") {
                map_path = argv[idx + 1];
            } else if (std::string(argv[idx]) == "-l" || std::string(argv[idx]) == "--log") {
                log_path = argv[idx + 1];
            }
            idx++;
        }
    }
    Logger::init(log_path, FileMode::OVERRIDE, LogLevel::INFO);
    std::shared_ptr<ModelHub> model_hub = BootLoader::load_model_hub(map_path);
    server_init(model_hub);
    return 0;
}