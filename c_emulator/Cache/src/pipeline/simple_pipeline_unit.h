#pragma once

#include "../Instruction/instruction_pool.h"
#include "../Instruction/instruction.h"
#include "simple_pipeline_unit_concept.h"

class Cache {};

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
    bool load_use_detection_ready = false;
    RegNum IdExRegRd, IfIdRegRs1, IfIdRegRs2;
    PCReg *pc_reg;
    IfIdReg *if_id_reg;
    IdExReg *id_ex_reg;

    bool detect_load_use_hazard();
    void send_stall_for_load_use_hazard();
    void handle_load_use_hazard();
};

class Fetch : public SimplePipelineStageLogicMixIn<Fetch> {
public:
    Fetch(Cache *ic) : icache(ic) {}

    void process_stage() {
        std::cout << "Fetch!" << std::endl;
        // ToDo: access cache and send signal to control unit if missed
    }

    void print_name() {
        std::cout << "Fetch";
    }
private:
    Cache *icache;
};
class Decode : public SimplePipelineStageLogicMixIn<Decode> {
public:
    Decode(HazardDetectionUnit *hdu) : hazard_detection_unit(hdu) {}

    void process_stage() {
        std::cout << "Decode!" << std::endl;
        // send rs
        if (data == nullptr)
            return;
        hazard_detection_unit->
            receive_IfId_rs(data->rs1, data->rs2);
    }

    void print_name() {
        std::cout << "Decode";
    }
private:
    HazardDetectionUnit *hazard_detection_unit;
};
class Execute: public SimplePipelineStageLogicMixIn<Execute> {
public:
    Execute(HazardDetectionUnit *hdu) : hazard_detection_unit(hdu) {}

    void process_stage() {
        if (data == nullptr)
            return;
        if (data->is_load())
            hazard_detection_unit->
                receive_IdEx_rd(data->rd);
    }

    void print_name() {
        std::cout << "Execute";
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

    void print_name() {
        std::cout << "Mem";
    }
private:
    Cache *dcache;
};

class Wb : public SimplePipelineStageLogicMixIn<Wb> {
public:
    Wb(Instruction_pool<INST_POOL_SIZE> *inst_pool)
        : inst_pool(inst_pool) {}
    
    void process_stage() {
        std::cout << "Wb!" << std::endl;
        std::cout << "free inst" << std::endl;
        inst_pool->freeInst();
    }

    void print_name() {
        std::cout << "Wb";
    }
private:
    Instruction_pool<INST_POOL_SIZE> *inst_pool;
};

class PCReg : public SimplePipelineRegMixin<PCReg, NoStage, Fetch> {
public:
    PCReg(Clock &clk, NoStage *noStage, Fetch *fetch)
        : SimplePipelineRegMixin(clk, noStage, fetch) {}
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

    void print_name() {
        std::cout << "PC";
    }
};
class IfIdReg : public SimplePipelineRegMixin<IfIdReg, Fetch, Decode> {
public:
    IfIdReg(Clock &clk, Fetch *fetch, Decode *decode)
        : SimplePipelineRegMixin(clk, fetch, decode) {}

    void print_name() {
        std::cout << "IF/ID";
    }
};
class IdExReg : public SimplePipelineRegMixin<IdExReg, Decode, Execute> {
public:
    IdExReg(Clock &clk, Decode *decode, Execute *execute)
        : SimplePipelineRegMixin(clk, decode, execute) {}
    
    void flush() {
        if (data == nullptr)
            return;
        data->set_bubble();
    }

    void print_name() {
        std::cout << "ID/EX";
    }
};
class ExMemReg : public SimplePipelineRegMixin<ExMemReg, Execute, Mem> {
public:
    ExMemReg(Clock &clk, Execute *execute, Mem *mem)
        : SimplePipelineRegMixin(clk, execute, mem) {}

    void print_name() {
        std::cout << "EX/MEM";
    }
};
class MemWbReg : public SimplePipelineRegMixin<MemWbReg, Mem, Wb> {
public:
    MemWbReg(Clock &clk, Mem *mem, Wb *wb)
        : SimplePipelineRegMixin(clk, mem, wb) {}

    void print_name() {
        std::cout << "MEM/WB";
    }
};

// HazardDetectionUnit function implementation
inline void HazardDetectionUnit::handle_load_use_hazard() {
    if (detect_load_use_hazard() == true)
        send_stall_for_load_use_hazard();
    load_use_detection_ready = false;
}

inline void HazardDetectionUnit::send_stall_for_load_use_hazard() {
    std::cout << "detect hazard !" << std::endl;
    pc_reg->receive_stall();
    if_id_reg->receive_stall();
    id_ex_reg->flush();
}

inline bool HazardDetectionUnit::detect_load_use_hazard() {
    return (IdExRegRd == IfIdRegRs1) || (IdExRegRd == IfIdRegRs2);
}

inline void HazardDetectionUnit::receive_IdEx_rd(RegNum rd) {
    IdExRegRd = rd;
    if (load_use_detection_ready)
        handle_load_use_hazard();
    else
        load_use_detection_ready = true;
}

inline void HazardDetectionUnit::receive_IfId_rs(RegNum rs1, RegNum rs2) {
    IfIdRegRs1 = rs1;
    IfIdRegRs2 = rs2;
    if (load_use_detection_ready)
        handle_load_use_hazard();
    else
        load_use_detection_ready = true;
}