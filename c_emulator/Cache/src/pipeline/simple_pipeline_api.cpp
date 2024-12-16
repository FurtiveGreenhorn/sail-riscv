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

unit set_rs(mach_bits rs1, mach_bits rs2) {
    stageInfoInstruction->set_rs(rs1, rs2);
    return UNIT;
}

unit set_rs1(mach_bits rs1) {
    stageInfoInstruction->set_rs1(rs1);
    return UNIT;
}

unit set_rs2(mach_bits rs2) {
    stageInfoInstruction->set_rs2(rs2);
    return UNIT;
}

unit set_rd(mach_bits rd) {
    stageInfoInstruction->set_rs1(rd);
    return UNIT;
}

unit set_taken(bool taken) {
    stageInfoInstruction->set_taken(taken);
    return UNIT;
}