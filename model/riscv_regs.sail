/*=======================================================================================*/
/*  This Sail RISC-V architecture model, comprising all files and                        */
/*  directories except where otherwise noted is subject the BSD                          */
/*  two-clause license in the LICENSE file.                                              */
/*                                                                                       */
/*  SPDX-License-Identifier: BSD-2-Clause                                                */
/*=======================================================================================*/

/* program counter */

register PC       : xlenbits
register nextPC   : xlenbits

/* internal state to hold instruction bits for faulting instructions */
register instbits : xlenbits

/* register file and accessors */
register x1  : regtype
register x2  : regtype
register x3  : regtype
register x4  : regtype
register x5  : regtype
register x6  : regtype
register x7  : regtype
register x8  : regtype
register x9  : regtype
register x10 : regtype
register x11 : regtype
register x12 : regtype
register x13 : regtype
register x14 : regtype
register x15 : regtype
register x16 : regtype
register x17 : regtype
register x18 : regtype
register x19 : regtype
register x20 : regtype
register x21 : regtype
register x22 : regtype
register x23 : regtype
register x24 : regtype
register x25 : regtype
register x26 : regtype
register x27 : regtype
register x28 : regtype
register x29 : regtype
register x30 : regtype
register x31 : regtype

/* speculative execution registers */
register x1_spec : regtype
register x2_spec : regtype
register x3_spec : regtype
register x4_spec : regtype
register x5_spec : regtype
register x6_spec : regtype
register x7_spec : regtype
register x8_spec : regtype
register x9_spec : regtype
register x10_spec : regtype
register x11_spec : regtype
register x12_spec : regtype
register x13_spec : regtype
register x14_spec : regtype
register x15_spec : regtype
register x16_spec : regtype
register x17_spec : regtype
register x18_spec : regtype
register x19_spec : regtype
register x20_spec : regtype
register x21_spec : regtype
register x22_spec : regtype
register x23_spec : regtype
register x24_spec : regtype
register x25_spec : regtype
register x26_spec : regtype
register x27_spec : regtype
register x28_spec : regtype
register x29_spec : regtype
register x30_spec : regtype
register x31_spec : regtype

/* branch prediction speculation flag */
register in_speculation : bool = false

function rX (r : regno) -> xlenbits = {
  let v : regtype =
    if in_speculation then
      match r {
        0 => zero_reg,
        1 => x1_spec,
        2 => x2_spec,
        3 => x3_spec,
        4 => x4_spec,
        5 => x5_spec,
        6 => x6_spec,
        7 => x7_spec,
        8 => x8_spec,
        9 => x9_spec,
        10 => x10_spec,
        11 => x11_spec,
        12 => x12_spec,
        13 => x13_spec,
        14 => x14_spec,
        15 => x15_spec,
        16 => x16_spec,
        17 => x17_spec,
        18 => x18_spec,
        19 => x19_spec,
        20 => x20_spec,
        21 => x21_spec,
        22 => x22_spec,
        23 => x23_spec,
        24 => x24_spec,
        25 => x25_spec,
        26 => x26_spec,
        27 => x27_spec,
        28 => x28_spec,
        29 => x29_spec,
        30 => x30_spec,
        31 => x31_spec,
        _  => {assert(false, "invalid register number"); zero_reg}
      }
    else
      match r {
        0 => zero_reg,
        1 => x1,
        2 => x2,
        3 => x3,
        4 => x4,
        5 => x5,
        6 => x6,
        7 => x7,
        8 => x8,
        9 => x9,
        10 => x10,
        11 => x11,
        12 => x12,
        13 => x13,
        14 => x14,
        15 => x15,
        16 => x16,
        17 => x17,
        18 => x18,
        19 => x19,
        20 => x20,
        21 => x21,
        22 => x22,
        23 => x23,
        24 => x24,
        25 => x25,
        26 => x26,
        27 => x27,
        28 => x28,
        29 => x29,
        30 => x30,
        31 => x31,
        _  => {assert(false, "invalid register number"); zero_reg}
      };
  regval_from_reg(v)
}

$ifdef RVFI_DII
function rvfi_wX (r : regno, v : xlenbits) -> unit = {
  rvfi_int_data[rvfi_rd_wdata] = zero_extend(v);
  rvfi_int_data[rvfi_rd_addr] = to_bits(8,r);
  rvfi_int_data_present = true;
}
$else
function rvfi_wX (r : regno, v : xlenbits) -> unit = ()
$endif

function wX (r : regno, in_v : xlenbits) -> unit = {
  let v = regval_into_reg(in_v);
  if in_speculation then {
    match r {
      0  => (),
      1  => x1_spec = v,
      2  => x2_spec = v,
      3  => x3_spec = v,
      4  => x4_spec = v,
      5  => x5_spec = v,
      6  => x6_spec = v,
      7  => x7_spec = v,
      8  => x8_spec = v,
      9  => x9_spec = v,
      10 => x10_spec = v,
      11 => x11_spec = v,
      12 => x12_spec = v,
      13 => x13_spec = v,
      14 => x14_spec = v,
      15 => x15_spec = v,
      16 => x16_spec = v,
      17 => x17_spec = v,
      18 => x18_spec = v,
      19 => x19_spec = v,
      20 => x20_spec = v,
      21 => x21_spec = v,
      22 => x22_spec = v,
      23 => x23_spec = v,
      24 => x24_spec = v,
      25 => x25_spec = v,
      26 => x26_spec = v,
      27 => x27_spec = v,
      28 => x28_spec = v,
      29 => x29_spec = v,
      30 => x30_spec = v,
      31 => x31_spec = v,
      _  => assert(false, "invalid register number")
    }
  } else {
    match r {
      0  => (),
      1  => x1 = v,
      2  => x2 = v,
      3  => x3 = v,
      4  => x4 = v,
      5  => x5 = v,
      6  => x6 = v,
      7  => x7 = v,
      8  => x8 = v,
      9  => x9 = v,
      10 => x10 = v,
      11 => x11 = v,
      12 => x12 = v,
      13 => x13 = v,
      14 => x14 = v,
      15 => x15 = v,
      16 => x16 = v,
      17 => x17 = v,
      18 => x18 = v,
      19 => x19 = v,
      20 => x20 = v,
      21 => x21 = v,
      22 => x22 = v,
      23 => x23 = v,
      24 => x24 = v,
      25 => x25 = v,
      26 => x26 = v,
      27 => x27 = v,
      28 => x28 = v,
      29 => x29 = v,
      30 => x30 = v,
      31 => x31 = v,
      _  => assert(false, "invalid register number")
    };
    if (r != 0) then {
      rvfi_wX(r, in_v);
      if   get_config_print_reg()
      then print_reg("x" ^ dec_str(r) ^ " <- " ^ RegStr(v));
    }
  }
}

function rX_bits(i: regidx) -> xlenbits = rX(unsigned(i))

function wX_bits(i: regidx, data: xlenbits) -> unit = {
  wX(unsigned(i)) = data
}

overload X = {rX_bits, wX_bits, rX, wX}

/* mappings for assembly */

val reg_name : regidx <-> string
mapping reg_name = {
    0b00000 <-> "zero",
    0b00001 <-> "ra",
    0b00010 <-> "sp",
    0b00011 <-> "gp",
    0b00100 <-> "tp",
    0b00101 <-> "t0",
    0b00110 <-> "t1",
    0b00111 <-> "t2",
    0b01000 <-> "fp",
    0b01001 <-> "s1",
    0b01010 <-> "a0",
    0b01011 <-> "a1",
    0b01100 <-> "a2",
    0b01101 <-> "a3",
    0b01110 <-> "a4",
    0b01111 <-> "a5",
    0b10000 <-> "a6",
    0b10001 <-> "a7",
    0b10010 <-> "s2",
    0b10011 <-> "s3",
    0b10100 <-> "s4",
    0b10101 <-> "s5",
    0b10110 <-> "s6",
    0b10111 <-> "s7",
    0b11000 <-> "s8",
    0b11001 <-> "s9",
    0b11010 <-> "s10",
    0b11011 <-> "s11",
    0b11100 <-> "t3",
    0b11101 <-> "t4",
    0b11110 <-> "t5",
    0b11111 <-> "t6"
}

mapping creg_name : cregidx <-> string = {
  0b000 <-> "s0",
  0b001 <-> "s1",
  0b010 <-> "a0",
  0b011 <-> "a1",
  0b100 <-> "a2",
  0b101 <-> "a3",
  0b110 <-> "a4",
  0b111 <-> "a5"
}

val init_base_regs : unit -> unit
function init_base_regs () = {
  x1  = zero_reg;
  x2  = zero_reg;
  x3  = zero_reg;
  x4  = zero_reg;
  x5  = zero_reg;
  x6  = zero_reg;
  x7  = zero_reg;
  x8  = zero_reg;
  x9  = zero_reg;
  x10 = zero_reg;
  x11 = zero_reg;
  x12 = zero_reg;
  x13 = zero_reg;
  x14 = zero_reg;
  x15 = zero_reg;
  x16 = zero_reg;
  x17 = zero_reg;
  x18 = zero_reg;
  x19 = zero_reg;
  x20 = zero_reg;
  x21 = zero_reg;
  x22 = zero_reg;
  x23 = zero_reg;
  x24 = zero_reg;
  x25 = zero_reg;
  x26 = zero_reg;
  x27 = zero_reg;
  x28 = zero_reg;
  x29 = zero_reg;
  x30 = zero_reg;
  x31 = zero_reg
}

function start_speculation() -> unit = {
  // 保存當前狀態到 spec registers
  x1_spec = x1;
  x2_spec = x2;
  x3_spec = x3;
  x4_spec = x4;
  x5_spec = x5;
  x6_spec = x6;
  x7_spec = x7;
  x8_spec = x8;
  x9_spec = x9;
  x10_spec = x10;
  x11_spec = x11;
  x12_spec = x12;
  x13_spec = x13;
  x14_spec = x14;
  x15_spec = x15;
  x16_spec = x16;
  x17_spec = x17;
  x18_spec = x18;
  x19_spec = x19;
  x20_spec = x20;
  x21_spec = x21;
  x22_spec = x22;
  x23_spec = x23;
  x24_spec = x24;
  x25_spec = x25;
  x26_spec = x26;
  x27_spec = x27;
  x28_spec = x28;
  x29_spec = x29;
  x30_spec = x30;
  x31_spec = x31;
  in_speculation = true;
}

function commit_speculation() -> unit = {
  // 將 spec registers 複製回主寄存器
  x1 = x1_spec;
  x2 = x2_spec;
  x3 = x3_spec;
  x4 = x4_spec;
  x5 = x5_spec;
  x6 = x6_spec;
  x7 = x7_spec;
  x8 = x8_spec;
  x9 = x9_spec;
  x10 = x10_spec;
  x11 = x11_spec;
  x12 = x12_spec;
  x13 = x13_spec;
  x14 = x14_spec;
  x15 = x15_spec;
  x16 = x16_spec;
  x17 = x17_spec;
  x18 = x18_spec;
  x19 = x19_spec;
  x20 = x20_spec;
  x21 = x21_spec;
  x22 = x22_spec;
  x23 = x23_spec;
  x24 = x24_spec;
  x25 = x25_spec;
  x26 = x26_spec;
  x27 = x27_spec;
  x28 = x28_spec;
  x29 = x29_spec;
  x30 = x30_spec;
  x31 = x31_spec;
  in_speculation = false;
}

function squash_speculation() -> unit = {
  // 清除推測狀態
  in_speculation = false;
}
