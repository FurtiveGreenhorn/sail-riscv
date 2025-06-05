val extend_value_new : forall 'n, 0 < 'n <= xlen. (bool, bits('n)) -> xlenbits
function extend_value_new(is_unsigned, value) = if is_unsigned then EXTZ(value) else EXTS(value)

val memoryAccess: unit -> unit
function memoryAccess() = {
    if eReg.stall > 0
    then {
        if   get_config_print_pipeline()
        then print("Memeory Access: Stall");
        return ()

    };
    if eReg.bubble 
    then{
        if   get_config_print_pipeline()
        then print("Memeory Access: Bubble");
        mRegNew.bubble = true;
        return ()
    };

    if get_config_print_pipeline()
    then {
      print("Memory Access: " ^ to_str(east));
    };

    mAccess_succ : bool = true;
    aq = false; 
    rl = false;

    if eReg.writeMem 
    then {
        //Write(Data) 定義在riscv_vmem_types.sail, ext_data_get_addr()並沒有真的使用到Write(Data)
        vaddr = eReg.out;
        match translateAddr(vaddr, Write(Data)) {
        TR_Failure(e, _)    => { handle_mem_exception(vaddr, e); mAccess_succ = false; },
        TR_Address(paddr, _) => {
          
          /*cache*/
          l1d_cache_access(paddr, true); 
          /*cache*/

          //mem_write_ea(): 檢查用
          let eares : MemoryOpResult(unit) = match eReg.memLen {
            BYTE   => mem_write_ea(paddr, 1, aq, rl, false),
            HALF   => mem_write_ea(paddr, 2, aq, rl, false),
            WORD   => mem_write_ea(paddr, 4, aq, rl, false),
             DOUBLE => mem_write_ea(paddr, 8, aq, rl, false)
          };
          match (eares) {
             MemException(e) => { handle_mem_exception(vaddr, e); mAccess_succ = false; },
             MemValue(_) => {
               let rs2_val = eReg.op2;
               let res : MemoryOpResult(bool) = match (eReg.memLen) {
                 BYTE => mem_write_value(paddr, 1, rs2_val[7..0],  aq, rl, false),
                 HALF => mem_write_value(paddr, 2, rs2_val[15..0], aq, rl, false),
                 WORD => mem_write_value(paddr, 4, rs2_val[31..0], aq, rl, false),
                 DOUBLE if sizeof(xlen) >= 64
                     => mem_write_value(paddr, 8, rs2_val,  aq, rl, false),
                 _   =>  {report_invalid_width(__FILE__, __LINE__, eReg.memLen, "store")},
               };
               match (res) {
                 MemValue(true)  => {mAccess_succ = true;},
                 MemValue(false) => {mAccess_succ = false;},
                 MemException(e) => {mAccess_succ = false;}
               }
             }
           }
         }
       }
    };
    //===============================================================================================
   
    out : xlenbits = eReg.out;
    if eReg.readMem
    then {
        vaddr = eReg.out;
        let width_bytes = word_width_bytes(eReg.memLen);
        //print("rs1_val = " ^ BitStr(vaddr));

        match translateAddr(vaddr, Read(Data)) {
          TR_Failure(e, _) => { handle_mem_exception(vaddr, e); mAccess_succ = false;  },
          TR_Address(paddr, _) => {
            /*cache*/
            l1d_cache_access(paddr, false);
            /*cache*/

            match mem_read(Read(Data), paddr, width_bytes, aq, rl, false) {
              MemValue(result) => {out = extend_value_new(eReg.readSignExt, result); mAccess_succ = true;  },
              MemException(e) => {mAccess_succ = true; },
            }
          }
          
        }
    };

    //check for data hazard and forward data
    if eReg.writeReg & eReg.destReg != EXTZ(0b0)
    then {
      if dRegNew.rs1 == eReg.destReg
      then {
        //Avoid overwriting recent data
        if executeWriteBack == false | (executeWriteBack & executeWBReg != eReg.destReg)
        then {
          dRegNew.op1 = out;
          memoryWBReg = eReg.destReg;
          memoryWriteBack = true;
          history.dataHazardCount = history.dataHazardCount + 1;
          if get_config_print_pipeline()
          then print("  Forward Data to Decode op1");     
        }
      };

      if dRegNew.rs2 == eReg.destReg
      then {
        if executeWriteBack == false | (executeWriteBack & executeWBReg != eReg.destReg)
        then {
          dRegNew.op2 = out;
          memoryWBReg = eReg.destReg;
          memoryWriteBack = true;
          history.dataHazardCount = history.dataHazardCount + 1;
          if get_config_print_pipeline()
          then print("  Forward Data to Decode op2");       
        }
      }
    };

    if dReg.stall > 0 & (eReg.writeReg & eReg.destReg != EXTZ(0b0))
    then {
      if dReg.rs1 == eReg.destReg & (executeWriteBack == false | (executeWriteBack & executeWBReg != eReg.destReg))
      then {
        //print("eReg.out = " ^ BitStr(eReg.out));
        dReg.op1 = out;
        memoryStallWBReg = eReg.destReg;
        memoryStallWriteBack = true;
        if get_config_print_pipeline()
        then print("  Stall Forward Data to Decode op1");
      };

      if dReg.rs2 == eReg.destReg & (executeWriteBack == false | (executeWriteBack & executeWBReg != eReg.destReg))
      then {
        dReg.op2 = out;
        memoryStallWBReg = eReg.destReg;
        memoryStallWriteBack = true;
        if get_config_print_pipeline()
        then print("  Stall Forward Data to Decode op2");
      };

      memoryWriteBack = true;
      memoryWBReg = eReg.destReg;
      history.dataHazardCount = history.dataHazardCount + 1;
      
    };

    mRegNew.bubble = false;
    mRegNew.stall = 0;
    mRegNew.pc = eReg.pc;
    mRegNew.op1 = eReg.op1;
    mRegNew.op2 = eReg.op2;
    mRegNew.destReg = eReg.destReg;
    mRegNew.writeReg = eReg.writeReg;
    mRegNew.out = out;

    

    return ()
}








