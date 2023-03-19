#ifndef __COMMON_TIMER_H__
#define __COMMON_TIMER_H__

#include <execinfo.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>

class Timer
{
private:
    std::mutex m_mutex;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;

    void start() { m_startTime = std::chrono::high_resolution_clock::now(); }
    void stop() { m_endTime = std::chrono::high_resolution_clock::now(); }

    void backtrace(std::vector<std::string> &bt, int size, int skip)
    {
        void **array = (void**)malloc((sizeof(void*) * size));
        size_t s = ::backtrace(array, size);
        char **strings = backtrace_symbols(array, s);
        if (strings == NULL)
        {
            std::cout << "backtrace_symbols error" << std::endl;
            return;
        }

        for (size_t i = skip; i < s; i++)
        {
            bt.push_back(strings[i]);
        }

        free(strings);
        free(array);
    }

    std::string backtraceToString(int size, int skip)
    {
        std::vector<std::string> bt;
        backtrace(bt, size, skip);
        std::stringstream ss;
        for (size_t i = 0; i < bt.size(); i++)
        {
            ss << "\t" << bt[i] << std::endl;
        }
        return ss.str();
    }

public:
    Timer() { start(); }

    ~Timer() { 
        stop(); 
        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout << std::endl;
        std::cout << backtraceToString(1, 0);
        std::cout << "Time elapsed: " << getDuration() << " ms" << std::endl;
    }

    double getDuration() const { 
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - m_startTime).count(); 
    }
};

#endif