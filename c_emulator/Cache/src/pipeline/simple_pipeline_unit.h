#pragma once

#include "../Instruction/instruction_pool.h"
#include "../Instruction/instruction.h"
#include "simple_pipeline_unit_concept.h"

class Cache {};

class Fetch;
class PCReg;
class IfIdReg;
class IdExReg;

/**
 * @brief Detects and manages load-use hazards in the pipeline.
 *
 * The `HazardDetectionUnit` monitors pipeline registers to detect load-use hazards, a scenario
 * where a subsequent instruction tries to use a register value that has not yet been loaded
 * by a preceding load instruction. When such a hazard is detected, the unit signals stalls
 * to the pipeline registers and flushes the pipeline to properly synchronize execution.
 */
class HazardDetectionUnit {
public:
    /**
     * @brief Constructs a HazardDetectionUnit.
     *
     * @param pc_reg Pointer to the PC register stage.
     * @param ifid Pointer to the IF/ID pipeline register.
     * @param idex Pointer to the ID/EX pipeline register.
     */
    HazardDetectionUnit(PCReg *pc_reg, IfIdReg *ifid, IdExReg *idex) 
        : pc_reg(pc_reg), if_id_reg(ifid), id_ex_reg(idex) {}

    /**
     * @brief Receives the ID/EX stage destination register (rd) for hazard detection.
     *
     * This method is called when the execute stage identifies a load instruction that will
     * produce a register value. The `rd` register number is recorded, and if load-use hazard
     * detection is ready, `handle_load_use_hazard()` is triggered.
     *
     * @param rd The register number (rd) from the ID/EX stage.
     */
    void receive_IdEx_rd(RegNum rd);

    /**
     * @brief Receives the IF/ID stage source registers (rs1, rs2) for hazard detection.
     *
     * This method is called when the decode stage identifies the source registers needed by
     * an instruction currently in the IF/ID stage. If load-use hazard detection is ready,
     * `handle_load_use_hazard()` is triggered.
     *
     * @param rs1 The first source register number.
     * @param rs2 The second source register number.
     */
    void receive_IfId_rs(RegNum rs1, RegNum rs2);
    
private:
    /**
     * @brief Indicates if load-use hazard detection logic is ready to be performed.
     *
     * This flag is set after receiving both the destination register from ID/EX and 
     * the source registers from IF/ID, enabling hazard detection.
     */
    bool load_use_detection_ready = false;

    /**
     * @brief The destination register from the ID/EX stage (a potential load target).
     */
    RegNum IdExRegRd;

    /**
     * @brief The source registers from the IF/ID stage instruction.
     */
    RegNum IfIdRegRs1, IfIdRegRs2;

    /**
     * @brief Pointers to pipeline registers involved in hazard detection and handling.
     */
    PCReg *pc_reg;
    IfIdReg *if_id_reg;
    IdExReg *id_ex_reg;

    /**
     * @brief Detects if there is a load-use hazard based on recorded registers.
     *
     * @return `true` if a load-use hazard is detected; otherwise `false`.
     */
    bool detect_load_use_hazard();

    /**
     * @brief Sends stall signals to pipeline registers and flushes the ID/EX register when a load-use hazard is detected.
     */
    void send_stall_for_load_use_hazard();

    /**
     * @brief Handles load-use hazard detection and invokes stall signals if necessary.
     *
     * If `detect_load_use_hazard()` returns true, it triggers `send_stall_for_load_use_hazard()`.
     * Resets the `load_use_detection_ready` flag after handling.
     */
    void handle_load_use_hazard();
};


/**
 * @brief The Fetch stage of the pipeline.
 *
 * The `Fetch` stage fetches instructions from the instruction cache. This class
 * is a pipeline stage that inherits from `SimplePipelineStageLogicMixIn`.
 */
class Fetch : public SimplePipelineStageLogicMixIn<Fetch> {
public:
    /**
     * @brief Constructs the Fetch stage with a given instruction cache.
     *
     * @param ic Pointer to the instruction cache (Cache).
     */
    Fetch(Cache *ic) : icache(ic) {}

    /**
     * @brief Processes the fetch stage logic.
     *
     * Currently, it just prints "Fetch!" for demonstration.
     * A real implementation would access the instruction cache and handle any misses.
     */
    void process_stage() {
        std::cout << "Fetch!" << std::endl;
        // ToDo: access cache and send signal to control unit if missed
    }

    /**
     * @brief Prints the name of this stage, used for logging.
     */
    void print_name() {
        std::cout << "Fetch";
    }

private:
    /**
     * @brief Pointer to the instruction cache used by this stage.
     */
    Cache *icache;
};

/**
 * @brief The Decode stage of the pipeline.
 *
 * This class decodes fetched instructions and prepares operands for the next stage.
 * It inherits from `SimplePipelineStageLogicMixIn` and interacts with the `HazardDetectionUnit`.
 */
class Decode : public SimplePipelineStageLogicMixIn<Decode> {
public:
    /**
     * @brief Constructs the Decode stage.
     *
     * @param hdu Pointer to the hazard detection unit to inform about source registers.
     */
    Decode(HazardDetectionUnit *hdu) : hazard_detection_unit(hdu) {}

    /**
     * @brief Decodes the instruction and notifies the HazardDetectionUnit of source registers.
     */
    void process_stage() {
        std::cout << "Decode!" << std::endl;
        // If no data (no instruction), do nothing
        if (data == nullptr)
            return;
        // Notify hazard detection unit of the source registers
        hazard_detection_unit->receive_IfId_rs(data->rs1, data->rs2);
    }

    /**
     * @brief Prints the stage name, "Decode".
     */
    void print_name() {
        std::cout << "Decode";
    }

private:
    /**
     * @brief Pointer to the hazard detection unit for load-use hazard prevention.
     */
    HazardDetectionUnit *hazard_detection_unit;
};

/**
 * @brief The Execute stage of the pipeline.
 *
 * This stage performs arithmetic or logic operations on the operands prepared by Decode.
 * In this simplified version, it only notifies the hazard detection unit if the instruction
 * is a load, which sets up conditions for a potential load-use hazard.
 */
class Execute : public SimplePipelineStageLogicMixIn<Execute> {
public:
    /**
     * @brief Constructs the Execute stage.
     *
     * @param hdu Pointer to the hazard detection unit.
     */
    Execute(HazardDetectionUnit *hdu) : hazard_detection_unit(hdu) {}

    /**
     * @brief Processes the Execute stage logic.
     *
     * If the current instruction is a load, notifies the hazard detection unit with the target register (rd).
     */
    void process_stage() {
        if (data == nullptr)
            return;
        if (data->is_load())
            hazard_detection_unit->receive_IdEx_rd(data->rd);
    }

    /**
     * @brief Prints the stage name, "Execute".
     */
    void print_name() {
        std::cout << "Execute";
    }

private:
    /**
     * @brief Pointer to the hazard detection unit for load-use hazard detection.
     */
    HazardDetectionUnit *hazard_detection_unit;
};

/**
 * @brief The Memory stage of the pipeline.
 *
 * This stage handles data memory accesses for load/store instructions. It inherits from `SimplePipelineStageLogicMixIn`.
 */
class Mem : public SimplePipelineStageLogicMixIn<Mem> {
public:
    /**
     * @brief Constructs the Memory stage.
     *
     * @param dc Pointer to the data cache.
     */
    Mem(Cache *dc) : dcache(dc) {}

    /**
     * @brief Processes memory operations.
     *
     * Currently a placeholder that could access the data cache and handle memory misses.
     */
    void process_stage() {
        // ToDo: access cache and send signal to control unit if missed
    }

    /**
     * @brief Prints the stage name, "Mem".
     */
    void print_name() {
        std::cout << "Mem";
    }

private:
    /**
     * @brief Pointer to the data cache.
     */
    Cache *dcache;
};

/**
 * @brief The Write-Back (WB) stage of the pipeline.
 *
 * This stage writes the results of executed instructions back to the register file, and
 * frees the instruction slot from the instruction pool.
 */
class Wb : public SimplePipelineStageLogicMixIn<Wb> {
public:
    /**
     * @brief Constructs the Wb stage.
     *
     * @param inst_pool Pointer to an instruction pool managing instruction memory resources.
     */
    Wb(Instruction_pool<INST_POOL_SIZE> *inst_pool)
        : inst_pool(inst_pool) {}
    
    /**
     * @brief Processes the write-back stage.
     *
     * Prints diagnostic messages and frees the instruction from the pool.
     */
    void process_stage() {
        std::cout << "Wb!" << std::endl;
        std::cout << "free inst" << std::endl;
        inst_pool->freeInst();
    }

    /**
     * @brief Prints the stage name, "Wb".
     */
    void print_name() {
        std::cout << "Wb";
    }

private:
    /**
     * @brief Pointer to the instruction pool, used to manage and free instructions.
     */
    Instruction_pool<INST_POOL_SIZE> *inst_pool;
};

/**
 * @brief The PC (Program Counter) register stage.
 *
 * The PCReg holds the address of the next instruction to be fetched. It inherits from
 * `SimplePipelineRegMixin` to integrate clocking and data movement logic.
 */
class PCReg : public SimplePipelineRegMixin<PCReg, NoStage, Fetch> {
public:
    /**
     * @brief Constructs a PC register stage and registers it with the system clock.
     *
     * @param clk       Reference to the system clock.
     * @param noStage   Pointer to a `NoStage` object (no preceding stage).
     * @param fetch     Pointer to the Fetch stage.
     */
    PCReg(Clock &clk, NoStage *noStage, Fetch *fetch)
        : SimplePipelineRegMixin(clk, noStage, fetch) {}

    /**
     * @brief Receives a new instruction pointer (PC) from outside, such as from the hazard unit.
     *
     * @param new_pc A pointer to an instruction object representing the next PC value.
     */
    void receive(Instruction *new_pc) {
        data = new_pc;
    }

    /**
     * @brief Checks if the stage is stalled.
     *
     * Used by the Sail frontend to ensure that instructions are not processed when the pipeline is stalled.
     *
     * @return `true` if the pipeline register is stalled; otherwise `false`.
     */
    bool isStalled() const {
        return stallFlag;
    }

    /**
     * @brief Prints the stage name, "PC".
     */
    void print_name() {
        std::cout << "PC";
    }
};

/**
 * @brief The IF/ID pipeline register.
 *
 * Bridges the Fetch and Decode stages. Accepts instructions from Fetch at clock end,
 * and transmits them to Decode at clock start.
 */
class IfIdReg : public SimplePipelineRegMixin<IfIdReg, Fetch, Decode> {
public:
    /**
     * @brief Constructs the IF/ID register.
     *
     * @param clk   Reference to the system clock.
     * @param fetch Pointer to the Fetch stage.
     * @param decode Pointer to the Decode stage.
     */
    IfIdReg(Clock &clk, Fetch *fetch, Decode *decode)
        : SimplePipelineRegMixin(clk, fetch, decode) {}

    /**
     * @brief Prints the name of this pipeline register, "IF/ID".
     */
    void print_name() {
        std::cout << "IF/ID";
    }
};

/**
 * @brief The ID/EX pipeline register.
 *
 * Bridges the Decode and Execute stages. It may be flushed by the HazardDetectionUnit
 * to inject a bubble (no-op) when hazards occur.
 */
class IdExReg : public SimplePipelineRegMixin<IdExReg, Decode, Execute> {
public:
    /**
     * @brief Constructs the ID/EX register.
     *
     * @param clk     Reference to the system clock.
     * @param decode  Pointer to the Decode stage.
     * @param execute Pointer to the Execute stage.
     */
    IdExReg(Clock &clk, Decode *decode, Execute *execute)
        : SimplePipelineRegMixin(clk, decode, execute) {}
    
    /**
     * @brief Flushes the ID/EX pipeline register by turning the current instruction into a bubble.
     *
     * This is done to resolve hazards by effectively canceling the current instruction.
     */
    void flush() {
        if (data == nullptr)
            return;
        data->set_bubble();
    }

    /**
     * @brief Prints the name of this pipeline register, "ID/EX".
     */
    void print_name() {
        std::cout << "ID/EX";
    }
};

/**
 * @brief The EX/MEM pipeline register.
 *
 * Bridges the Execute and Memory stages, passing the results of execution to the memory stage.
 */
class ExMemReg : public SimplePipelineRegMixin<ExMemReg, Execute, Mem> {
public:
    /**
     * @brief Constructs the EX/MEM register.
     *
     * @param clk     Reference to the system clock.
     * @param execute Pointer to the Execute stage.
     * @param mem     Pointer to the Memory stage.
     */
    ExMemReg(Clock &clk, Execute *execute, Mem *mem)
        : SimplePipelineRegMixin(clk, execute, mem) {}

    /**
     * @brief Prints the name of this pipeline register, "EX/MEM".
     */
    void print_name() {
        std::cout << "EX/MEM";
    }
};

/**
 * @brief The MEM/WB pipeline register.
 *
 * Bridges the Memory and Write-Back stages, delivering data from memory operations
 * to the WB stage for final register write-back.
 */
class MemWbReg : public SimplePipelineRegMixin<MemWbReg, Mem, Wb> {
public:
    /**
     * @brief Constructs the MEM/WB register.
     *
     * @param clk Reference to the system clock.
     * @param mem Pointer to the Memory stage.
     * @param wb  Pointer to the Write-Back stage.
     */
    MemWbReg(Clock &clk, Mem *mem, Wb *wb)
        : SimplePipelineRegMixin(clk, mem, wb) {}

    /**
     * @brief Prints the name of this pipeline register, "MEM/WB".
     */
    void print_name() {
        std::cout << "MEM/WB";
    }
};

// Implementation of HazardDetectionUnit methods

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
