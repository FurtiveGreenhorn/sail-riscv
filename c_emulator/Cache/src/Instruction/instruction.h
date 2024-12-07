#pragma once
#include <cstdint>

namespace cache {

class Instruction {
    uint64_t addr;
    int rs1, rs2, rd;
};

void show_instruction(int rs1, int rs2, int rd);

}  // end cache namespace 