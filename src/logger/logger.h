#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <fstream>
#include <memory>
#include <string>
// simple logger with single instance model
enum class FileMode { APPEND, OVERRIDE };
enum class LogLevel { INFO, DEBUG, WARN, ERROR };
class Logger {
   private:
    std::string filename;
    std::shared_ptr<std::ostream> out;
    FileMode mode;
    LogLevel log_level;
    static Logger* instance;

    void write_to_stream(const std::string&, LogLevel);

   public:
    // filename could be a path, stdout or stderr
    Logger(std::string&, FileMode, LogLevel);
    static void init(std::string, FileMode, LogLevel);
    static Logger* get_instance();
    // 4 log mode with increasing severity
    static void info(const std::string&);
    static void debug(const std::string&);
    static void warn(const std::string&);
    static void error(const std::string&);
    ~Logger();
};

#endif  // !LOGGER_LOGGER_H