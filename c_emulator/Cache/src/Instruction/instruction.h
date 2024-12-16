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
        rs2 = new_rs2;
    }
    void set_rs1(RegNum new_rs1) {
        rs1 = new_rs1;
        rs2 = -1;
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

    // int rs_usage(Instruction_type inst_type);
};

// int Instruction::rs_usage(Instruction_type inst_type) {
//     switch(inst_type) {
//         // 不使用 rs1 和 rs2，回傳 0
//         case INST_nop:
//         case INST_lui:
//         case INST_auipc:
//         case INST_jal:
//         case INST_ecall:
//         case INST_ebreak:
//         case INST_fence:  // 確保 INST_fence 在這裡
//             return 0;

//         // 使用 rs1，回傳 1
//         case INST_addi: case INST_addiw:
//         case INST_andi: case INST_ori: case INST_xori:
//         case INST_slli: case INST_slliw:
//         case INST_srli: case INST_srliw:
//         case INST_srai: case INST_sraiw:
//         case INST_slti: case INST_sltiu:
//         case INST_jalr:
//         case INST_lb: case INST_lbu: case INST_lh: case INST_lhu: case INST_lw: case INST_lwu: case INST_ld:
//         case INST_sb: case INST_sh: case INST_sw: case INST_sd:
//         case INST_csrrc: case INST_csrrci: case INST_csrrs: case INST_csrrsi: case INST_csrrw: case INST_csrrwi:
//             return 1;

//         // 使用 rs1 和 rs2，回傳 2
//         case INST_add: case INST_addw: case INST_sub: case INST_subw:
//         case INST_and: case INST_or: case INST_xor:
//         case INST_sll: case INST_sllw: case INST_srl: case INST_srlw: case INST_sra: case INST_sraw:
//         case INST_slt: case INST_sltu:
//         case INST_beq: case INST_bne: case INST_bge: case INST_bgeu: case INST_blt: case INST_bltu:
//             return 2;

//         // 預設回傳 -1 (未識別指令)
//         default:
//             return -1;
//     }
// }