#ifndef TOKI_H
#define TOKI_H
#include <boost/format.hpp>
#include <cstdint>
#include <string>
#include <thread>
#include "logger/logger.h"
class SystemTime {
   private:
    uint64_t timeval;

   public:
    SystemTime(uint64_t tv = 0) : timeval{tv} {
    }
    void set_timeval(uint64_t tv) {
        this->timeval = tv;
    }
    void inc(uint64_t duration = 1) {
        timeval += duration;
    }
    std::string to_string() {
        uint32_t hours = timeval / 3600;
        uint32_t minutes = (timeval - hours * 3600) / 60;
        uint32_t seconds = (timeval - hours * 3600 - minutes * 60);
        return boost::str(boost::format("%02d:%02d:%02d") % hours % minutes % seconds);
    }
};

class Toki {
   private:
    uint32_t _time_scale;
    SystemTime _time;
    std::thread _timer_thread;

    void tick() {
        _time.inc();
    }

    static void tick_loop(Toki* tptr) {
        while (true) {
            tptr->tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / tptr->_time_scale));
        }
    }

   public:
    // how many times quicker the system is than the real world
    Toki(uint32_t time_scale = 1) : _time_scale{time_scale}, _timer_thread(Toki::tick_loop, this) {
    }
    // start ticking
    void run() {
        Logger::info("starting timer thread");
        _timer_thread.detach();
    }
    std::string get_format_string() {
        return _time.to_string();
    }
};

#endif