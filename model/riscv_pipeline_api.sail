// instruction types
enum instruction_type = {
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
}

// pipeline api
val createInstrForStageInfo = pure {c: "createInstrForStageInfo"} : unit -> unit
val sendInstToPipeline = pure {c: "sendInstToPipeline"} : unit -> unit

val show_performance = pure {c: "show_performance"} : unit -> unit
val show_cache_info = pure {c: "show_cache_info"} : unit -> unit

val read_addr = pure {c: "read_addr"} : xlenbits -> unit
val read_ls_addr = pure {c: "read_ls_addr"} : xlenbits -> unit

val read_rs_val = pure {c: "read_rs_val"} : (xlenbits, xlenbits) -> unit

val read_Rtype_regs = pure {c: "read_Rtype_regs"} : (bits(5), bits(5), bits(5)) -> unit
val read_Itype_regs_ = pure {c: "read_Itype_regs"} : (bits(5), bits(5)) -> unit
val read_Itype_xRs1 = pure {c: "read_Itype_xRs1"} : (bits(5)) -> unit
val read_Itype_xRs1xRd = pure {c: "read_Itype_xRs1xRd"} : unit -> unit
overload read_Itype_regs = {read_Itype_regs_, read_Itype_xRs1, read_Itype_xRs1xRd}
val read_SBtype_regs = pure {c: "read_SBtype_regs"} : (bits(5), bits(5)) -> unit
val read_UJtype_regs = pure {c: "read_UJtype_regs"} : (bits(5)) -> unit

val read_type = pure {c: "read_type"} : instruction_type -> unit
val read_taken = pure {c: "read_taken"} : bool -> unit