#pragma once
#include <array>
#include "instruction.h"
#include <cassert>

template<auto size>
class Instruction_pool {
public:
    Instruction_pool() : start(0), end(0) {}
    Instruction *newInst() {
        // FixMe: should support full checking
        if (end == size) end = 0;
        return &inst[end++]; 
    }
    void *freeInst(unsigned num = 1) {
        // FixMe: should support empty checking
        // ToDo: dtor inst if needed
        if (start == size) start = 0;
        return start++;
    }
private:
    std::array<Instruction, size> inst;
    std::size_t start, end;
};