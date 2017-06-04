#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdarg.h>

#include <iostream>
#include <mutex>
#include <thread>
#include <sstream>


#ifdef _DEBUG
#define LOG(...) Logger::log(__FUNCTION__, ##__VA_ARGS__);
#else
#define LOG(...)
#endif

class Logger
{
private:
    static std::mutex printMutex_;

public:
    static void log(const std::string &msg)
    {
        std::unique_lock<std::mutex> lock(Logger::printMutex_);
        std::cerr << "[tid: " << std::this_thread::get_id() << "] " << msg
                  << std::endl;
    }

    static void log(const std::string func, const std::string &msg)
    {
        std::unique_lock<std::mutex> lock(Logger::printMutex_);
        std::cerr << "[" << func << " - tid: " << std::this_thread::get_id() << "] " << msg
                  << std::endl;
    }
};

#endif //_LOGGER_H
