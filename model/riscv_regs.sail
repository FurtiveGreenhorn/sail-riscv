/*=======================================================================================*/
/*  RISCV Sail Model                                                                     */
/*                                                                                       */
/*  This Sail RISC-V architecture model, comprising all files and                        */
/*  directories except for the snapshots of the Lem and Sail libraries                   */
/*  in the prover_snapshots directory (which include copies of their                     */
/*  licences), is subject to the BSD two-clause licence below.                           */
/*                                                                                       */
/*  Copyright (c) 2017-2023                                                              */
/*    Prashanth Mundkur                                                                  */
/*    Rishiyur S. Nikhil and Bluespec, Inc.                                              */
/*    Jon French                                                                         */
/*    Brian Campbell                                                                     */
/*    Robert Norton-Wright                                                               */
/*    Alasdair Armstrong                                                                 */
/*    Thomas Bauereiss                                                                   */
/*    Shaked Flur                                                                        */
/*    Christopher Pulte                                                                  */
/*    Peter Sewell                                                                       */
/*    Alexander Richardson                                                               */
/*    Hesham Almatary                                                                    */
/*    Jessica Clarke                                                                     */
/*    Microsoft, for contributions by Robert Norton-Wright and Nathaniel Wesley Filardo  */
/*    Peter Rugg                                                                         */
/*    Aril Computer Corp., for contributions by Scott Johnson                            */
/*    Philipp Tomsich                                                                    */
/*    VRULL GmbH, for contributions by its employees                                     */
/*                                                                                       */
/*  All rights reserved.                                                                 */
/*                                                                                       */
/*  This software was developed by the above within the Rigorous                         */
/*  Engineering of Mainstream Systems (REMS) project, partly funded by                   */
/*  EPSRC grant EP/K008528/1, at the Universities of Cambridge and                       */
/*  Edinburgh.                                                                           */
/*                                                                                       */
/*  This software was developed by SRI International and the University of               */
/*  Cambridge Computer Laboratory (Department of Computer Science and                    */
/*  Technology) under DARPA/AFRL contract FA8650-18-C-7809 ("CIFV"), and                 */
/*  under DARPA contract HR0011-18-C-0016 ("ECATS") as part of the DARPA                 */
/*  SSITH research programme.                                                            */
/*                                                                                       */
/*  This project has received funding from the European Research Council                 */
/*  (ERC) under the European Union’s Horizon 2020 research and innovation                */
/*  programme (grant agreement 789108, ELVER).                                           */
/*                                                                                       */
/*                                                                                       */
/*  Redistribution and use in source and binary forms, with or without                   */
/*  modification, are permitted provided that the following conditions                   */
/*  are met:                                                                             */
/*  1. Redistributions of source code must retain the above copyright                    */
/*     notice, this list of conditions and the following disclaimer.                     */
/*  2. Redistributions in binary form must reproduce the above copyright                 */
/*     notice, this list of conditions and the following disclaimer in                   */
/*     the documentation and/or other materials provided with the                        */
/*     distribution.                                                                     */
/*                                                                                       */
/*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''                   */
/*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED                    */
/*  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                      */
/*  PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR                  */
/*  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,                         */
/*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT                     */
/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF                     */
/*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND                  */
/*  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,                   */
/*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT                   */
/*  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF                   */
/*  SUCH DAMAGE.                                                                         */
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

/* for pipeline register */
register fReg    : FReg
register fRegNew : FReg
register dReg    : DReg
register dRegNew : DReg
register eReg    : EReg
register eRegNew : EReg
register mReg    : MReg
register mRegNew : MReg

register alu : bool
register multiplier  : bool
register divider  : bool
register mul_cycle  : int = 5 //cycle為n+1
register div_cycle  : int = 5 //cycle為n+1
register mul_cycle_count  : int = mul_cycle //cycle為n+1
register div_cycle_count  : int = div_cycle //cycle為n+1


register fu_forward : Fu = EMPTY //為了data forwarding需紀錄, 進入exe前紀錄使用哪個fu,並且需要forwarding,才會記錄
register exe_issue :  Fu = EMPTY //為了data forwarding需紀錄

register fu_order : vector(3, dec, Fu) = [EMPTY, EMPTY, EMPTY]
register aeReg_temp    : EReg
register meReg_temp    : EReg
register deReg_temp    : EReg

register executeWriteBack : bool = false
register executeWBReg : regtype = EXTS(0b0)
register memoryWriteBack : bool = false
register memoryWBReg : regtype = EXTS(0b0)
register memoryStallWriteBack : bool = false
register memoryStallWBReg : regtype = EXTS(0b0)

/* for pipeline register */


val rX : forall 'n, 0 <= 'n < 32. regno('n) -> xlenbits effect {rreg, escape}
function rX r = {
  let v : regtype =
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
val rvfi_wX : forall 'n, 0 <= 'n < 32. (regno('n), xlenbits) -> unit effect {wreg}
function rvfi_wX (r,v) = {
  rvfi_int_data->rvfi_rd_wdata() = EXTZ(v);
  rvfi_int_data->rvfi_rd_addr() = to_bits(8,r);
  rvfi_int_data_present = true;
}
$else
val rvfi_wX : forall 'n, 0 <= 'n < 32. (regno('n), xlenbits) -> unit
function rvfi_wX (r,v) = ()
$endif

val wX : forall 'n, 0 <= 'n < 32. (regno('n), xlenbits) -> unit effect {wreg, escape}
function wX (r, in_v) = {
  let v = regval_into_reg(in_v);
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
     then print_reg("x" ^ string_of_int(r) ^ " <- " ^ RegStr(v));
  }
}

function rX_bits(i: bits(5)) -> xlenbits = rX(unsigned(i))

function wX_bits(i: bits(5), data: xlenbits) -> unit = {
  wX(unsigned(i)) = data
}

overload X = {rX_bits, wX_bits, rX, wX}

/* register names */

val reg_name_abi : regidx -> string

function reg_name_abi(r) = {
  match (r) {
    0b00000 => "zero",
    0b00001 => "ra",
    0b00010 => "sp",
    0b00011 => "gp",
    0b00100 => "tp",
    0b00101 => "t0",
    0b00110 => "t1",
    0b00111 => "t2",
    0b01000 => "fp",
    0b01001 => "s1",
    0b01010 => "a0",
    0b01011 => "a1",
    0b01100 => "a2",
    0b01101 => "a3",
    0b01110 => "a4",
    0b01111 => "a5",
    0b10000 => "a6",
    0b10001 => "a7",
    0b10010 => "s2",
    0b10011 => "s3",
    0b10100 => "s4",
    0b10101 => "s5",
    0b10110 => "s6",
    0b10111 => "s7",
    0b11000 => "s8",
    0b11001 => "s9",
    0b11010 => "s10",
    0b11011 => "s11",
    0b11100 => "t3",
    0b11101 => "t4",
    0b11110 => "t5",
    0b11111 => "t6"
  }
}

overload to_str = {reg_name_abi}

/* mappings for assembly */

val reg_name : bits(5) <-> string
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

mapping creg_name : bits(3) <-> string = {
  0b000 <-> "s0",
  0b001 <-> "s1",
  0b010 <-> "a0",
  0b011 <-> "a1",
  0b100 <-> "a2",
  0b101 <-> "a3",
  0b110 <-> "a4",
  0b111 <-> "a5"
}

val init_base_regs : unit -> unit effect {wreg}
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




val init_pipeline_regs : unit -> unit 
function init_pipeline_regs () = {
  fReg.bubble = false;
  fReg.stall = 0;
  fReg.pc = EXTZ(0b0);
  fReg.inst = EXTZ(0b0);
  fReg.len = 0;

  fRegNew.bubble = false;
  fRegNew.stall = 0;
  fRegNew.pc = EXTZ(0b0);
  fRegNew.inst = EXTZ(0b0);
  fRegNew.len = 0;
//-----------------------------------
  dReg.bubble = false;
  dReg.stall = 0;
  dReg.pc = EXTZ(0b0);
  dReg.inst = EXTZ(0b0);
  dReg.rs1 = EXTZ(0b0);
  dReg.rs2 = EXTZ(0b0);
  dReg.rd = EXTZ(0b0);

  dReg.op1 = EXTZ(0b0);
  dReg.op2 = EXTZ(0b0);
  dReg.predictedBranch = false;
  dReg.predictedPC = EXTZ(0b0);
  dReg.anotherPC = EXTZ(0b0);

  dRegNew.bubble = false;
  dRegNew.stall = 0;
  dRegNew.pc = EXTZ(0b0);
  dRegNew.inst = EXTZ(0b0);
  dRegNew.rs1 = EXTZ(0b0);
  dRegNew.rs2 = EXTZ(0b0);
  dRegNew.rd = EXTZ(0b0);

  dRegNew.op1 = EXTZ(0b0);
  dRegNew.op2 = EXTZ(0b0);
  dRegNew.predictedBranch = false;
  dRegNew.predictedPC = EXTZ(0b0);
  dRegNew.anotherPC = EXTZ(0b0);
//-----------------------------------
  eReg.bubble = false;
  eReg.stall = 0;
  eReg.pc = EXTZ(0b0);
  eReg.inst = EXTZ(0b0);
  eReg.op1 = EXTZ(0b0);
  eReg.op2 = EXTZ(0b0);
  eReg.writeReg = false;
  eReg.destReg = EXTZ(0b0);
  eReg.out = EXTZ(0b0);
  eReg.writeMem = false;
  eReg.readMem = false;
  eReg.readSignExt = false;
  eReg.memLen = BYTE;
  eReg.branch = false;

  eRegNew.bubble = false;
  eRegNew.stall = 0;
  eRegNew.pc = EXTZ(0b0);
  eRegNew.inst = EXTZ(0b0);
  eRegNew.op1 = EXTZ(0b0);
  eRegNew.op2 = EXTZ(0b0);
  eRegNew.writeReg = false;
  eRegNew.destReg = EXTZ(0b0);
  eRegNew.out = EXTZ(0b0);
  eRegNew.writeMem = false;
  eRegNew.readMem = false;
  eRegNew.readSignExt = false;
  eRegNew.memLen = BYTE;
  eRegNew.branch = false;
//-----------------------------------
  aeReg_temp.bubble = false;
  aeReg_temp.stall = 0;
  aeReg_temp.pc = EXTZ(0b0);
  aeReg_temp.inst = EXTZ(0b0);
  aeReg_temp.op1 = EXTZ(0b0);
  aeReg_temp.op2 = EXTZ(0b0);
  aeReg_temp.writeReg = false;
  aeReg_temp.destReg = EXTZ(0b0);
  aeReg_temp.out = EXTZ(0b0);
  aeReg_temp.writeMem = false;
  aeReg_temp.readMem = false;
  aeReg_temp.readSignExt = false;
  aeReg_temp.memLen = BYTE;
  aeReg_temp.branch = false;
//-----------------------------------
  meReg_temp.bubble = false;
  meReg_temp.stall = 0;
  meReg_temp.pc = EXTZ(0b0);
  meReg_temp.inst = EXTZ(0b0);
  meReg_temp.op1 = EXTZ(0b0);
  meReg_temp.op2 = EXTZ(0b0);
  meReg_temp.writeReg = false;
  meReg_temp.destReg = EXTZ(0b0);
  meReg_temp.out = EXTZ(0b0);
  meReg_temp.writeMem = false;
  meReg_temp.readMem = false;
  meReg_temp.readSignExt = false;
  meReg_temp.memLen = BYTE;
  meReg_temp.branch = false;
//-----------------------------------
  deReg_temp.bubble = false;
  deReg_temp.stall = 0;
  deReg_temp.pc = EXTZ(0b0);
  deReg_temp.inst = EXTZ(0b0);
  deReg_temp.op1 = EXTZ(0b0);
  deReg_temp.op2 = EXTZ(0b0);
  deReg_temp.writeReg = false;
  deReg_temp.destReg = EXTZ(0b0);
  deReg_temp.out = EXTZ(0b0);
  deReg_temp.writeMem = false;
  deReg_temp.readMem = false;
  deReg_temp.readSignExt = false;
  deReg_temp.memLen = BYTE;
  deReg_temp.branch = false;
//-----------------------------------

  mReg.bubble = false;
  mReg.stall = 0;
  mReg.pc = EXTZ(0b0);
  mReg.inst = EXTZ(0b0);
  mReg.op1 = EXTZ(0b0);
  mReg.op2 = EXTZ(0b0);
  mReg.out = EXTZ(0b0);
  mReg.writeReg = false;
  mReg.readMem = false;
  mReg.destReg = EXTZ(0b0);

  mRegNew.bubble = false;
  mRegNew.stall = 0;
  mRegNew.pc = EXTZ(0b0);
  mRegNew.inst = EXTZ(0b0);
  mRegNew.op1 = EXTZ(0b0);
  mRegNew.op2 = EXTZ(0b0);
  mRegNew.out = EXTZ(0b0);
  mRegNew.writeReg = false;
  mRegNew.readMem = false;
  mRegNew.destReg = EXTZ(0b0);
}


val init_pipeline_regNews : unit -> unit 
function init_pipeline_regNews () = {
  fRegNew.bubble = false;
  fRegNew.stall = 0;
  fRegNew.pc = EXTZ(0b0);
  fRegNew.inst = EXTZ(0b0);
  fRegNew.len = 0;
//-----------------------------------
  dRegNew.bubble = false;
  dRegNew.stall = 0;
  dRegNew.pc = EXTZ(0b0);
  dRegNew.inst = EXTZ(0b0);
  dRegNew.rs1 = EXTZ(0b0);
  dRegNew.rs2 = EXTZ(0b0);
  dRegNew.op1 = EXTZ(0b0);
  dRegNew.op2 = EXTZ(0b0);
  dRegNew.predictedBranch = false;
  dRegNew.predictedPC = EXTZ(0b0);
  dRegNew.anotherPC = EXTZ(0b0);
//-----------------------------------
  eRegNew.bubble = false;
  eRegNew.stall = 0;
  eRegNew.pc = EXTZ(0b0);
  eRegNew.inst = EXTZ(0b0);
  eRegNew.op1 = EXTZ(0b0);
  eRegNew.op2 = EXTZ(0b0);
  eRegNew.writeReg = false;
  eRegNew.destReg = EXTZ(0b0);
  eRegNew.out = EXTZ(0b0);
  eRegNew.writeMem = false;
  eRegNew.readMem = false;
  eRegNew.readSignExt = false;
  eRegNew.memLen = BYTE;
  eRegNew.branch = false;
//-----------------------------------
  mRegNew.bubble = false;
  mRegNew.stall = 0;
  mRegNew.pc = EXTZ(0b0);
  mRegNew.inst = EXTZ(0b0);
  mRegNew.op1 = EXTZ(0b0);
  mRegNew.op2 = EXTZ(0b0);
  mRegNew.out = EXTZ(0b0);
  mRegNew.writeReg = false;
  mRegNew.readMem = false;
  mRegNew.destReg = EXTZ(0b0);
}

val init_pipeline_fu : unit -> unit 
function init_pipeline_fu () = {
  alu = false;
  multiplier = false;
  divider = false;
}

val print_cpuState : unit -> unit 

function print_cpuState() = {
  print("==================== CPU STATE ====================");

  print("PC:" ^ BitStr(PC));
  print("zero: " ^ BitStr(rX_bits(0b00000)) ^ " ra: " ^ BitStr(rX_bits(0b00001)) ^ " sp:  " ^ BitStr(rX_bits(0b00010)) ^ " gp:  " ^ BitStr(rX_bits(0b00011)));
  print("tp:   " ^ BitStr(rX_bits(0b00100)) ^ " t0: " ^ BitStr(rX_bits(0b00101)) ^ " t1:  " ^ BitStr(rX_bits(0b00110)) ^ " t2:  " ^ BitStr(rX_bits(0b00111)));
  print("s0:   " ^ BitStr(rX_bits(0b01000)) ^ " s1: " ^ BitStr(rX_bits(0b01001)) ^ " a0:  " ^ BitStr(rX_bits(0b01010)) ^ " a1:  " ^ BitStr(rX_bits(0b01011)));
  print("a2:   " ^ BitStr(rX_bits(0b01100)) ^ " a3: " ^ BitStr(rX_bits(0b01101)) ^ " a4:  " ^ BitStr(rX_bits(0b01110)) ^ " a5:  " ^ BitStr(rX_bits(0b01111)));
  print("a6:   " ^ BitStr(rX_bits(0b10000)) ^ " a7: " ^ BitStr(rX_bits(0b10001)) ^ " s2:  " ^ BitStr(rX_bits(0b10010)) ^ " s3:  " ^ BitStr(rX_bits(0b10011)));
  print("s4:   " ^ BitStr(rX_bits(0b10100)) ^ " s5: " ^ BitStr(rX_bits(0b10101)) ^ " s6:  " ^ BitStr(rX_bits(0b10110)) ^ " s7:  " ^ BitStr(rX_bits(0b10111)));
  print("s8:   " ^ BitStr(rX_bits(0b11000)) ^ " s9: " ^ BitStr(rX_bits(0b11001)) ^ " s10: " ^ BitStr(rX_bits(0b11010)) ^ " s11: " ^ BitStr(rX_bits(0b11011)));
  print("t3:   " ^ BitStr(rX_bits(0b11100)) ^ " t4: " ^ BitStr(rX_bits(0b11101)) ^ " t5:  " ^ BitStr(rX_bits(0b11110)) ^ " t6:  " ^ BitStr(rX_bits(0b11111)));
  print("===================================================");
  print("");
}

