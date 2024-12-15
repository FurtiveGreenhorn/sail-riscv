#include <memory>
#include "simple_pipeline.h"

SimplePipeline::SimplePipeline() :
    cycle_counter(0),
    // hazard_detection_unit
    hazard_dectection_unit(&pc_reg, &if_id_reg),
    // cache
    icache(std::make_unique<Cache>()),
    dcache(std::make_unique<Cache>()),
    l2cache(std::make_unique<Cache>()),
    // pipeline stage
    mem(dcache.get()),
    execute(&hazard_dectection_unit),
    decode(&hazard_dectection_unit),
    fetch(icache.get()),
    // pipeline reg
    mem_wb_reg(clock, &mem, &wb),
    ex_mem_reg(clock, &execute, &mem),
    id_ex_reg(clock, &decode, &execute),
    if_id_reg(clock, &fetch, &decode),
    pc_reg(clock, &noStage, &fetch) {}

void SimplePipeline::read_inst(Instruction *inst) {
    while(pc_reg.isStalled()) {
        clock.tick();
    }
}

void SimplePipeline::clock_tick() {
    clock.tick();
    ++cycle_counter;
}