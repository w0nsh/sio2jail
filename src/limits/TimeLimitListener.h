#pragma once

#include "executor/ExecuteEventListener.h"
#include "printer/OutputSource.h"

#include <chrono>

namespace s2j {
namespace limits {

class TimeLimitListener
        : public executor::ExecuteEventListener
        , public printer::OutputSource {
public:
    TimeLimitListener(
            uint64_t rTimelimitUs,
            uint64_t uTimelimitUs,
            uint64_t sTimelimitUs,
            uint64_t usTimelimitUs);
    ~TimeLimitListener();

    void onPostForkParent(pid_t childPid) override;
    executor::ExecuteAction onSigalrmSignal() override;
    void onPostExecute() override;

private:
    struct ProcessTimeUsage {
        uint64_t uTimeUs; // user time in [us]
        uint64_t sTimeUs; // system time in [us]
    };

    struct TimeUsage {
        uint64_t realTimeUs;
        ProcessTimeUsage processTimeUs;
    };

    static const uint64_t TIMER_TICKING_INTERVAL_US;
    static const long CLOCK_TICKS_PER_SECOND;

    executor::ExecuteAction verifyTimeUsage(std::unique_ptr<TimeUsage>);
    uint64_t getRealTimeUsage() const;
    ProcessTimeUsage getProcessTimeUsage() const;
    std::unique_ptr<TimeUsage> getTimeUsage() const;

    uint64_t rTimelimitUs_; // real time limit in [us]
    uint64_t uTimelimitUs_; // user time limit in [us]
    uint64_t sTimelimitUs_; // system time limit in [us]
    uint64_t usTimelimitUs_; // user+system time limit in [us]
    pid_t childPid_{};

    std::chrono::steady_clock::time_point startRealTime_;
    bool isTimerCreated_;
    timer_t timerId_{};
};

} // namespace limits
} // namespace s2j
