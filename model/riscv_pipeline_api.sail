// instruction types
enum instruction_type = {
  INST_nop,
  // RV64I
  INST_add, INST_addw, INST_addi, INST_addiw, INST_sub, INST_subw,
  INST_and, INST_andi, INST_or, INST_ori, INST_xor, INST_xori,
  INST_sll, INST_sllw, INST_slli, INST_slliw,
  INST_sra, INST_sraw, INST_srai, INST_sraiw,
  INST_srl, INST_srlw, INST_srli, INST_srliw,
  INST_lui, INST_auipc,
  INST_slt, INST_sltu, INST_slti, INST_sltiu,
  INST_be, INST_bne,
  INST_bqe, INST_bqeu, INST_blt, INST_bltu,
  INST_jal, INST_jalr,
  INST_lb, INST_lh, INST_lw, INST_ld, INST_sb, INST_sh, INST_sw, INST_sd,
  INST_lbu, INST_lhu, INST_lwu,
  INST_fence, INST_ebreak, INST_ecall,
  INST_csrrc, INST_csrrci, INST_csrrs, INST_csrrsi, INST_csrrw, INST_csrrwi
}

// pipeline api
val createInstrForStageInfo = pure {c: "createInstrForStageInfo"} : unit -> unit
val sendInstToPipeline = pure {c: "sendInstToPipeline"} : unit -> unit
val read_addr = pure {c: "read_addr"} : xlenbits -> unit
val read_type = pure {c: "read_type"} : instruction_type -> unit
val read_rs = pure {c: "read_rs"} : (bits(5), bits(5)) -> unit
val read_rs1 = pure {c: "read_rs1"} : bits(5) -> unit
val read_rs2 = pure {c: "read_rs2"} : bits(5) -> unit
val read_rd = pure {c: "read_rd"} : bits(5) -> unit
val read_taken = pure {c: "read_taken"} : bool -> unit
