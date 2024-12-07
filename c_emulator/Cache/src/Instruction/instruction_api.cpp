#include "instruction.h"
#include "instruction_api.h"

unit show_instruction(mach_bits rs1, mach_bits rs2, mach_bits rd) {
    cache::show_instruction(rs1, rs2, rd);
    return UNIT;
}