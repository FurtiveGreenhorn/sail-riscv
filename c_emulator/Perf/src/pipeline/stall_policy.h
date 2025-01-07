#pragma once

#include "clock.h"

// Skip simulation of stall cycles to save simulation time.
// This is feasible because in a simple 5-stage pipeline,
// skipping stall cycles does not affect the cycle count result.
class SkippedStallCycle {
public:
    SkippedStallCycle(Clock *clock) : clock(clock) {}
    void stall(unsigned cycles) {
        clock->skip_cycle(cycles);
    }
private:
    Clock *clock;
};