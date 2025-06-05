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

/* Extensions may wish to interpose and transform decoded instructions,
 * based on other machine state. This is supported via decode instruction
  hooks, the default implementation of which is provided below.
 */


val ext_decode_compressed : bits(16) -> ast effect {rreg}
function ext_decode_compressed(bv) = encdec_compressed(bv)

val ext_decode : bits(32) -> ast effect {rreg}
function ext_decode(bv) = {
    /* for pipeline */            
    if get_config_pipeline() 
    then {
        if fReg.stall > 0 
        then {
            if   get_config_print_pipeline()
            then print("Decode: Stall");
            PC = PC - 4;
            //dRegNew.bubble = true; ?
            return NONE()
        };
        if fReg.bubble | (fReg.inst == EXTZ(0b0))
        then{
            if   get_config_print_pipeline()
            then print("Decode: Bubble");
            dRegNew.bubble = true;
            return NONE()
        };

        let insn : ast = encdec(fReg.inst);
        predictedBranch : bool = false;
        offset : bits(13) = EXTZ(0b0);
        isBranch : bool = false;
        
        match insn {
            BTYPE(imm, rs2, rs1, op)  => {
                offset = imm;
                isBranch = true;
            },
            _ => {
                ();
            }
        }; 

        if isBranch 
        then{
            predictedBranch = predict(fReg.pc, EXTS(offset));
            if predictedBranch 
            then {
                dRegNew.predictedPC = fReg.pc + EXTS(offset); ///跳轉 
                dRegNew.anotherPC = fReg.pc + 4; //不跳轉
                fRegNew.bubble = true;
            } else {
                dRegNew.anotherPC = fReg.pc + EXTS(offset); 
            }
        };

        setDregNew(insn); //設置dRegNew.rs1(rs2) 為了forward而設置的
        dRegNew.stall = 0;
        dRegNew.bubble = false;
        dRegNew.pc = fReg.pc;  
        dRegNew.predictedBranch = predictedBranch; 

        if get_config_print_pipeline()
        then {
            print("Decoded instruction " ^ BitStr(fReg.inst) ^ " as " ^ to_str(insn));
        };

        insn
    } else {
        encdec(bv)
    };
    /* for pipeline */            
}
