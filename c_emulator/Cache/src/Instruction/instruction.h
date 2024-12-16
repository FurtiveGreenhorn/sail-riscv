#pragma once
#include <cstdint>
#include <cassert>

// instruction types
enum Instruction_type : unsigned {
    INST_nop,
    // RV64I
    INST_add, INST_addw, INST_addi, INST_addiw, INST_sub, INST_subw,
    INST_and, INST_andi, INST_or, INST_ori, INST_xor, INST_xori,
    INST_sll, INST_sllw, INST_slli, INST_slliw,
    INST_sra, INST_sraw, INST_srai, INST_sraiw,
    INST_srl, INST_srlw, INST_srli, INST_srliw,
    INST_lui, INST_auipc,
    INST_slt, INST_sltu, INST_slti, INST_sltiu,
    INST_beq, INST_bne,
    INST_bge, INST_bgeu, INST_blt, INST_bltu,
    INST_jal, INST_jalr,
    INST_lb, INST_lbu, INST_lh, INST_lhu,INST_lw, INST_lwu, INST_ld, 
    INST_sb, INST_sh, INST_sw, INST_sd,
    INST_fence, INST_ebreak, INST_ecall,
    INST_csrrc, INST_csrrci, INST_csrrs, INST_csrrsi, INST_csrrw, INST_csrrwi
};

using RegNum = unsigned;
struct Instruction {
    uint64_t addr;
    Instruction_type type;
    RegNum rs1, rs2, rd;
    bool taken, is_used = false;

    void set_addr(uint64_t new_addr) {
        addr = new_addr;
    }
    void set_type(Instruction_type new_type) {
        new_type = type;
    }
    void set_rs(RegNum new_rs1, RegNum new_rs2) {
        rs1 = new_rs1;
    }
    void set_rs1(RegNum new_rs1) {
        rs1 = new_rs1;
    }
    void set_rs2(RegNum new_rs2) {
        rs2 = new_rs2;
    }
    void set_rd(RegNum new_rd) {
        rd = new_rd;
    } 
    void set_taken(bool new_taken) {
        taken = new_taken;
    }
    void set_bubble() {
        type = INST_nop;
    }

    bool is_load() {
        return (INST_lb <= type) && (type <= INST_ld);
    }
    bool is_branch() {
        return (INST_beq <= type) && (type <= INST_bltu);
    }
    bool is_jump() {
        return (type == INST_jal) || (type == INST_jalr);
    }
};

void show_instruction(int rs1, int rs2, int rd);
