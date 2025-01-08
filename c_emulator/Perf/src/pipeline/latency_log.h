#pragma once
#include "clock.h"

namespace pipeline_simulator {

class LatencyInfo {
public:
    LatencyInfo() : latency_cycles_count(0) {}

    void update(unsigned cycles) {
        latency_cycles_count += cycles;
    }

    Clock::Cycles get_total_latency() const {
        return latency_cycles_count;
    }

private:
    Clock::Cycles latency_cycles_count;
};

} // namespace pipeline_simulator