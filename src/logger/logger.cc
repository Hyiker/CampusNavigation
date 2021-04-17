#include "logger.h"
#include <ctime>
#include <iomanip>
#include <iostream>

using std::string, std::make_shared, std::shared_ptr, std::ios, std::ostream, std::make_unique, std::ofstream;
Logger* Logger::instance = nullptr;
Logger::Logger(string& filename, FileMode mode, LogLevel log_level = LogLevel::DEBUG)
    : filename{filename}, mode{mode}, log_level{log_level} {
    auto open_mode = ios::app;
    if (mode == FileMode::APPEND) {
        open_mode = ios::app;
    } else if (mode == FileMode::OVERRIDE) {
        open_mode = ios::out;
    }
    if (filename == "stderr") {
        shared_ptr<ostream> tmp(&std::cerr);
        this->out = tmp;
    } else if (filename == "stdout") {
        shared_ptr<ostream> tmp(&std::cout);
        this->out = tmp;
    } else {
        this->out = make_shared<ofstream>(filename, open_mode);
    }
}

void Logger::init(string filename, FileMode mode, LogLevel level) {
    if (Logger::instance == nullptr) {
        Logger::instance = new Logger(filename, mode, level);
    }
}

Logger* Logger::get_instance() {
    return Logger::instance;
}
Logger::~Logger() {
    if (filename != "stderr" && filename != "stdout") {
        auto out = std::dynamic_pointer_cast<ofstream>(this->out);
        if (out && out->is_open()) {
            out->close();
        }
    }else{
        this->out = nullptr;
    }
    if (Logger::instance == this) {
        Logger::instance = nullptr;
    }
}
void Logger::write_to_stream(const string& message) {
    auto stream = this->out.get();
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    string level_str;
    switch (this->log_level) {
        case LogLevel::INFO:
            level_str = "INFO";
            break;
        case LogLevel::DEBUG:
            level_str = "DEBUG";
            break;
        case LogLevel::WARN:
            level_str = "WARN";
            break;
        case LogLevel::ERROR:
            level_str = "ERROR";
            break;
        default:
            // TODO err
            break;
    }
    (*stream) << '(' << level_str << ")[" << std::put_time(&tm, "%c %Z") << "] " << message << '\n';
}
inline static uint8_t log_level_to_int(LogLevel level) {
    if (level == LogLevel::INFO) {
        return 0;
    } else if (level == LogLevel::DEBUG) {
        return 1;
    } else if (level == LogLevel::WARN) {
        return 2;
    } else if (level == LogLevel::ERROR) {
        return 3;
    }
    return UINT8_MAX;
}
inline static bool can_output(LogLevel instance_level, LogLevel msg_level) {
    return log_level_to_int(msg_level) >= log_level_to_int(instance_level);
}

// 4 log mode with increasing severity
void Logger::info(const string& msg) {
    auto instance = Logger::get_instance();
    if (can_output(instance->log_level, LogLevel::INFO)) {
        Logger::get_instance()->write_to_stream(msg);
    }
}
void Logger::debug(const string& msg) {
    auto instance = Logger::get_instance();
    if (can_output(instance->log_level, LogLevel::DEBUG)) {
        Logger::get_instance()->write_to_stream(msg);
    }
}
void Logger::warn(const string& msg) {
    auto instance = Logger::get_instance();
    if (can_output(instance->log_level, LogLevel::WARN)) {
        Logger::get_instance()->write_to_stream(msg);
    }
}
void Logger::error(const string& msg) {
    auto instance = Logger::get_instance();
    if (can_output(instance->log_level, LogLevel::ERROR)) {
        Logger::get_instance()->write_to_stream(msg);
    }
}