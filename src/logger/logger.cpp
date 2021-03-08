#include "logger.h"
#include <iostream>

using std::string, std::make_shared, std::shared_ptr, std::ios, std::ostream, std::make_unique, std::ofstream;

Logger::Logger(string& filename, LoggerMode mode) : filename{filename}, mode{mode} {
    auto open_mode = ios::app;
    if (mode == LoggerMode::APPEND) {
        open_mode = ios::app;
    } else if (mode == LoggerMode::OVERRIDE) {
        open_mode = ios::out;
    }
    if (filename == "stderr") {
        this->out = make_unique<ostream>(std::cerr);
    } else if (filename == "stdout") {
        this->out = make_unique<ostream>(std::cout);
    } else {
        this->out = make_unique<ofstream>(filename, open_mode);
    }
}

void Logger::init(string& filename, LoggerMode mode) {
    if (Logger::instance != nullptr) {
        Logger::instance = make_shared<Logger>(filename, mode);
    }
}

shared_ptr<Logger> Logger::get_instance() {
    return Logger::instance;
}
Logger::~Logger() {
    if (filename != "stderr" && filename != "stdin") {
        std::dynamic_pointer_cast<ofstream>(this->out)->close();
    }
    if (Logger::instance.get() == this) {
        Logger::instance = nullptr;
    }
}
// 4 log mode with increasing severity
// TODO: implement me
static void info(std::string&);
static void debug(std::string&);
static void warn(std::string&);
static void error(std::string&);