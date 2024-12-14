#include <memory>
#include "simple_pipeline.h"

SimplePipeline::SimplePipeline() :
    // cache
    icache(std::make_unique<Cache>()),
    dcache(std::make_unique<Cache>()),
    l2cache(std::make_unique<Cache>()),
    // pipeline stage
    fetch(std::make_unique<Fetch>(icache.get())),
    decode(std::make_unique<Decode>(&hazard_dectection_unit)),
    execute(std::make_unique<Execute>(&hazard_dectection_unit)),
    mem(std::make_unique<Mem>(dcache.get())), 
    wb(std::make_unique<WB>()), 
    // pipeline reg
    if_id_reg(fetch.get(), decode.get()),
    id_ex_reg(decode.get(), execute.get()),
    ex_mem_reg(execute.get(), mem.get()),
    mem_wb_reg(mem.get(), wb.get()) {}

void SimplePipeline::read_inst(Instruction *inst) {
    fetch->recieve(inst);
    // ...
}

void SimplePipeline::clock_tick() {
    mem_wb_reg.clockTicked();
    ex_mem_reg.clockTicked();
    id_ex_reg.clockTicked();
    if_id_reg.clockTicked();
}