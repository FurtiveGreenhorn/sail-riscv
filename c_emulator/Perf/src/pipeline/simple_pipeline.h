#pragma once

#include "simple_pipeline_unit.h"
#include <memory>

class SimplePipeline {
public:
    SimplePipeline();
    void read_inst(Instruction *inst);
    Instruction *create_inst();
    void show_cycle_count();
    void show_cache_info();

private:
    Instruction_pool<INST_POOL_SIZE> inst_pool;
    Clock clock;
    HazardDetectionUnit hazard_dectection_unit;
    std::unique_ptr<CacheConcept> icache, dcache, l2cache, memory;
    // pipeline stage
    Fetch fetch;
    Decode decode;
    Execute execute;
    Mem mem;
    Wb wb;
    // pipeline reg
    PCReg pc_reg;
    IfIdReg if_id_reg;
    IdExReg id_ex_reg;
    ExMemReg ex_mem_reg;
    MemWbReg mem_wb_reg;
    RetireReg retire_reg;
};