#include <iostream>
#include <memory>
#include "pipeline.h"
#include "cache/cache.h"
#include "pipeline_unit.h"
#include "stall_policy.h"
#include"flush_policy.h"

namespace pipeline_simulator {

SimplePipeline::SimplePipeline() :
    // hazard_detection_unit
    hazard_dectection_unit(&pc_reg, &if_id_reg, &id_ex_reg),
    // branch_predictor
    branch_predictor(std::make_unique<SkippedMispredFlushCycle>(&clock)),
    // cache
    icache(0, std::make_unique<SkippedStallCycle>(&clock)),
    dcache(0, std::make_unique<SkippedStallCycle>(&clock)),
    l2cache(4),
    memory(10),
    // pipeline stage
    fetch(&icache, &branch_predictor),
    decode(&hazard_dectection_unit),
    execute(&hazard_dectection_unit, &branch_predictor, std::make_shared<SkippedStallCycle>(&clock)),
    mem(&dcache),
    // pipeline reg
    pc_reg(clock, nullptr, &fetch),
    if_id_reg(clock, &fetch, &decode),
    id_ex_reg(clock, &decode, &execute, &inst_pool),
    ex_mem_reg(clock, &execute, &mem),
    mem_wb_reg(clock, &mem, &wb),
    retire_reg(clock, &wb, &inst_pool) {
    icache.set_next_level_cache(&l2cache);
    dcache.set_next_level_cache(&l2cache);
    l2cache.set_next_level_cache(&memory);
}

void SimplePipeline::read_inst(Instruction *inst) {
    pc_reg.receive(inst);
    ++instruction_count;
    clock.tick();
    // Ensure the previous PC is consumed 
    // before reading the next instruction
    while(pc_reg.isStalled())
        clock.tick();
}

Instruction *SimplePipeline::create_inst() {
    return inst_pool.newInst();
}

void SimplePipeline::show_performance() const {
    std::cout << "Instruction Count: "
              << instruction_count
              << std::endl;

    auto cycle_count = clock.get_cycle_count();
    std::cout << "Cycle Count: " 
              << cycle_count
              << std::endl;

    auto cache_latency = 
        icache.get_latency_info().get_total_latency() + 
        dcache.get_latency_info().get_total_latency();

    std::cout << "Latency by cache: "
              << cache_latency
              << std::endl;

    auto mul_unit_latency = 
        execute.get_latency_info(ExecutionUnitType::MUL_UNIT)
               ->get_total_latency();
    auto div_unit_latency = 
        execute.get_latency_info(ExecutionUnitType::DIV_UNIT)
               ->get_total_latency();

    std::cout << "Latency by Multiplier Unit: "
            << mul_unit_latency
            << std::endl;

    std::cout << "Latency by Divider Unit: "
              << div_unit_latency
              << std::endl;

    auto load_use_hazard_latency =
        hazard_dectection_unit.get_latency_info()
                              .get_total_latency();

    std::cout << "Latency by Load-Use Hazard: "
              << load_use_hazard_latency
              << std::endl;

    std::cout << std::setprecision(4) << std::fixed;

    std::cout << "CPI: "
              << 100.0 * cycle_count / instruction_count
              << '%' << std::endl;

    std::cout << "Cache Latency in Total Cycles: "
              << 100.0 * cache_latency / cycle_count
              << '%' << std::endl;

    std::cout << "Multiplier Unit Latency in Total Cycles: "
              << 100.0 * mul_unit_latency / cycle_count
              << '%' << std::endl;

    std::cout << "Divider Unit Latency in Total Cycles: "
              << 100.0 * div_unit_latency / cycle_count
              << '%' << std::endl;
}

void SimplePipeline::show_cache_info() const {
    icache.show("L1 Icache");
    std::cout << std::endl << std::endl;
    dcache.show("L1 Dcache");
    std::cout << std::endl << std::endl;
    l2cache.show("L2 cache");
}

} // namespace pipeline_simulator