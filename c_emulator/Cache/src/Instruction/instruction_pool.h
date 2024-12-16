#pragma once
#include <array>
#include "instruction.h"
#include <cassert>

#define INST_POOL_SIZE 10

/**
 * @brief A fixed-size pool for managing `Instruction` objects.
 *
 * This class provides a simple circular buffer for `Instruction` objects. 
 * New instructions are allocated from the pool via `newInst()` and 
 * returned to the pool via `freeInst()`. It ensures that the pool does 
 * not run out of instructions (assuming proper usage) by wrapping around 
 * when the end is reached.
 *
 * @tparam size The maximum number of `Instruction` objects the pool can hold.
 */
template<auto size>
class Instruction_pool {
public:
    /**
     * @brief Constructs an empty instruction pool.
     *
     * Initializes `start` and `end` indices to zero, indicating that 
     * no instructions are currently allocated.
     */
    Instruction_pool() : start(0), end(0) {}

    /**
     * @brief Allocates a new `Instruction` from the pool.
     *
     * This method returns a pointer to a free `Instruction` object and marks it as used.
     * If `end` reaches `size`, it wraps around to 0, implementing a circular buffer.
     * An assertion ensures that the allocated instruction is previously unused.
     *
     * @return A pointer to the newly allocated `Instruction`.
     */
    Instruction *newInst() {
        if (end == size) 
            end = 0;
        auto& new_inst = inst[end++];
        assert(new_inst.is_used == false);
        new_inst.is_used = true;
        return &new_inst;
    }

    /**
     * @brief Frees an `Instruction` object and returns it to the pool.
     *
     * This method marks the `Instruction` at the `start` index as free and increments `start`.
     * If `start` reaches `size`, it wraps around to 0. An assertion ensures that the instruction
     * being freed is currently in use. In a more complex scenario, you might call the `Instruction` 
     * destructor here if necessary.
     */
    void freeInst() {
        // Note: Destructor call for Instruction could go here if needed.
        if (start == size) 
            start = 0;
        auto& free_inst = inst[start++];
        assert(free_inst.is_used == true);
        free_inst.is_used = false;
    }

private:
    /**
     * @brief The array holding all `Instruction` objects.
     *
     * Acts as a circular buffer, with `start` and `end` indices defining the usage boundaries.
     */
    std::array<Instruction, size> inst;

    /**
     * @brief The index of the next instruction to free.
     *
     * `start` moves forward as instructions are freed, wrapping around when it reaches `size`.
     */
    std::size_t start;

    /**
     * @brief The index of the next available instruction to allocate.
     *
     * `end` moves forward as instructions are allocated, also wrapping around when it reaches `size`.
     */
    std::size_t end;
};
