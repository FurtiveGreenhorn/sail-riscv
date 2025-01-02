#include <memory>
#include "simple_pipeline.h"
#include "simple_cache/cache.h"
#include "simple_pipeline_unit.h"

SimplePipeline::SimplePipeline() :
    // hazard_detection_unit
    hazard_dectection_unit(&pc_reg, &if_id_reg, &id_ex_reg),
    // cache
    icache(0, std::make_unique<SkippedStallCycle>(&clock)),
    dcache(0, std::make_unique<SkippedStallCycle>(&clock)),
    l2cache(4),
    memory(),
    // pipeline stage
    fetch(&icache),
    decode(&hazard_dectection_unit),
    execute(&hazard_dectection_unit),
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
    icache.show("L1 Icache");
    std::cout << std::endl << std::endl;
    dcache.show("L1 Dcache");
    std::cout << std::endl << std::endl;
    l2cache.show("L2 cache");
}