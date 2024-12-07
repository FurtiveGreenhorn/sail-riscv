#include <iostream>
#include "instruction.h"

namespace cache {

void show_instruction(int rs1, int rs2, int rd) {
    // std::cout << "addr: " << addr << std::endl;
    std::cout << "rs1: " << rs1 << std::endl;
    std::cout << "rs2: " << rs2 << std::endl;
    std::cout << "rd: " << rd << std::endl;
};

}