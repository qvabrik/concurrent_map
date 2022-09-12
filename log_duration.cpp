#include <chrono>
#include <iostream>

#include "log_duration.h"

LogDuration::LogDuration(const std::string& id)
    : id_(id) 
{
}

LogDuration::LogDuration(const std::string& id, std::ostream& os)
    : id_(id), os_(os)
{
}

LogDuration::~LogDuration() {
    using namespace std::chrono;
    using namespace std::literals;

    const auto end_time = Clock::now();
    const auto dur = end_time - start_time_;

    if (&os_ == &std::cout) {
        os_ << "Operation time: "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }
    else {
        os_ << id_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }
}