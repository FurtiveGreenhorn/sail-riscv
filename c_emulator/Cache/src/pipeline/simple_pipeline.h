#include "simple_pipeline_unit.h"
#include <memory>

class SimplePipeline {
public:
    SimplePipeline();
    void read_inst(Instruction *inst);
    void clock_tick();

private:
    Clock clock;
    HazardDetectionUnit hazard_dectection_unit;
    std::unique_ptr<Cache> icache, dcache, l2cache;
    // pipeline stage
    NoStage noStage;
    Fetch fetch;
    Decode decode;
    Execute execute;
    Mem mem;
    WB wb;
    // pipeline reg
    PCReg pc_reg;
    IfIdReg if_id_reg;
    IdExReg id_ex_reg;
    MemWbReg mem_wb_reg;
    ExMemReg ex_mem_reg;
};