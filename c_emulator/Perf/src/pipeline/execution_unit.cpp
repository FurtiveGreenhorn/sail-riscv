#include "execution_unit.h"
#include "clock.h"

namespace pipeline_simulator {

Clock::Cycles DividerUnit::process(Instruction *inst) {
    // std::cout << "DividerUnit process" << std::endl;
    bool is_signed = inst->type == INST_div || 
                     inst->type == INST_divw ||
                     inst->type == INST_rem ||
                     inst->type == INST_remw;
    Clock::Cycles latency = 2;

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

} // namespace pipeline_simulator