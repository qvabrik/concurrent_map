#pragma once

#include <chrono>
#include <iostream>
#include <string_view>

#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)
#define LOG_DURATION_STREAM(x, os) LogDuration UNIQUE_VAR_NAME_PROFILE(x, os)

class LogDuration {
    using Clock = std::chrono::steady_clock;
public:
    LogDuration(const std::string& id);
    LogDuration(const std::string& id, std::ostream& os);

    ~LogDuration();

private:
    std::ostream& os_ = std::cerr;
    const std::string id_;
    const Clock::time_point start_time_ = Clock::now();
};
