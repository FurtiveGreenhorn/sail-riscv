#pragma once
#include <array>
#include "instruction.h"
#include <cassert>

#define INST_POOL_SIZE 10

template<auto size>
class Instruction_pool {
public:
    Instruction_pool() : start(0), end(0) {}
    Instruction *newInst() {
        if (end == size) 
            end = 0;
        auto& new_inst = inst[end++];
        assert(new_inst.is_used == false);
        new_inst.is_used = true;
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
    }
private:
    std::array<Instruction, size> inst;
    std::size_t start, end;
};