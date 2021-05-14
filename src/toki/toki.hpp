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
    uint64_t get_timeval() {
        return timeval;
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
    uint64_t _last_time;

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
    Toki(uint32_t time_scale = 1) : _time_scale{time_scale}, _timer_thread(Toki::tick_loop, this), _last_time{0} {
    }
    // start ticking
    void run() {
        Logger::info("starting timer thread");
        _timer_thread.detach();
    }
    std::string get_format_string() {
        return _time.to_string();
    }
    void clear_delta_time() {
        _last_time = this->_time.get_timeval();
    }
    uint64_t delta_time() {
        uint32_t dt = this->_time.get_timeval() - _last_time;
        _last_time = this->_time.get_timeval();
        return dt;
    }
};

#endif