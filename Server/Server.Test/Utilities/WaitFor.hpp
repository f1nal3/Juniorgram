#pragma once 

#include <chrono>

template<typename TTime = std::chrono::milliseconds>
class WaitFor
{
public:
    WaitFor(TTime timeForWait) :
        waitTime(timeForWait) {}

    ~WaitFor() { waitTime = TTime(0); }

    void wait()
    {
        bool sleep = true;
        while (sleep)
        {
            auto now     = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<TTime>(now - start);
            if (elapsed.count() > waitTime.count())
            {
                sleep = false;
            }
        }
    }

private:
    TTime waitTime;
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
};
