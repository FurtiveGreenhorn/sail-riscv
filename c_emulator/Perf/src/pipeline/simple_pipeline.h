#pragma once

#include "simple_cache/cache.h"
#include "simple_cache/replacement_policy/Tree-PLRU.h"
#include "simple_pipeline_unit.h"
#include <memory>

namespace pipeline_simulator {

using IcacheT = L1Cache<CacheParams<128, 2, 64>>;
using DcacheT = L1Cache<CacheParams<64, 4, 64>>;
using L2cacheT = Cache<CacheParams<256, 8, 64>>;

class SimplePipeline {
    unsigned instruction_count = 0;
public:
    SimplePipeline();
    void read_inst(Instruction *inst);
    Instruction *create_inst();
    void show_performance() const;
    void show_cache_info() const;

private:
    Instruction_pool<INST_POOL_SIZE> inst_pool;
    Clock clock;
    HazardDetectionUnit hazard_dectection_unit;
    IcacheT icache;
    DcacheT dcache;
    L2cacheT l2cache;
    Memory memory;
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

} // namespace pipeline_simulator