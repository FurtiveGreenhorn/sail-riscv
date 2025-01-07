#pragma once

#include <memory>
#include <cmath>
#include "../Instruction/instruction.h"
#include "clock.h"
#include "stall_policy.h"

class ExecutionUnitBase {
public:
    virtual void execute(Instruction *inst) = 0;
};

template <typename Derived>
class SimpleExecutionUnitMixin : public ExecutionUnitBase {
public:
    using Cycles = unsigned;
    SimpleExecutionUnitMixin(std::shared_ptr<SkippedStallCycle> stall_policy) : 
        stall_policy(stall_policy) {}

    virtual void execute(Instruction *inst) override {
        stall_policy->stall(static_cast<Derived *>(this)->process(inst));
    }

private:
    std::shared_ptr<SkippedStallCycle> stall_policy;
};

class IntegerALU : public SimpleExecutionUnitMixin<IntegerALU> {
public:
    IntegerALU(std::shared_ptr<SkippedStallCycle> stall_policy) : 
        SimpleExecutionUnitMixin(stall_policy) {}

    Cycles process(Instruction *inst) {
        return 0;
    }
};

class MultiplierUnit : public SimpleExecutionUnitMixin<MultiplierUnit> {
public:
    MultiplierUnit(std::shared_ptr<SkippedStallCycle> stall_policy) : 
        SimpleExecutionUnitMixin(stall_policy) {}

    Cycles process(Instruction *inst) {
        // std::cout << "MultiplierUnit process" << std::endl;
        return 3;
    }
};

class DividerUnit : public SimpleExecutionUnitMixin<DividerUnit> {
public:
    DividerUnit(std::shared_ptr<SkippedStallCycle> stall_policy) : 
        SimpleExecutionUnitMixin(stall_policy) {}

    // reference: https://www.scs.stanford.edu/~zyedidia/docs/sifive/sifive-u74mc.pdf (page 37)
    Cycles process(Instruction *inst) {
        // std::cout << "DividerUnit process" << std::endl;
        bool is_signed = inst->type == INST_div || 
                         inst->type == INST_divw ||
                         inst->type == INST_rem ||
                         inst->type == INST_remw;
        Cycles latency = 2;

        if (is_signed) {
            auto dividend = int32_t(inst->rs1_val);
            auto divisor = int32_t(inst->rs2_val);

            latency += (std::log2(std::abs(dividend)) - 
                       std::log2(std::abs(divisor)));
            if ((dividend | divisor) < 0)
                ++latency;
            if ((dividend ^ divisor) < 0)
                ++latency;
            return latency;
        }
        else {
            auto dividend = uint32_t(inst->rs1_val);
            auto divisor = uint32_t(inst->rs2_val);

            latency += log2(dividend) - log2(divisor);
        }

        return latency;
    }
};