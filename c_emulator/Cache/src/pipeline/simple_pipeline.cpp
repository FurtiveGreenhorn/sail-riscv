#include <memory>
#include "simple_pipeline.h"

SimplePipeline::SimplePipeline() :
    // hazard_detection_unit
    hazard_dectection_unit(&pc_reg, &if_id_reg),
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
    pc_reg(clock, &noStage, &fetch),
    if_id_reg(clock, &fetch, &decode),
    id_ex_reg(clock, &decode, &execute),
    ex_mem_reg(clock, &execute, &mem),
    mem_wb_reg(clock, &mem, &wb) {}

void SimplePipeline::read_inst(Instruction *inst) {
    pc_reg.accept(inst);
    clock.tick();
    // Ensure the previous PC is consumed 
    // before reading the next instruction
    while(pc_reg.isStalled())
        clock.tick();
}