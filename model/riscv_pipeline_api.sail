// instruction types
enum instruction_type = {
  INST_unknown,
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
  INST_lb, INST_lh, INST_lw, INST_ld, INST_sb, INST_sh, INST_sw, INST_sd,
  INST_lbu, INST_lhu, INST_lwu,
  INST_fence, INST_ebreak, INST_ecall,
  INST_csrrc, INST_csrrci, INST_csrrs, INST_csrrsi, INST_csrrw, INST_csrrwi,
  INST_privilege, INST_nop,
}

// pipeline api
val createInstrForStageInfo = pure {c: "createInstrForStageInfo"} : unit -> unit
val sendInstToPipeline = pure {c: "sendInstToPipeline"} : unit -> unit
val show_cycle_count = pure {c: "show_cycle_count"} : unit -> unit
val show_cache_info = pure {c: "show_cache_info"} : unit -> unit
val read_addr = pure {c: "read_addr"} : xlenbits -> unit
val read_ls_addr = pure {c: "read_ls_addr"} : xlenbits -> unit
val read_Rtype = pure {c: "read_Rtype"} : (bits(5), bits(5), bits(5)) -> unit
val read_Itype_ = pure {c: "read_Itype"} : (bits(5), bits(5)) -> unit
val read_Itype_xRs1 = pure {c: "read_Itype_xRs1"} : (bits(5)) -> unit
val read_Itype_xRs1xRd = pure {c: "read_Itype_xRs1xRd"} : unit -> unit
overload read_Itype = {read_Itype_, read_Itype_xRs1, read_Itype_xRs1xRd}
val read_SBtype = pure {c: "read_SBtype"} : (bits(5), bits(5)) -> unit
val read_UJtype = pure {c: "read_UJtype"} : (bits(5)) -> unit
val read_type = pure {c: "read_type"} : instruction_type -> unit
// val read_rs = pure {c: "read_rs"} : (bits(5), bits(5)) -> unit
// val read_rs1 = pure {c: "read_rs1"} : bits(5) -> unit
// val read_rd = pure {c: "read_rd"} : bits(5) -> unit
val read_taken = pure {c: "read_taken"} : bool -> unit

/* cache api test */
// val cache_init = impure {c: "cache_init"} : unit -> unit

// val ic_read = impure {c: "ic_read"} : xlenbits -> unit
// val dc_read = impure {c: "dc_read"} : xlenbits -> unit
// val dc_write = impure {c: "dc_write"} : xlenbits -> unit

// val print_ic_stats = pure {c: "print_ic_stats"} : unit -> unit
// val print_dc_stats = pure {c: "print_dc_stats"} : unit -> unit
// val print_l2_stats = pure {c: "print_l2_stats"} : unit -> unit
/* cache api test */