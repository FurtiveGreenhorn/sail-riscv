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

/* The emulator fetch-execute-interrupt dispatch loop. */


/* returns whether to increment the step count in the trace */
function step(step_no : int) -> bool = {
  /*for pipeline ntust*/            
  if get_config_pipeline() 
  then {
    if fu_forward == EMPTY
    then{
      executeWriteBack = false; 
      executeWBReg = EXTS(0b1);
    };
    memoryWriteBack = false; 
    memoryWBReg = EXTS(0b1);
    memoryStallWriteBack = false;
    memoryStallWBReg = EXTS(0b0);
  };
  /*for pipeline ntust*/            

  /* for step extensions */
  ext_pre_step_hook();
  /*
   * This records whether or not minstret should be incremented when
   * the instruction is retired. Since retirement occurs before CSR
   * writes we initialise it based on mcountinhibit here, before it is
   * potentially changed. This is also set to false if minstret is
   * written.  See the note near the minstret declaration for more
   * information.
   */
  minstret_increment = mcountinhibit.IR() == 0b0;

  let (retired, stepped) : (Retired, bool) =
    match dispatchInterrupt(cur_privilege) {
      Some(intr, priv) => {
        if   get_config_print_instr()
        then print_bits("Handling interrupt: ", interruptType_to_bits(intr));
        handle_interrupt(intr, priv);
        (RETIRE_FAIL, false)
      },
      None() => {
        /* the extension hook interposes on the fetch result */
        let f : FetchResult = ext_fetch_hook(fetch());
        //print("fetback_PC = " ^ BitStr(PC));

        match f {
          /* extension error */
          F_Ext_Error(e)   => {
            ext_handle_fetch_check_error(e);
            (RETIRE_FAIL, false)
          },
          /* standard error */
          F_Error(e, addr) => {
            handle_mem_exception(addr, e);
            (RETIRE_FAIL, false)
          },
          /* non-error cases: */
          F_RVC(h) => {
            instbits = EXTZ(h);
            let ast = ext_decode_compressed(h);
            if   get_config_print_instr()
            then {
              print_instr("[" ^ string_of_int(step_no) ^ "] [" ^ to_str(cur_privilege) ^ "]: " ^ BitStr(PC) ^ " (" ^ BitStr(h) ^ ") " ^ to_str(ast));
            };
            /* check for RVC once here instead of every RVC execute clause. */
            if haveRVC() then {
              if get_config_pipeline() 
              then {
                () ;
              } else {
                nextPC = PC + 2;
              };
              (execute(ast), true)
             } else {
              handle_illegal();
              (RETIRE_FAIL, true)
             }
          },
          F_Base(w) => {
            instbits = EXTZ(w);
            //print("decfront_PC = " ^ BitStr(PC));

            let ast : ast = ext_decode(w);
            //print("decback_PC = " ^ BitStr(PC));

            /* for pipeline */            
            if get_config_pipeline() 
            then {
              dastNew = ast;
              //print("dastNew: " ^ to_str(dastNew));
              // /print("dast: " ^ to_str(dast));
              execute_check_fu();    //檢查alu,mul和div是否正在運作, 有的話減cycle
              //print("test: " ^ to_str(dastNew));
              if execute_check(dast) //檢查是否為stall or bubble
              then {(RETIRE_SUCCESS,true)} 
              else {
                history.instCount = history.instCount + 1;
                if get_config_print_pipeline()
                then {
                 // print_int("history.instCount = " , history.instCount);
                  print("Execute: " ^ to_str(dast));
                };
                //print("exefront_PC = " ^ BitStr(PC));
                (execute(dast), true)
              }
            } else {
                if   get_config_print_instr()
                then {
                  print_instr("[" ^ string_of_int(step_no) ^ "] [" ^ to_str(cur_privilege) ^ "]: " ^ BitStr(PC) ^ " (" ^ BitStr(w) ^ ") " ^ to_str(ast));
                };
                nextPC = PC + 4;
                (execute(ast), true)
            };
            /* for pipeline */
          }
        }
      }
    };
  
  /* for pipeline */            
  if get_config_pipeline() 
  then {
    execute_issue();
    execute_forward_data(dast);

    //print("memfront_PC = " ^ BitStr(PC));

    memoryAccess();
    //print("memback_PC = " ^ BitStr(PC));

    writeBack();
    //print("writeBack_PC = " ^ BitStr(PC));

  //==========================================================================
    if fReg.stall > 0
    then {
      fReg.stall = fReg.stall - 1;
    } else {
      fReg = fRegNew;
    };
    //print_int("dReg.stall = " , dReg.stall);

    if dReg.stall > 0
    then {
    
      dReg.stall = dReg.stall - 1;
      //mast = east; //為了印出結果 , 改到execute_issue
      //east = NONE(()); //為了印出結果, 改到execute_issue
    } else {
      //mast = east; //為了印出結果 
      //east = dast; //為了印出結果, 改到execute_issue
      dast = dastNew;
      dReg = dRegNew;
    };
    


    mast = east; //為了印出結果

    if exe_issue == MULTIPLIER
    then {east = mdast;} //為了印出結果
    else if exe_issue == DIVIDER
    then {east = ddast;} //為了印出結果
    else if exe_issue == ALU
    then {east = adast;} //為了印出結果
    else east = NONE(());

    eReg = eRegNew;


    mReg = mRegNew;
    init_pipeline_regNews(); 
    // The Branch perdiction happens here to avoid strange bugs in branch prediction
    if dReg.bubble == false & /*dReg.stall == 0 & fReg.stall == 0 &*/ dReg.predictedBranch
    then {
      //print("back");
      PC = dReg.predictedPC;
    };
    history.cycleCount = history.cycleCount + 1;
    if get_config_print_pipeline()
    then {
      print_cpuState();
    };
    //print_int("Number of Instructions: ", history.instCount);

  } else {
    tick_pc();
  };
  /* for pipeline */            

  /* update minstret */
  match retired {
    RETIRE_SUCCESS => retire_instruction(),
    RETIRE_FAIL    => ()
  };

  /* for step extensions */
  ext_post_step_hook();

  stepped
}

function loop () : unit -> unit = {
  let insns_per_tick = plat_insns_per_tick();
  i : int = 0;
  step_no : int = 0;
  while not(htif_done) do {
    let stepped = step(step_no);
    if stepped then step_no = step_no + 1;

    /* check htif exit */
    if htif_done then {
      let exit_val = unsigned(htif_exit_code);
      if exit_val == 0 then print("SUCCESS")
      else print_int("FAILURE: ", exit_val);
    } else {
      /* update time */
      i = i + 1;
      if i == insns_per_tick then {
        tick_clock();
        /* for now, we drive the platform i/o at every clock tick. */
        tick_platform();
        i = 0;
      }
    }
  }
}

/* initialize model state */
function init_model () -> unit = {
  init_platform (); /* devices */
  init_sys ();      /* processor */
  init_vmem ();     /* virtual memory */

  /* for pipeline */            
  if get_config_pipeline() 
  then {
    init_predbuf();
    init_history();
    init_pipeline_regs(); /* pipeline reg ntust*/
    init_pipeline_fu(); /*function unit*/
    fReg.bubble = true;
    dReg.bubble = true;
    eReg.bubble = true;
    mReg.bubble = true;
  };
  /* for pipeline */

  /* initialize extensions last */
  ext_init ();
  ext_init_regs ();
}

