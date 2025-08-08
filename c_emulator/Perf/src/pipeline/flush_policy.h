#pragma once

#include "clock.h"

namespace pipeline_simulator {

// Skip simulation of stall cycles to save simulation time.
// This is feasible because in a simple 5-stage pipeline,
// skipping stall cycles does not affect the cycle count result.
class SkippedMispredFlushCycle {
public:
    SkippedMispredFlushCycle(Clock *clock) : clock(clock) {}
    void flush(unsigned cycles = 2) {
        clock->skip_cycle(cycles);
    }
private:
    Clock *clock;
};

} // namespace pipeline_simulator