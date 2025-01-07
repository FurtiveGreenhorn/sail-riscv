#include "simple_pipeline.h"
#include "simple_pipeline_api.h"
#include "sail.h"

static SimplePipeline simplePipeline;
static Instruction *stageInfoInstruction;

unit createInstrForStageInfo() {
    stageInfoInstruction = simplePipeline.create_inst();
    return UNIT;
}

unit sendInstToPipeline() {
    simplePipeline.read_inst(stageInfoInstruction);
    return UNIT;
}

unit show_cycle_count() {
    simplePipeline.show_cycle_count();
    return UNIT;
}

unit show_cache_info() {
    simplePipeline.show_cache_info();
    return UNIT;
}

unit read_addr(uint64_t addr) {
    stageInfoInstruction->set_addr(addr);
    return UNIT;
}
unit read_ls_addr(uint64_t addr) {
    stageInfoInstruction->set_ls_addr(addr);
    return UNIT;
}

unit read_rs_val(uint64_t rs1_val, uint64_t rs2_val) {
    stageInfoInstruction->set_rs_val(rs1_val, rs2_val);
    return UNIT;
}

// unit read_rs1_val(uint64_t rs1_val) {
//     stageInfoInstruction->set_rs1_val(rs1_val);
//     return UNIT;
// }

// unit read_rs2_val(uint64_t rs2_val) {
//     stageInfoInstruction->set_rs1_val(rs1_val);
//     return UNIT;
// }

unit read_Rtype(mach_bits rs2, mach_bits rs1, mach_bits rd) {
    stageInfoInstruction->set_Rtype(
        rs1, rs2, rd);
    return UNIT;
}
unit read_Itype(mach_bits rs1, mach_bits rd) {
    stageInfoInstruction->set_Itype(
        rs1, rd);
    return UNIT;
}
unit read_Itype_xRs1(mach_bits rd) {
    stageInfoInstruction->set_Itype(
        REGISTER_NOT_USED, rd);
    return UNIT;
}
unit read_Itype_xRs1xRd() {
    stageInfoInstruction->set_Itype(
        REGISTER_NOT_USED, REGISTER_NOT_USED);
    return UNIT;
}
unit read_SBtype(mach_bits rs2, mach_bits rs1) {
    stageInfoInstruction->set_SBtype(
        rs1, rs2);
    return UNIT;
}
unit read_UJtype(mach_bits rd) {
    stageInfoInstruction->set_UJtype(
        rd);
    return UNIT;
}
unit read_type(const int type) {
    stageInfoInstruction->set_type(Instruction_type(type));
    return UNIT;
}

// unit read_rs(mach_bits rs1, mach_bits rs2) {
//     stageInfoInstruction->set_rs(rs1, rs2);
//     return UNIT;
// }

// unit read_rs1(mach_bits rs1) {
//     stageInfoInstruction->set_rs1(rs1);
//     return UNIT;
// }

// unit read_rd(mach_bits rd) {
//     stageInfoInstruction->set_rs1(rd);
//     return UNIT;
// }

unit read_taken(bool taken) {
    stageInfoInstruction->set_taken(taken);
    return UNIT;
}