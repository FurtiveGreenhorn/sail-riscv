#include "simple_pipeline_unit.h"
#include <memory>

class SimplePipeline {
public:
    SimplePipeline();
    void read_inst(Instruction *inst);
    void clock_tick();

private:
    HazardDetectionUnit hazard_dectection_unit;
    std::unique_ptr<Cache> icache, dcache, l2cache;
    // pipeline stage
    std::unique_ptr<Fetch> fetch;
    std::unique_ptr<Decode> decode;
    std::unique_ptr<Execute> execute;
    std::unique_ptr<Mem> mem;
    std::unique_ptr<WB> wb;
    // pipeline reg
    IfIdReg if_id_reg;
    IdExReg id_ex_reg;
    ExMemReg ex_mem_reg;
    MemWbReg mem_wb_reg;
};