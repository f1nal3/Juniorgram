#pragma once 

#include <chrono>

/**
* @brief class waiting for a certain amount of time.
* @details Emulates the "normal behavior" of client-server interaction. /
*          Used in testing to temporarily process actions on the client/server side. /
*          It is a template designed to work with different types of time, for example: seconds, milliseconds... /
*          By default, the template works with milliseconds.
*/
template<typename TimeType = std::chrono::milliseconds>
class WaitFor
{
public:
    explicit WaitFor(TimeType timeForWait) :
        waitTime(timeForWait) {}

    WaitFor(WaitFor& other)                   = delete;
    WaitFor& operator=(const WaitFor& other)  = delete;
    WaitFor(WaitFor&& other) noexcept         = delete;
    WaitFor const& operator=(WaitFor&& other) = delete;

    ~WaitFor() { waitTime = TimeType(0); }

    void wait()
    {
        bool IsSleep = true;
        while (IsSleep)
        {
            auto currentTime = std::chrono::system_clock::now();
            auto elapsedTime = std::chrono::duration_cast<TimeType>(currentTime - startTime);
            if (elapsedTime.count() > waitTime.count())
            {
                IsSleep = false;
            }
        }
    }

private:
    TimeType                              waitTime;
    std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
};
