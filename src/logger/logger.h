
#include <fstream>
#include <memory>
#include <string>
// simple logger with single instance model
enum class LoggerMode { APPEND, OVERRIDE };
class Logger {
   private:
    std::string filename;
    std::shared_ptr<std::ostream> out;
    LoggerMode mode;
    Logger(std::string&, LoggerMode);
    ~Logger();

    static std::shared_ptr<Logger> instance;
    static void write(std::string&);

   public:
    static void init(std::string&, LoggerMode);
    static std::shared_ptr<Logger> get_instance();
    // 4 log mode with increasing severity
    static void info(std::string&);
    static void debug(std::string&);
    static void warn(std::string&);
    static void error(std::string&);
};