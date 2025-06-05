/*for pipeline ntust*/            


enum Strategy = {AT, NT, BTFNT, BPB} //Always Taken, Always Not Taken, Backward Taken Forward Not Taken, 2bit

enum PredictorState = {
    STRONG_TAKEN, WEAK_TAKEN,
    WEAK_NOT_TAKEN, STRONG_NOT_TAKEN   
}

register strategy : Strategy = BPB
register predbuf : vector(4096, dec, PredictorState)
/*
val isBranch : ast -> bool 
function isBranch(insn) = {
    match insn {
        BTYPE(imm, rs2, rs1, op)  => {
            true
        },
        _ => {
            false
        }
    }; 
}
*/
val init_predbuf : unit -> unit 
function init_predbuf() -> unit = {
    foreach (i from 0 to 4095) {
		predbuf[i] = WEAK_TAKEN
    };
}

val predict : (xlenbits, xlenbits) -> bool 
function predict(pc, offset) = {
    
    match strategy {
        NT => { false }, //Always Not Taken
        AT => { true },  //Always Taken
        BTFNT => { //Back Taken Forward Not Taken
            if bit_to_bool(offset[63])  then { true } 
            else { false }
        },
        BPB => { //Branch Prediction Buffer
            let index : int = int_of_bits(pc) % 4096;
            state : PredictorState = STRONG_TAKEN;
            if 0 <= index & index < 4096
            then {state = predbuf[index]};
/*
            if state == STRONG_TAKEN
            then {print_int("", index);print("STRONG_TAKEN: " ^ BitStr(pc));};

            if state == STRONG_NOT_TAKEN
            then {print_int("", index);print("STRONG_NOT_TAKEN: " ^ BitStr(pc));};

            if state == WEAK_TAKEN
            then {print_int("", index);print("WEAK_TAKEN: " ^ BitStr(pc));};

            if state == WEAK_NOT_TAKEN
            then {print_int("", index);print("WAKK_NOT_TAKEN: " ^ BitStr(pc));};
*/
            if state == STRONG_TAKEN | state == WEAK_TAKEN
            then { true }
            else if state == STRONG_NOT_TAKEN | state == WEAK_NOT_TAKEN 
            then { false }
            else {
                print("Strange Perdiction Buffer");
                false
            }
        }, 
        _ => {
            print("Unkown Branch Perdiction Strategy!");
            false
        }
    };

}

val update : (xlenbits, bool) -> unit 
function update(pc, branch) = { 

    let id : int = int_of_bits(pc) % 4096;  
    state : PredictorState = STRONG_TAKEN;

    if 0 <= id & id < 4096
    then {state = predbuf[id]};

    if branch 
    then {
        if state == STRONG_NOT_TAKEN
        then{
            if 0 <= id & id < 4096
            then {predbuf[id] = WEAK_NOT_TAKEN};
        } 
        else if state == WEAK_NOT_TAKEN 
        then {
            if 0 <= id & id < 4096
            then {predbuf[id] = WEAK_TAKEN};
        } 
        else if state == WEAK_TAKEN 
        then {
            if 0 <= id & id < 4096
            then {predbuf[id] = STRONG_TAKEN};
        } // do nothing if STRONG_TAKEN
    } else { // not branch
        if state == STRONG_TAKEN 
        then{
            if 0 <= id & id < 4096
            then {predbuf[id] = WEAK_TAKEN};
        } 
        else if state == WEAK_TAKEN 
        then {
            if 0 <= id & id < 4096
            then {predbuf[id] = WEAK_NOT_TAKEN};
        } 
        else if state == WEAK_NOT_TAKEN 
        then {
            if 0 <= id & id < 4096
            then {predbuf[id] = STRONG_NOT_TAKEN};
        }  // do noting if STRONG_NOT_TAKEN
  }
}
/*for pipeline ntust*/            
