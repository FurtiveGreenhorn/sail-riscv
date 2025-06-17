#pragma once
#include "../../Instruction/Instruction.h"
#include "../Concept/CoreConcept.h"

/**
 * @brief 最簡核心模型。
 */
class MinimalCore {
public:
    /**
     * @brief 讀取指令。
     */
    void read_inst(Instruction *inst) {
        // 最簡模型，目前不處理指令內容
    }
};

// 檢查 MinimalCore 是否符合 CoreConcept
static_assert(CoreConcept<MinimalCore>::value, "MinimalCore 不符合 CoreConcept。");