#include <memory>
#include "simple_pipeline.h"

SimplePipeline::SimplePipeline() :
    // cache
    icache(std::make_unique<Cache>()),
    dcache(std::make_unique<Cache>()),
    l2cache(std::make_unique<Cache>()),
    // stage
    fetch(std::make_unique<Fetch>(icache.get())),
    decode(std::make_unique<Decode>()),
    execute(std::make_unique<Execute>()),
    mem(std::make_unique<Mem>(dcache.get())), 
    wb(std::make_unique<WB>()), 
    // pipeline reg
    if_id_reg(fetch.get(), decode.get()),
    id_ex_reg(decode.get(), execute.get()),
    ex_mem_reg(execute.get(), mem.get()),
    mem_wb_reg(mem.get(), wb.get()) {}