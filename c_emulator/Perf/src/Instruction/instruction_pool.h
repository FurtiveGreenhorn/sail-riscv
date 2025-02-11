#pragma once
#include <array>
#include "instruction.h"
#include <cassert>
#include <iostream>

namespace pipeline_simulator {

#define INST_POOL_SIZE 10

template<auto size>
class Instruction_pool {
public:
    Instruction_pool() : start(0), end(0) {
        bubble.set_bubble();
    }
    Instruction *newInst() {
        if (end == size) 
            end = 0;
        auto &new_inst = inst[end++].allocate();
        if (debug_logged) {
            std::cout << "Instruction Created !" << std::endl;
        }
        return &new_inst;
    }
    void freeInst() {
        if (start == size) 
            start = 0;
        inst[start++].free();
        if (debug_logged) {
            std::cout << "Instruction Retired !" << std::endl;
        }
    }
    Instruction *getBubble() {
        if (debug_logged) {
            std::cout << "Bubble created !" << std::endl;
        }
        return &bubble;
    }
private:
    std::array<Instruction, size> inst;
    Instruction bubble;
    std::size_t start, end;
    bool debug_logged = false;
};

} // namespace performance