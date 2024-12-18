#include <memory>
#include "simple_pipeline.h"

SimplePipeline::SimplePipeline() :
    // hazard_detection_unit
    hazard_dectection_unit(&pc_reg, &if_id_reg, &id_ex_reg),
    // cache
    icache(std::make_unique<Cache>()),
    dcache(std::make_unique<Cache>()),
    l2cache(std::make_unique<Cache>()),
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
    retire_reg(clock, &wb, &inst_pool) {}

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