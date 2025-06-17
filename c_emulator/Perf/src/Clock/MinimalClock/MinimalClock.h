#pragma once
#include "../Concept/ClockConcept.h"

/**
 * @brief 最簡時脈模型。
 */
class MinimalClock {
public:
    /**
     * @brief 推進時脈。
     */
    void tick() {
        // 最簡模型
    }
};

// 檢查 MinimalClock 是否符合 ClockConcept
static_assert(ClockConcept<MinimalClock>::value, "MinimalClock 不符合 ClockConcept。");