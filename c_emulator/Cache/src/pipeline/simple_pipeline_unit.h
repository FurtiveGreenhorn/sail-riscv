#include "../Instruction/instruction_pool.h"
#include "../Instruction/instruction.h"
#include "simple_pipeline_unit_base.h"

class Cache {};

class Fetch : public SimplePipelineStageLogicMixIn<Fetch> {
public:
    Fetch(Cache *ic) : icache(ic) {}
    void process_stage() {
        // ToDo: access cache and send signal to control unit if missed
    }
private:
    Cache *icache;
};
class Decode : public SimplePipelineStageLogicMixIn<Decode> {
public:
    RegNum rs1_flow_out() {
        return data->rs1;
    }
    RegNum rs2_flow_out() {
        return data->rs2;
    }
};
class Execute: public SimplePipelineStageLogicMixIn<Execute> {
public:
    bool mem_read() {
        return data->is_load();
    }
    RegNum rd_flow_out() {
        return data->rs1;
    }
};
class Mem : public SimplePipelineStageLogicMixIn<Mem> {
public:
    Mem(Cache *dc) : dcache(dc) {}
    void process_stage() {
        // ToDo: access cache and send signal to control unit if missed
    }
private:
    Cache *dcache;
};

class WB : public SimplePipelineStageLogicMixIn<WB> {};

// #define PIPELINE_REG_CLASS(RegClass, PrevStage, NextStage)           \
// class RegClass : public SimplePipelineRegMixin<RegClass, PrevStage, NextStage> {  \
// public:                                                               \
//     RegClass(PrevStage* prev, NextStage* next)                        \
//         : SimplePipelineRegMixin<RegClass, PrevStage, NextStage>(prev, next) {} \
// };

// PIPELINE_REG_CLASS(IfIdReg, Fetch, Decode)
// PIPELINE_REG_CLASS(IdExReg, Decode, Execute)
// PIPELINE_REG_CLASS(ExMemReg, Execute, Mem)
// PIPELINE_REG_CLASS(MemWbReg, Mem, WB)

class IfIdReg : SimplePipelineRegMixin<IfIdReg, Fetch, Decode> {
public:
    IfIdReg(Fetch *fetch, Decode *decode)
        : SimplePipelineRegMixin(fetch, decode) {}
};
class IdExReg : SimplePipelineRegMixin<IdExReg, Decode, Execute> {
public:
    IdExReg(Decode *decode, Execute *execute)
        : SimplePipelineRegMixin(decode, execute) {}
};
class ExMemReg : SimplePipelineRegMixin<ExMemReg, Execute, Mem> {
public:
    ExMemReg(Execute *execute, Mem *mem)
        : SimplePipelineRegMixin(execute, mem) {}
};
class MemWbReg : SimplePipelineRegMixin<MemWbReg, Mem, WB> {
public:
    MemWbReg(Mem *mem, WB *wb)
        : SimplePipelineRegMixin(mem, wb) {}
};