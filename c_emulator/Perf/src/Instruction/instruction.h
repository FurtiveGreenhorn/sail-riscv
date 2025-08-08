#pragma once
#include <cstdint>
#include <cassert>

namespace pipeline_simulator {

// Enum to represent different instruction types
enum Instruction_type : unsigned {
    // INST_unknown is used to represent instructions whose type has not yet been determined.
    // This can occur when parsing or decoding an instruction, and the type is unknown or invalid.
    INST_unknown,

    // INST_nop represents a No-Operation (NOP) instruction.
    // While the specification suggests using "ADDI x0, x0, 0" to implement NOPs, 
    // we use this custom instruction for simulation efficiency. It accelerates the process
    // of inserting a bubble in the pipeline during simulation without needing to decode a full instruction.
    INST_nop,

    // ------------------------ Unprivileged Architecture ------------------------
    // ---------- zicsr ----------
    INST_csrrc, INST_csrrci, INST_csrrs, INST_csrrsi, INST_csrrw, INST_csrrwi,
    
    // ---------- RV32I & RV64I ----------
    // system control
    INST_fence, INST_ebreak, INST_ecall,
    // arithmetic
    INST_add, INST_addi, INST_sub,
    INST_addw, INST_addiw, INST_subw,
    // logic
    INST_and, INST_or, INST_xor,
    INST_andi, INST_ori, INST_xori,
    // shift
    INST_sll, INST_sra, 
    INST_slli, INST_srai,
    INST_srl, INST_srli,
    INST_sllw, INST_sraw,
    INST_slliw,INST_sraiw,
    INST_srlw, INST_srliw,
    // load upper immediate
    INST_lui, INST_auipc,
    // compare
    INST_slt, INST_sltu, INST_slti, INST_sltiu,
    // branch
    INST_beq, INST_bne,
    INST_bge, INST_blt,
    INST_bgeu, INST_bltu,
    // jump
    INST_jal, INST_jalr,
    // load
    INST_lb, INST_lbu,
    INST_lh, INST_lhu,
    INST_lw, INST_lwu, INST_ld, 
    // store
    INST_sb, INST_sh, INST_sw, INST_sd,
 
    // ------------------------- RV32M Standard Extension -------------------------
    // multiplication
    // lower 32 bits
    INST_mul,
    // upper 32 bits
    INST_mulh, INST_mulhu, INST_mulhsu,
    INST_mulw,
    // division
    INST_div, INST_divu,
    INST_divw, INST_divuw,
    // remainder
    INST_rem, INST_remu,
    INST_remw, INST_remuw,

    // ------------------------- Privileged Architecture -------------------------
    INST_mret, INST_sret, INST_sfence_vma, INST_wfi,
};

enum class ExecutionUnitType {
    // Execution unit for integer arithmetic and logic operations
    INT_ALU,
    // Execution unit for integer multiplication
    MUL_UNIT,
    // Execution unit for integer division
    DIV_UNIT,
    // No execution unit used
    NONE
};



using RegNum = unsigned;
constexpr unsigned REGISTER_NOT_USED = static_cast<unsigned>(-1);
struct Instruction {
    uint64_t addr, ls_addr/*for load or store*/, target_addr/*for branch target*/;
    Instruction_type type;
    RegNum rs1, rs2, rd;
    bool taken, is_used;
    uint64_t rs1_val, rs2_val;

    Instruction() : is_used(false) { reset(); }

    Instruction &allocate() {
        // for debug
        assert(is_used == false);
        is_used = true;
        return *this;
    }

    void free() {
        // for debug
        assert(is_used);
        is_used = false;
        reset();
    }

    void set_addr(uint64_t new_addr) {
        addr = new_addr;
    }
    void set_ls_addr(uint64_t new_addr) {
        ls_addr = new_addr;
    }
    
    void set_rs_val(uint64_t new_rs1_val, uint64_t new_rs2_val) {
        rs1_val = new_rs1_val;
        rs2_val = new_rs2_val;
    }
    // void set_rs1_val(uint64_t new_rs1_val) {
    //     rs1_val = new_rs1_val;
    // }
    // void set_rs2_val(uint64_t new_rs2_val) {
    //     rs2_val = new_rs2_val;
    // }

    void set_Rtype(RegNum new_rs2, RegNum new_rs1, RegNum new_rd) {
        rs1 = new_rs1;
        rs2 = new_rs2;
        rd = new_rd;
    }
    void set_Itype(RegNum new_rs1, RegNum new_rd) {
        rs1 = new_rs1;
        rd = new_rd;
    }  
    void set_SBtype(RegNum new_rs2, RegNum new_rs1) {
        rs1 = new_rs1;
        rs2 = new_rs2;
    }
    void set_UJtype(RegNum new_rd) {
        rd = new_rd;
    }   

    void set_type(Instruction_type new_type) {
        type = new_type;
    }

    void set_taken(bool new_taken) {
        taken = new_taken;
    }

    bool branch_taken() const {
        return taken;
    }

    void set_target_addr(uint64_t new_target_addr) {
        target_addr = new_target_addr;
    }

    uint64_t get_target_addr() const {
        return target_addr;
    }

    void set_bubble() {
        type = INST_nop;
    }
    bool is_bubble() {
        return (type == INST_nop);
    }

    bool is_load() {
        return (INST_lb <= type) && (type <= INST_ld);
    }
    bool is_store() {
        return (INST_sb <= type) && (type <= INST_sd);
    }
    bool is_branch() {
        return (INST_beq <= type) && (type <= INST_bltu);
    }
    bool is_jump() {
        return (type == INST_jal) || (type == INST_jalr);
    }

    inline ExecutionUnitType get_execution_unit_type();

    void reset() {
        type = INST_unknown;
        rs1 = REGISTER_NOT_USED;
        rs2 = REGISTER_NOT_USED;
        rd = REGISTER_NOT_USED;
    }
};

ExecutionUnitType Instruction::get_execution_unit_type() {
    switch (type) {
        // arithmetic
        case INST_add: case INST_addi: case INST_sub:
        case INST_addw: case INST_addiw: case INST_subw:
        // logic
        case INST_and: case INST_or: case INST_xor:
        case INST_andi: case INST_ori: case INST_xori:
        // shift
        case INST_sll: case INST_sra: case INST_slli:
        case INST_srai: case INST_srl: case INST_srli:
        case INST_sllw: case INST_sraw: case INST_slliw:
        case INST_sraiw: case INST_srlw: case INST_srliw:
        // load upper immediate
        case INST_lui: case INST_auipc:
        // compare
        case INST_slt: case INST_sltu: case INST_slti: case INST_sltiu:
        // branch
        case INST_beq: case INST_bne: case INST_bge: case INST_blt:
        case INST_bgeu: case INST_bltu:
        // jump
        case INST_jal: case INST_jalr:
        // load
        case INST_lb: case INST_lbu: case INST_lh: case INST_lhu:
        case INST_lw: case INST_lwu: case INST_ld:
        // store
        case INST_sb: case INST_sh: case INST_sw: case INST_sd:
            return ExecutionUnitType::INT_ALU;
        break;
        case INST_mul:
        case INST_mulh:
        case INST_mulhu:
        case INST_mulhsu:
        case INST_mulw:
            return ExecutionUnitType::MUL_UNIT;
        break;
        case INST_div:
        case INST_divu:
        case INST_divw:
        case INST_divuw:
        case INST_rem:
        case INST_remu:
        case INST_remw:
        case INST_remuw:
            return ExecutionUnitType::DIV_UNIT;
        break;
        default:
            return ExecutionUnitType::NONE;
        break;
    }
}

} // namespace pipeline_simulator