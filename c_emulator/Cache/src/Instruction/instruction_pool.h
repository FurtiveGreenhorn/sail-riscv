#pragma once
#include <array>
#include "instruction.h"
#include <cassert>
#include <iostream>

#define INST_POOL_SIZE 10

template<auto size>
class Instruction_pool {
public:
    Instruction_pool() : start(0), end(0) {
        bubble.type = INST_nop;
        bubble.is_used = true;
    }
    Instruction *newInst() {
        if (end == size) 
            end = 0;
        auto& new_inst = inst[end++];
        assert(new_inst.is_used == false);
        new_inst.is_used = true;
        // for debug
        new_inst.debugNo = end - 1;
        if (logged) {
            // std::cout << "Instruction Created !" << std::endl;
            // for debug
            std::cout << "Instruction " << end - 1 << " Created !" << std::endl;
        }
        return &new_inst;
    }
    void freeInst() {
        // Note: Dtor inst if needed
        //  empty checking
        if (start == size) 
            start = 0;
        auto& free_inst = inst[start++];
        assert(free_inst.is_used == true);
        free_inst.is_used = false;
        if (logged) {
            // std::cout << "Instruction Retired !" << std::endl;
            // for debug
            std::cout << "Instruction " << start - 1 << " Retired !" << std::endl;
        }
    }
    Instruction *getBubble() {
        return &bubble;
    }
private:
    std::array<Instruction, size> inst;
    Instruction bubble;
    std::size_t start, end;
    bool logged = true;
};