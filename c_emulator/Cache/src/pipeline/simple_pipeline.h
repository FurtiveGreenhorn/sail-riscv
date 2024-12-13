#include "simple_pipeline_unit.h"
#include <memory>

class SimplePipeline {
public:
    SimplePipeline();
private:
    std::unique_ptr<Cache> icache, dcache, l2cache;
    std::unique_ptr<Fetch> fetch;
    std::unique_ptr<Decode> decode;
    std::unique_ptr<Execute> execute;
    std::unique_ptr<Mem> mem;
    std::unique_ptr<WB> wb;
    IfIdReg if_id_reg;
    IdExReg id_ex_reg;
    ExMemReg ex_mem_reg;
    MemWbReg mem_wb_reg;
};