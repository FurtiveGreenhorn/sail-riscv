#include <memory>
#include "simple_pipeline.h"
#include "simple_cache/cache.h"
#include "simple_pipeline_unit.h"

SimplePipeline::SimplePipeline() :
    // hazard_detection_unit
    hazard_dectection_unit(&pc_reg, &if_id_reg, &id_ex_reg),
    // cache
    icache(std::make_unique<Cache<
             CacheParams<128, 2, 64>, SkippedStallCycle>>(0,
           std::make_unique<SkippedStallCycle>(&clock))),
    dcache(std::make_unique<Cache<
             CacheParams<64, 4, 64>, SkippedStallCycle>>(0,
           std::make_unique<SkippedStallCycle>(&clock))),
    l2cache(std::make_unique<Cache<CacheParams<128, 2, 64>>>(4)),
    memory(std::make_unique<Memory>()),
    // pipeline stage
    fetch(icache.get()),
    decode(&hazard_dectection_unit),
    execute(&hazard_dectection_unit),
    mem(dcache.get()),
    // pipeline reg
    pc_reg(clock, nullptr, &fetch),
    if_id_reg(clock, &fetch, &decode),
    id_ex_reg(clock, &decode, &execute, &inst_pool),
    ex_mem_reg(clock, &execute, &mem),
    mem_wb_reg(clock, &mem, &wb),
    retire_reg(clock, &wb, &inst_pool) {
    icache->set_next_level_cache(l2cache.get());
    dcache->set_next_level_cache(l2cache.get());
    l2cache->set_next_level_cache(memory.get());
}

void SimplePipeline::read_inst(Instruction *inst) {
    pc_reg.receive(inst);
    clock.tick();
    // Ensure the previous PC is consumed 
    // before reading the next instruction
    while(pc_reg.isStalled())
        clock.tick();
}

Instruction *SimplePipeline::create_inst() {
    return inst_pool.newInst();
}

void SimplePipeline::show_cycle_count() {
    std::cout << "Cycle Count: " 
              << clock.get_cycle_count()
              << std::endl;
}

void SimplePipeline::show_cache_info() {
    icache->show("L1 Icache");
    std::cout << std::endl << std::endl;
    dcache->show("L1 Dcache");
    std::cout << std::endl << std::endl;
    l2cache->show("L2 cache");
}