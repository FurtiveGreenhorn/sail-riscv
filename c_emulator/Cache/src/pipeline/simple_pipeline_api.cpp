#include "simple_pipeline.h"

unit show_instructions(mach_bits rs1, mach_bits rs2, mach_bits rd) {
    show_instruction(rs1, rs2, rd);
    return UNIT;
}

static SimplePipeline simplePipeline;