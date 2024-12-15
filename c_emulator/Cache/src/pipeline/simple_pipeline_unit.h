#include "../Instruction/instruction_pool.h"
#include "../Instruction/instruction.h"
#include "simple_pipeline_unit_concept.h"

class Cache {};

class Fetch;
class PCReg;
class IfIdReg;

class HazardDetectionUnit {
public:
    HazardDetectionUnit(PCReg *pc_reg, IfIdReg *ifid) 
        : pc_reg(pc_reg), if_id_reg(ifid) {}
    void active_load_use_hazard_detect(RegNum rd);
    void load_use_hazard_detect(RegNum IfIdRegRs1, RegNum IfIdRegRs2);
    
private:
    bool IdExMemRead = false;
    RegNum IdExRegRd;
    PCReg *pc_reg;
    IfIdReg *if_id_reg;

    void send_stall_for_load_use_hazard();
};

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
    Decode(HazardDetectionUnit *hdu) : hazard_detection_unit(hdu) {}
    void process_stage() {
        hazard_detection_unit->
            load_use_hazard_detect(data->rs1, data->rs2);
    }
private:
    HazardDetectionUnit *hazard_detection_unit;
};
class Execute: public SimplePipelineStageLogicMixIn<Execute> {
public:
    Execute(HazardDetectionUnit *hdu) : hazard_detection_unit(hdu) {}
    void process_stage() {
        if (data->is_load())
            hazard_detection_unit->
                active_load_use_hazard_detect(data->rd);
    }
private:
    HazardDetectionUnit *hazard_detection_unit;
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
//     RegClass(Clock &clk, PrevStage* prev, NextStage* next)                        \
//         : SimplePipelineRegMixin(clk, prev, next) {} \
// };

// PIPELINE_REG_CLASS(IfIdReg, Fetch, Decode)
// PIPELINE_REG_CLASS(IdExReg, Decode, Execute)
// PIPELINE_REG_CLASS(ExMemReg, Execute, Mem)
// PIPELINE_REG_CLASS(MemWbReg, Mem, WB)

class PCReg : public SimplePipelineRegMixin<PCReg, NoStage, Fetch> {
public:
    PCReg(Clock &clk, NoStage *noStage, Fetch *fetch)
        : SimplePipelineRegMixin(clk, noStage, fetch) {}
    // Check if the stage is stalled
    // This method is designed to synchronize with the Sail frontend clock.
    // When the Sail simulator ticks, we must ensure that the current stage is not stalled,
    // otherwise, we need to keep clock ticking until the stall state is cleared before processing instructions.
    bool isStalled() const {
        return stallFlag;
    }
};
class IfIdReg : public SimplePipelineRegMixin<IfIdReg, Fetch, Decode> {
public:
    IfIdReg(Clock &clk, Fetch *fetch, Decode *decode)
        : SimplePipelineRegMixin(clk, fetch, decode) {}
};
class IdExReg : public SimplePipelineRegMixin<IdExReg, Decode, Execute> {
public:
    IdExReg(Clock &clk, Decode *decode, Execute *execute)
        : SimplePipelineRegMixin(clk, decode, execute) {}
};
class ExMemReg : public SimplePipelineRegMixin<ExMemReg, Execute, Mem> {
public:
    ExMemReg(Clock &clk, Execute *execute, Mem *mem)
        : SimplePipelineRegMixin(clk, execute, mem) {}
};
class MemWbReg : public SimplePipelineRegMixin<MemWbReg, Mem, WB> {
public:
    MemWbReg(Clock &clk, Mem *mem, WB *wb)
        : SimplePipelineRegMixin(clk, mem, wb) {}
};

// HazardDetectionUnit function implementation
inline void HazardDetectionUnit::send_stall_for_load_use_hazard() {
    pc_reg->receive_stall();
}

inline void HazardDetectionUnit::active_load_use_hazard_detect(RegNum rd) {
    IdExMemRead = true;
    IdExRegRd = rd;
}
inline void HazardDetectionUnit::load_use_hazard_detect(RegNum IfIdRegRs1, RegNum IfIdRegRs2) {
    if (IdExRegRd != IfIdRegRs1 && IdExRegRd != IfIdRegRs2)
        return;
    IdExMemRead = false;
    send_stall_for_load_use_hazard();
}