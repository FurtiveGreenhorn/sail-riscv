#pragma once

#include "../Instruction/instruction_pool.h"
#include "../Instruction/instruction.h"
#include "simple_execution_unit.h"
#include "simple_pipeline_unit_concept.h"
#include "simple_cache/cache.h"
#include "stall_policy.h"
#include <memory>

class Fetch;
class PCReg;
class IfIdReg;
class IdExReg;

class HazardDetectionUnit {
public:
    HazardDetectionUnit(PCReg *pc_reg, IfIdReg *ifid, IdExReg *idex) 
        : pc_reg(pc_reg), if_id_reg(ifid), id_ex_reg(idex) {}
    void receive_IdEx_rd(RegNum rd);
    void receive_IfId_rs(RegNum rs1, RegNum rs2);
    
private:
    RegNum IdExRegRd, IfIdRegRs1, IfIdRegRs2;
    PCReg *pc_reg;
    IfIdReg *if_id_reg;
    IdExReg *id_ex_reg;
    bool logged = false;

    void handle_load_use_hazard();
    void check_hazard();
};

class Fetch : public SimplePipelineStageLogicMixIn<Fetch> {
public:
    Fetch(CacheConcept *ic) : 
        icache(ic){
        name = "Fetch";
    }
    void process_stage() {
        icache->access(data->addr);
        if (logged)
            std::cout << "fetch addr: " << data->addr << std::endl;
    }
private:
    CacheConcept *icache;
};
class Decode : public SimplePipelineStageLogicMixIn<Decode> {
public:
    Decode(HazardDetectionUnit *hdu) : 
        hazard_detection_unit(hdu) {
        name = "Decode";
    }

    void process_stage() {
        // send rs
        if (data == nullptr)
            return;
        send_rs();
    }
private:
    HazardDetectionUnit *hazard_detection_unit;

    void send_rs() {
        if (data->is_load()) {
            hazard_detection_unit->
                receive_IfId_rs(data->rs1, data->rs2);
        }
    }
};

class Execute: public SimplePipelineStageLogicMixIn<Execute> {
public:
    Execute(HazardDetectionUnit *hdu, 
            std::shared_ptr<SkippedStallCycle> stall_policy) : 
                hazard_detection_unit(hdu),
                stall_policy(stall_policy), 
                int_alu(stall_policy), 
                mul_unit(stall_policy), 
                div_unit(stall_policy) {
        name = "Execute";
    }

    void process_stage() {
        if (data->is_load()) {
            hazard_detection_unit->
                receive_IdEx_rd(data->rd);
        }
        // a simple implementation of execution unit
        switch (data->get_execution_unit_type()) {
            case ExecutionUnitType::INT_ALU:
                int_alu.execute(data);
            break;
            case ExecutionUnitType::MUL_UNIT:
                mul_unit.execute(data);
            break;
            case ExecutionUnitType::DIV_UNIT:
                div_unit.execute(data);
            break;
            case ExecutionUnitType::NONE:
                // Handle NONE case if necessary
            break;
        }
    }
private:
    HazardDetectionUnit *hazard_detection_unit;
    std::shared_ptr<SkippedStallCycle> stall_policy;
    IntegerALU int_alu;
    MultiplierUnit mul_unit;
    DividerUnit div_unit;
};
class Mem : public SimplePipelineStageLogicMixIn<Mem> {
public:
    Mem(CacheConcept *dc) : dcache(dc) {
        name = "Mem";
    }

    void process_stage() {
        if (data->is_load()) {
            dcache->access(data->addr);
        }
        if (data->is_store()) {
            dcache->access(data->addr, true);
        }
    }
private:
    CacheConcept *dcache;
};

class Wb : public SimplePipelineStageLogicMixIn<Wb> {
public:
    Wb() {
        name = "Wb";
    }
};

class PCReg : public SimplePipelineRegMixin<PCReg, NoStage, Fetch> {
public:
    PCReg(Clock &clk, NoStage *noStage, Fetch *fetch)
        : SimplePipelineRegMixin(clk, noStage, fetch) {
        name = "PC";
    }
    void receive(Instruction *new_pc) {
        data = new_pc;
    }
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
        : SimplePipelineRegMixin(clk, fetch, decode) {
        name = "IF/ID";
    }
};
class IdExReg : public SimplePipelineRegMixin<IdExReg, Decode, Execute> {
public:
    IdExReg(Clock &clk, Decode *decode, Execute *execute, 
            Instruction_pool<INST_POOL_SIZE> *inst_pool)
        : SimplePipelineRegMixin(clk, decode, execute),
            inst_pool(inst_pool) {
        name = "ID/EX";
    }
    
    void flush() {
        assert(data != nullptr);
        flushFlag = true;
    }

    void after_accept() {
        if (logged)
            std::cout << name << " has been flushed !" << std::endl;
        if (flushFlag == false)
            return;
        data = inst_pool->getBubble();
        flushFlag = false;
    }
private:
    Instruction_pool<INST_POOL_SIZE> *inst_pool;
    bool flushFlag = false;
};
class ExMemReg : public SimplePipelineRegMixin<ExMemReg, Execute, Mem> {
public:
    ExMemReg(Clock &clk, Execute *execute, Mem *mem)
        : SimplePipelineRegMixin(clk, execute, mem) {
        name = "EX/MEM";
    }
};
class MemWbReg : public SimplePipelineRegMixin<MemWbReg, Mem, Wb> {
public:
    MemWbReg(Clock &clk, Mem *mem, Wb *wb)
        : SimplePipelineRegMixin(clk, mem, wb) {
        name = "MEM/WB";
    }
};

class RetireReg : public SimplePipelineRegMixin<RetireReg, Wb> {
public:
    RetireReg(Clock &clk, Wb *wb, Instruction_pool<INST_POOL_SIZE> *inst_pool)
        : SimplePipelineRegMixin(clk, wb), inst_pool(inst_pool) {
        name = "Retire";
    }
    void accept() {
        SimplePipelineRegMixin::accept();
        if (data && data->is_bubble() == false)
            inst_pool->freeInst();
    }
private:
    Instruction_pool<INST_POOL_SIZE> *inst_pool;
};

// HazardDetectionUnit function implementation
inline void HazardDetectionUnit::handle_load_use_hazard() {
    if (logged)
        std::cout << "detect hazard !" << std::endl;
    // send stall signal to IF/ID & PC
    // send flush signal to ID/EX
    pc_reg->receive_stall();
    if_id_reg->receive_stall();
    id_ex_reg->flush();
    if (logged) {
        std::cout << "Send flush to ID/EX !" << std::endl;
    }
}

inline void HazardDetectionUnit::receive_IdEx_rd(RegNum rd) {
    IdExRegRd = rd;
    check_hazard();
}

inline void HazardDetectionUnit::receive_IfId_rs(RegNum rs1, RegNum rs2) {
    IfIdRegRs1 = rs1;
    IfIdRegRs2 = rs2;
    check_hazard();
}

inline void HazardDetectionUnit::check_hazard() {
    static bool load_use_detection_ready = false;

    if (load_use_detection_ready == false) {
        load_use_detection_ready = true;
        return;
    }

    load_use_detection_ready = false;
    if ((IdExRegRd != IfIdRegRs1) && (IdExRegRd != IfIdRegRs2))
        return;
    
    handle_load_use_hazard();
}