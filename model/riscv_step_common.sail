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

/* The result of a fetch, which includes any possible error
 * from an extension that interposes on the fetch operation.
 */

union FetchResult = {
  F_Ext_Error : ext_fetch_addr_error,      /* For extensions */
  F_Base      : word,                      /* Base ISA */
  F_RVC       : half,                      /* Compressed ISA */
  F_Error     : (ExceptionType, xlenbits)  /* standard exception and PC */
}


/* for pipeline */ 
//　for forward
val setDregNew : ast -> unit 
function setDregNew (ast) = {
  match ast {
    UTYPE(imm, rd, op) => {
      dRegNew.rd = EXTZ(rd);
    },
    
    RISCV_JAL(imm, rd) => {
      ()
    },

    RISCV_JALR(imm, rs1, rd) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
    },

    BTYPE(imm, rs2, rs1, op)  => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    },

    ITYPE (imm, rs1, rd, op) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
    },

    SHIFTIOP(shamt, rs1, rd, op) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
    },

    RTYPE(rs2, rs1, rd, op) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    },

    LOAD(imm, rs1, rd, is_unsigned, width, aq, rl) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
    },

    STORE(imm, rs2, rs1, width, aq, rl) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    },

    ADDIW(imm, rs1, rd) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
    },

    SHIFTW(shamt, rs1, rd, op) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
    },

    RTYPEW(rs2, rs1, rd, op) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    },

    SHIFTIWOP(shamt, rs1, rd, op) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);

    }, 
    
    MUL(rs2, rs1, rd, high, signed1, signed2) => {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    }, 

    DIV(rs2, rs1, rd, s)=> {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    }, 

    REM(rs2, rs1, rd, s)=> {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    }, 

    MULW(rs2, rs1, rd)=> {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    }, 

    DIVW(rs2, rs1, rd, s)=> {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    }, 

    REMW(rs2, rs1, rd, s)=> {
      dRegNew.rs1 = EXTZ(rs1);
      dRegNew.rs2 = EXTZ(rs2);
      dRegNew.rd = EXTZ(rd);
      dRegNew.op1 = X(rs1);
      dRegNew.op2 = X(rs2);
    }, 

    _ => {
      ();
    }
  }; 
} 

val isReadMem : ast -> bool
function isReadMem(ast) = {
    match ast {
      LOAD(imm, rs1, rd, is_unsigned, width, aq, rl) => {
        true
      },

      _ => {
        false
      }
    }
}

val isRunAlu : ast -> bool
function isRunAlu(ast) = {
    match ast {
      UTYPE(imm, rd, op) => {
        true
      },
    
      RISCV_JAL(imm, rd) => {
        true
      },

      RISCV_JALR(imm, rs1, rd) => {
        true
      },

      BTYPE(imm, rs2, rs1, op)  => {
        true
      },

      ITYPE (imm, rs1, rd, op) => {
        true
      },

      SHIFTIOP(shamt, rs1, rd, op) => {
        true
      },

      RTYPE(rs2, rs1, rd, op) => {
        true
      },

      LOAD(imm, rs1, rd, is_unsigned, width, aq, rl) => {
        true
      },

      STORE(imm, rs2, rs1, width, aq, rl) => {
        true
      },

      ADDIW(imm, rs1, rd) => {
        true
      },

      SHIFTW(shamt, rs1, rd, op) => {
        true
      },

      RTYPEW(rs2, rs1, rd, op) => {
        true
      },

      SHIFTIWOP(shamt, rs1, rd, op) => {
        true
      }, 
      _ => {
        false
      }
    }
}

val isRunMul : ast -> bool
function isRunMul(ast) = {
    match ast {
      MUL(rs2, rs1, rd, high, signed1, signed2) => {
        true
      },
      MULW(rs2, rs1, rd)=> {
        true
      },
      _ => {
        false
      }
    }
}

val isRunDiv : ast -> bool
function isRunDiv(ast) = {
    match ast {
      DIV(rs2, rs1, rd, s)=> {
        true
     }, 

     REM(rs2, rs1, rd, s)=> {
        true
     }, 

     DIVW(rs2, rs1, rd, s)=> {
        true 
     }, 

     REMW(rs2, rs1, rd, s)=> {
        true
     }, 
     
     _ => {
        false
     }
    }
}


val itype_writereg_check : ast -> bool 
function itype_writereg_check(ast) -> bool = {
    match ast {
      UTYPE(imm, rd, op) => {
        true
      },
    
      ITYPE (imm, rs1, rd, op) => {
        true
      },

      SHIFTIOP(shamt, rs1, rd, op) => {
        true
      },

      RTYPE(rs2, rs1, rd, op) => {
        true
      },

      LOAD(imm, rs1, rd, is_unsigned, width, aq, rl) => {
        true
      },

      ADDIW(imm, rs1, rd) => {
        true
      },

      SHIFTW(shamt, rs1, rd, op) => {
        true
      },

      RTYPEW(rs2, rs1, rd, op) => {
        true
      },

      SHIFTIWOP(shamt, rs1, rd, op) => {
        true
      }, 
    
      _ => {
        false
      }
    }
}

val aluAddStall : unit -> unit 
function aluAddStall() -> unit = {

      //mul_cycle or div_cycle 在這邊等於n 代表還會在exe待n+1 cycle ,還有alu這邊做issue 所以都是+2cycle
      if multiplier & divider 
      then {
        if fu_order[2] == ALU
        then {
          if (mul_cycle_count > div_cycle_count) & (fRegNew.stall < (mul_cycle_count+2))  
          then {
            fRegNew.stall = mul_cycle_count+2;
            dRegNew.stall = mul_cycle_count+2;
          }
          else if (mul_cycle_count < div_cycle_count) & (fRegNew.stall < (div_cycle_count+2))  
          then {
            fRegNew.stall = div_cycle_count+2;
            dRegNew.stall = div_cycle_count+2;
          }
        }
        else if fu_order[1] == ALU
        then {

          if (fu_order[0] == DIVIDER) & (fRegNew.stall < (div_cycle_count+2))  
          then {
            fRegNew.stall = div_cycle_count+2;
            dRegNew.stall = div_cycle_count+2;
          }
          else if (fu_order[0] == MULTIPLIER) & (fRegNew.stall < (mul_cycle_count+2))  
          then {
            fRegNew.stall = mul_cycle_count+2;
            dRegNew.stall = mul_cycle_count+2;
          }
        }
      }

      else if multiplier & (fRegNew.stall < (mul_cycle_count+2)) & fu_order[0] != ALU
      then {
          fRegNew.stall = mul_cycle_count+2;
          dRegNew.stall = mul_cycle_count+2;
      }
      else if divider & (fRegNew.stall < (div_cycle_count+2)) & fu_order[0] != ALU
      then {
          fRegNew.stall = div_cycle_count+2;
          dRegNew.stall = div_cycle_count+2;
      };  
}

val execute_check_fu : unit -> unit 
function execute_check_fu() -> unit = {
  /*
  print_int("fRegNew.stall = ", fRegNew.stall);
  print_int("dRegNew.stall = ", dRegNew.stall);
  print_int("fReg.stall = ", fReg.stall);
  print_int("dReg.stall = ", dReg.stall);
  print_int("mul_cycle_count = ", mul_cycle_count);
    print_int("div_cycle_count: " , div_cycle_count);
*/
  //先減去各個fu的cycle
  if multiplier 
  then {
    if mul_cycle_count > 0
    then mul_cycle_count = mul_cycle_count - 1;
    
    if get_config_print_pipeline() 
    then print("Multiplier executing");
  };
  if divider 
  then {
    if div_cycle_count > 0
    then div_cycle_count = div_cycle_count - 1;

    if get_config_print_pipeline() 
    then print("Divider executing");
  };
  if alu & get_config_print_pipeline()
  then {
    print("Alu executing");
  };
  //如果不再這裡檢查fu 是否運作 會有指令偷溜
  //檢查stall和bubble　用意為如果上個階段為空　這階段也不要執行
  if isRunMul(dast) & dReg.stall == 0 & dReg.bubble != true
  then {
    multiplier = true;
  };

  if isRunDiv(dast) & dReg.stall == 0 & dReg.bubble != true
  then {
    divider = true;
  };

  if isRunAlu(dast) & dReg.stall == 0 & dReg.bubble != true
  then {
    alu = true;
  };

  
  if multiplier
  then {    
    if isRunMul(dastNew) & (fRegNew.stall < mul_cycle_count)
    then {
      fRegNew.stall = mul_cycle_count;
      dRegNew.stall = mul_cycle_count;
    };
  };

  if divider
  then {
    if isRunDiv(dastNew) & (fRegNew.stall < div_cycle_count)
    then {
      fRegNew.stall = div_cycle_count; 
      dRegNew.stall = div_cycle_count;
    };
  };

  if alu
  then {
    if isRunAlu(dastNew) 
    then {
      aluAddStall();
    };
  }
}



val execute_check : ast -> bool 
function execute_check(ast) -> bool = { //檢查是否為 stall or bubble
  //print_int("Stall_count = ", dReg.stall);

  empty_flag : bool = true;

  if dReg.stall > 0
  then {
    if   get_config_print_pipeline()
    then print("Execute: Stall");
    //eRegNew.bubble = true;
    return true
  };
    
  if dReg.bubble 
  then{
    if   get_config_print_pipeline()
    then print("Execute: Bubble");
    //eRegNew.bubble = true;
    return true
  };

  match ast {
    MUL(rs2, rs1, rd, high, signed1, signed2) as ast => {
     
      //multiplier = true; //代表mlu正在使用
      mdast = ast; //為了issue傳遞給memory access
      
      if dRegNew.rs1 == dReg.rd
      then {
          fu_forward = MULTIPLIER;
          fRegNew.stall = mul_cycle;
          dRegNew.stall = mul_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };
      if dRegNew.rs2 == dReg.rd 
      then {
          fu_forward = MULTIPLIER;
          fRegNew.stall = mul_cycle;
          dRegNew.stall = mul_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };

      foreach (i from 0 to 2) {
        if fu_order[i] == EMPTY & empty_flag
        then { 
          fu_order[i] = MULTIPLIER;
          empty_flag = false;
        }
      };


    },
     MULW(rs2, rs1, rd) as ast=> {
     
      //multiplier = true;
      mdast = ast; //為了issue傳遞給memory access

      if dRegNew.rs1 == dReg.rd
      then {
          fu_forward = MULTIPLIER;
          fRegNew.stall = mul_cycle;
          dRegNew.stall = mul_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };
      if dRegNew.rs2 == dReg.rd 
      then {
          fu_forward = MULTIPLIER;
          fRegNew.stall = mul_cycle;
          dRegNew.stall = mul_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };

      foreach (i from 0 to 2) {
        if fu_order[i] == EMPTY & empty_flag
        then { 
          fu_order[i] = MULTIPLIER;
          empty_flag = false;

        }
      };     
      
     },
     DIV(rs2, rs1, rd, s) as ast=> {
      
      //divider = true;
      ddast = ast; //為了issue傳遞給memory access

      if dRegNew.rs1 == dReg.rd
      then {
          fu_forward = DIVIDER;
          fRegNew.stall = div_cycle;
          dRegNew.stall = div_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };
      if dRegNew.rs2 == dReg.rd 
      then {
          fu_forward = DIVIDER;
          fRegNew.stall = div_cycle;
          dRegNew.stall = div_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };

      foreach (i from 0 to 2) {
        if fu_order[i] == EMPTY & empty_flag
        then {
          fu_order[i] = DIVIDER;
          empty_flag = false;

        }
      };  
      
     },
     REM(rs2, rs1, rd, s) as ast=> {
      
      //divider = true;
      ddast = ast; //為了issue傳遞給memory access

      //print("dRegNew.rs1 = " ^ BitStr(dRegNew.rs1) ^ ", dReg.rd = " ^ BitStr(dReg.rd));

      if dRegNew.rs1 == dReg.rd
      then {
          fu_forward = DIVIDER;
          fRegNew.stall = div_cycle;
          dRegNew.stall = div_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };
      if dRegNew.rs2 == dReg.rd 
      then {
          fu_forward = DIVIDER;
          fRegNew.stall = div_cycle;
          dRegNew.stall = div_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };
      
      foreach (i from 0 to 2) {
        if fu_order[i] == EMPTY & empty_flag 
        then {
          fu_order[i] = DIVIDER;
          empty_flag = false;
        }
      };   
      
     },
     DIVW(rs2, rs1, rd, s) as ast=> {
      
      //divider = true;
      ddast = ast; //為了issue傳遞給memory access

      if dRegNew.rs1 == dReg.rd
      then {
          fu_forward = DIVIDER;
          fRegNew.stall = div_cycle;
          dRegNew.stall = div_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd

      };
      if dRegNew.rs2 == dReg.rd 
      then {
          fu_forward = DIVIDER;
          fRegNew.stall = div_cycle;
          dRegNew.stall = div_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };
      
      foreach (i from 0 to 2) {
        if fu_order[i] == EMPTY & empty_flag
        then {
          fu_order[i] = DIVIDER;
          empty_flag = false;
        }
      };  
      
     },
     REMW(rs2, rs1, rd, s) as ast=> {
    
      //divider = true;
      ddast = ast; //為了issue傳遞給memory access

      if dRegNew.rs1 == dReg.rd
      then {
          fu_forward = DIVIDER;
          fRegNew.stall = div_cycle;
          dRegNew.stall = div_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd

      };
      if dRegNew.rs2 == dReg.rd 
      then {
          fu_forward = DIVIDER;
          fRegNew.stall = div_cycle;
          dRegNew.stall = div_cycle;
          executeWriteBack = true;
          executeWBReg = dReg.rd
      };
      
      foreach (i from 0 to 2) {
        if fu_order[i] == EMPTY & empty_flag
        then {
          fu_order[i] = DIVIDER;
          empty_flag = false;
        }
      };  
      
     },
     ast => {
      
      //alu = true;
      adast = ast; //為了issue傳遞給memory access

      if itype_writereg_check(ast)
      then {
        if dRegNew.rs1 == dReg.rd
        then {
          //history.cycleCount = history.cycleCount - 1;
          history.memoryHazardCount = history.memoryHazardCount + 1;
          fu_forward = ALU;
          executeWriteBack = true;
          executeWBReg = dReg.rd;
          aluAddStall(); 
        };

        if dRegNew.rs2 == dReg.rd 
        then {
          //history.cycleCount = history.cycleCount - 1;
          history.memoryHazardCount = history.memoryHazardCount + 1;
          fu_forward = ALU;
          executeWriteBack = true;
          executeWBReg = dReg.rd;
          aluAddStall();
        };
      };
      
      foreach (i from 0 to 2) {
        if fu_order[i] == EMPTY & empty_flag
        then {
          //print_int("check: fu_order: i = ", i);
          fu_order[i] = ALU;
          empty_flag = false;
        }
      }; 
     }
  };

  //================================================================確認decode, fetch是否需要stall
      
 /*
  if alu then   print("alu = true")
  else  print("alu = false");

  if multiplier then   print("multiplier = true")
  else  print("multiplier = false");
  if divider then   print("divider = true")
  else  print("divider = false");
  
  print_int("fRegNew.stall: " , fRegNew.stall);
  print_int("dRegNew.stall: " , dRegNew.stall);
  print_int("mul_cycle_count: " , mul_cycle_count);
  print_int("div_cycle_count: " , div_cycle_count);
*/
  
//================================================================
  /*
  if fu_forward == EMPTY
  then print("fu_forward = EMPTY");
  if fu_forward == ALU
  then print("fu_forward = ALU");
  if fu_forward == MULTIPLIER
  then print("fu_forward = MULTIPLIER");
  if fu_forward == DIVIDER
  then print("fu_forward = DIVIDER");
  */
  false
}

val execute_issue : unit -> unit 
function execute_issue() -> unit = {
  foreach (i from 0 to 2) {
    //print_int("issue: fu_order: i = ", i);

    if fu_order[i] != EMPTY
    then {
      
      //print_int("not empty: fu_order: i = ", i);
      //print_int("not empty: div_cycle_count = ", div_cycle_count);
      

      if fu_order[i] == MULTIPLIER & mul_cycle_count == 0
      then {
        mul_cycle_count = mul_cycle;
        multiplier = false;
        exe_issue = MULTIPLIER;
        //(pc, op1, op2, writeReg, destReg, out, writeMem , readMem, readSignExt, memLen, branch)
        setEregNew(meReg_temp.pc, meReg_temp.op1, meReg_temp.op2, meReg_temp.writeReg, meReg_temp.destReg, meReg_temp.out, meReg_temp.writeMem, meReg_temp.readMem, meReg_temp.readSignExt, meReg_temp.memLen, meReg_temp.branch);    
        foreach (j from i to 1) {
          fu_order[j] = fu_order[j + 1];
        };
        fu_order[2] = EMPTY;
      } 
      else if fu_order[i] == DIVIDER & div_cycle_count == 0 
      then {
        //print("DIVIDER SUSS");

        div_cycle_count = div_cycle;
        exe_issue = DIVIDER;
        divider = false;
        //(pc, op1, op2, writeReg, destReg, out, writeMem , readMem, readSignExt, memLen, branch)
        setEregNew(deReg_temp.pc, deReg_temp.op1, deReg_temp.op2, deReg_temp.writeReg, deReg_temp.destReg, deReg_temp.out, deReg_temp.writeMem, deReg_temp.readMem, deReg_temp.readSignExt, deReg_temp.memLen, deReg_temp.branch);
        foreach (j from i to 1) {
          fu_order[j] = fu_order[j + 1];
        };
        fu_order[2] = EMPTY;
      }
      else if fu_order[i] == ALU 
      then {
        exe_issue = ALU;
        alu = false;
//        print_int("issue: fu_order: i = ", i);

        //(pc, op1, op2, writeReg, destReg, out, writeMem , readMem, readSignExt, memLen, branch)
        setEregNew(aeReg_temp.pc, aeReg_temp.op1, aeReg_temp.op2, aeReg_temp.writeReg, aeReg_temp.destReg, aeReg_temp.out, aeReg_temp.writeMem, aeReg_temp.readMem, aeReg_temp.readSignExt, aeReg_temp.memLen, aeReg_temp.branch);
        foreach (j from i to 1) {
          fu_order[j] = fu_order[j + 1];
          //print_int("issue: fu_order: j = ", j);

        };
        fu_order[2] = EMPTY;
      } else {
        eRegNew.bubble = true;
        
      };
      
      
      return ();
    } else {
        eRegNew.bubble = true;
    };
  }   
}

val execute_forward_data : ast -> unit
function execute_forward_data(ast) = {
  match ast {
    LOAD(imm, rs1, rd, is_unsigned, width, aq, rl) => {
      if eRegNew.writeReg & (eRegNew.destReg != EXTZ(0b0)) 
      then { 
       if dRegNew.rs1 == eRegNew.destReg & exe_issue == fu_forward
       then {
          fRegNew.stall = 1;
          dRegNew.stall = 1;
          fu_forward = EMPTY;

       };

       if dRegNew.rs2 == eRegNew.destReg & exe_issue == fu_forward
       then {
          fRegNew.stall = 1;
          dRegNew.stall = 1;  
          fu_forward = EMPTY;
   
       };
      };
    },
    
    _ => {
  /*
  if fu_forward == EMPTY
  then print("fu_forward = EMPTY");
  if fu_forward == ALU
  then print("fu_forward = ALU");
  if fu_forward == MULTIPLIER
  then print("fu_forward = MULTIPLIER");
  if fu_forward == DIVIDER
  then print("fu_forward = DIVIDER");

  if exe_issue == EMPTY
  then print("exe_issue = EMPTY");
  if exe_issue == ALU
  then print("exe_issue = ALU");
  if exe_issue == MULTIPLIER
  then print("exe_issue = MULTIPLIER");
  if exe_issue == DIVIDER
  then print("exe_issue = DIVIDER");
  if eRegNew.writeReg 
  then {print("eRegNew.writeReg = true" );}
  else print("eRegNew.writeReg = false" );
  
   print ("eRegNew.destReg = " ^ BitStr(eRegNew.destReg));
           print("dRegNew.rs1   = " ^ BitStr(dRegNew.rs1 ));
           print("dRegNew.rs2   = " ^ BitStr(dRegNew.rs2 ));
           print("dReg.rs1   = " ^ BitStr(dReg.rs1 ));
           print("dReg.rs2   = " ^ BitStr(dReg.rs2 ));
  */
      if eRegNew.writeReg & (eRegNew.destReg != EXTZ(0b0)) 
      then { 
       if dRegNew.rs1 == eRegNew.destReg & exe_issue == fu_forward
       then {
         dRegNew.op1 = eRegNew.out;
         executeWBReg = eRegNew.destReg; 
         executeWriteBack = true;  
         history.dataHazardCount = history.dataHazardCount + 1;
         fu_forward = EMPTY;

         if get_config_print_pipeline()
         then print("  Forward Data to Decode op1" );    
       };

       if dRegNew.rs2 == eRegNew.destReg & exe_issue == fu_forward
       then {
         dRegNew.op2 = eRegNew.out;
         executeWBReg = eRegNew.destReg; 
         executeWriteBack = true;  
         history.dataHazardCount = history.dataHazardCount + 1;
         fu_forward = EMPTY;
         if get_config_print_pipeline()
         then print("  Forward Data to Decode op2" );        
       };

       if dReg.stall > 0 
       then {
         if dReg.rs1 == eRegNew.destReg & exe_issue == fu_forward
         then {
           dReg.op1 = eRegNew.out;
           executeWBReg = eRegNew.destReg; 
           executeWriteBack = true;  
           history.dataHazardCount = history.dataHazardCount + 1;
           fu_forward = EMPTY;

           if get_config_print_pipeline()
           then print("  Stall Forward Data to Decode op1" ); 
         };

         if dReg.rs2 == eRegNew.destReg & exe_issue == fu_forward
         then {
           dReg.op2 = eRegNew.out;
           executeWBReg = eRegNew.destReg; 
           executeWriteBack = true;  
           history.dataHazardCount = history.dataHazardCount + 1;
           fu_forward = EMPTY;
           if get_config_print_pipeline()
           then print("  Stall Forward Data to Decode op2" );     
         };
       };
     };
   }
  }
}
/* for pipeline */ 

