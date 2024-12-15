#include "simple_pipeline_unit.h"
#include <memory>

class SimplePipeline {
public:
    SimplePipeline();
    void read_inst(Instruction *inst);
    void clock_tick();

private:
    Clock clock;
    unsigned cycle_counter;
    HazardDetectionUnit hazard_dectection_unit;
    std::unique_ptr<Cache> icache, dcache, l2cache;
    // pipeline stage
    WB wb;
    Mem mem;
    Execute execute;
    Decode decode;
    Fetch fetch;
    NoStage noStage;
    // pipeline reg
    // Note !!! Need to follow dependency
    // (transmit before accept)
    MemWbReg mem_wb_reg;
    ExMemReg ex_mem_reg;
    IdExReg id_ex_reg;
    IfIdReg if_id_reg;
    PCReg pc_reg;
};