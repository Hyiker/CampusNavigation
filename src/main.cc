#include <cstdio>
#include "logger/logger.h"

int main() {
    Logger::init("stdout", FileMode::APPEND, LogLevel::INFO);
    std::string info = "Hello World";
    Logger::info(info);
}