#pragma once 

#include <chrono>

/**
* @brief class waiting for a certain amount of time.
* @details Emulates the "normal behavior" of the user when interacting with services. /
*          Used in testing to create delays in operation on both sides. /
*          It is a template designed to work with different types of time, for example: seconds, milliseconds... /
*          By default, the template works with milliseconds.
*/
template<typename TimeType = std::chrono::milliseconds>
class WaitForTime
{
public:
    explicit WaitForTime(TimeType timeForWait) :
        waitTime(timeForWait) {}

    WaitForTime(WaitForTime& other)                   = delete;
    WaitForTime& operator=(const WaitForTime& other)  = delete;
    WaitForTime(WaitForTime&& other) noexcept         = delete;
    WaitForTime const& operator=(WaitForTime&& other) = delete;

    ~WaitForTime() = default;

    void wait()
    {
        isSleep   = true;
        startTime = std::chrono::system_clock::now();

        while (isSleep)
        {
            auto currentTime = std::chrono::system_clock::now();
            auto elapsedTime = std::chrono::duration_cast<TimeType>(currentTime - startTime);
            if (elapsedTime.count() > waitTime.count())
            {
                isSleep = false;
            }
        }
    }

private:
    bool                                  isSleep;
    TimeType                              waitTime;
    std::chrono::system_clock::time_point startTime;
};
