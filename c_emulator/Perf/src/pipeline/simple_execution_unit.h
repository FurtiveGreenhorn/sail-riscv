#pragma once

#include <memory>
#include <cmath>
#include "../Instruction/instruction.h"
#include "clock.h"
#include "stall_policy.h"
#include "latency_log.h"

namespace pipeline_simulator {

class ExecutionUnitBase {
public:
    virtual void execute(Instruction *inst) = 0;
};

template <typename Derived>
class SimpleExecutionUnitMixin : public ExecutionUnitBase {
public:
    SimpleExecutionUnitMixin(std::shared_ptr<SkippedStallCycle> stall_policy) : 
        stall_policy(stall_policy) {}

    virtual void execute(Instruction *inst) override {
        auto latency = static_cast<Derived *>(this)->process(inst);
        stall_policy->stall(latency);
        latency_info.update(latency);
    }

    const LatencyInfo& get_latency_info() const {
        return latency_info;
    }

private:
    std::shared_ptr<SkippedStallCycle> stall_policy;
    LatencyInfo latency_info;
};

class IntegerALU : public SimpleExecutionUnitMixin<IntegerALU> {
public:
    IntegerALU(std::shared_ptr<SkippedStallCycle> stall_policy) : 
        SimpleExecutionUnitMixin(stall_policy) {}

    Clock::Cycles process(Instruction *inst) {
        return 0;
    }
};

class MultiplierUnit : public SimpleExecutionUnitMixin<MultiplierUnit> {
public:
    MultiplierUnit(std::shared_ptr<SkippedStallCycle> stall_policy) : 
        SimpleExecutionUnitMixin(stall_policy) {}

    Clock::Cycles process(Instruction *inst) {
        // std::cout << "MultiplierUnit process" << std::endl;
        return 3;
    }
};

class DividerUnit : public SimpleExecutionUnitMixin<DividerUnit> {
public:
    DividerUnit(std::shared_ptr<SkippedStallCycle> stall_policy) : 
        SimpleExecutionUnitMixin(stall_policy) {}

    // reference: https://www.scs.stanford.edu/~zyedidia/docs/sifive/sifive-u74mc.pdf (page 37)
    Clock::Cycles process(Instruction *inst);
};

} // namespace pipeline_simulator