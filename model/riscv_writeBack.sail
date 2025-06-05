val writeBack: unit -> unit
function writeBack() = {
    if mReg.stall > 0
    then {
        if   get_config_print_pipeline()
        then print("WriteBack: Stall");
        return ()

    };
    if mReg.bubble 
    then{
        if   get_config_print_pipeline()
        then print("WriteBack: Bubble");
        return ()
    };

    if get_config_print_pipeline()
    then {
      print("WriteBack: " ^ to_str(mast));
    };

/*
    print("dRegNew.rs2 = " ^ BitStr(dRegNew.rs2));
    print("mReg.destReg = " ^ BitStr(mReg.destReg));
    print("executeWBReg = " ^ BitStr(executeWBReg));

    if executeWriteBack then   print("executeWriteBack = true")
    else  print("executeWriteBack = false");

    if memoryWriteBack then   print("memoryWriteBack = true")
    else  print("memoryWriteBack = false");
  */  


    //check for data hazard and forward data
    if mReg.writeReg & mReg.destReg != EXTZ(0b0)
    then {
      if dRegNew.rs1 == mReg.destReg 
      then {
        //Avoid overwriting recent data
        if executeWriteBack == false | (executeWriteBack & executeWBReg != mReg.destReg)
        then {
          if memoryWriteBack == false | (memoryWriteBack & memoryWBReg != mReg.destReg)
          then {
            dRegNew.op1 = mReg.out;
            history.dataHazardCount = history.dataHazardCount + 1;
            if get_config_print_pipeline()
            then print(" Forward Data to Decode op1");     
          }
        }
      };

      if dRegNew.rs2 == mReg.destReg
      then {
        if executeWriteBack == false | (executeWriteBack & executeWBReg != mReg.destReg)
        then {
          if memoryWriteBack == false | (memoryWriteBack & memoryWBReg != mReg.destReg)
          then {
            dRegNew.op2 = mReg.out;
            history.dataHazardCount = history.dataHazardCount + 1;
            if get_config_print_pipeline()
            then print(" Forward Data to Decode op2");    
          }
        }
      };


      
      if dReg.stall > 0 
      then {
        if dReg.rs1 == mReg.destReg & (executeWriteBack == false | (executeWriteBack & executeWBReg != mReg.destReg)) & (memoryStallWriteBack == false | (memoryStallWriteBack & memoryStallWBReg != mReg.destReg))
        then {
          //print("eReg.out = " ^ BitStr(eReg.out));
          dReg.op1 = mReg.out;
          if get_config_print_pipeline()
          then print("  Stall Forward Data to Decode op1");
        };

        if dReg.rs2 == mReg.destReg & (executeWriteBack == false | (executeWriteBack & executeWBReg != mReg.destReg)) & (memoryStallWriteBack == false | (memoryStallWriteBack & memoryStallWBReg != mReg.destReg))
        then {
          dReg.op2 = mReg.out;
          if get_config_print_pipeline()
          then print("  Stall Forward Data to Decode op2");
        };
        history.dataHazardCount = history.dataHazardCount + 1;      
    };

    };

    X(mReg.destReg[4 .. 0]) = mReg.out;
}