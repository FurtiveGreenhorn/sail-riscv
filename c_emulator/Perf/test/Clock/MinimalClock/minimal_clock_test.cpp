#include "gtest/gtest.h"                  // Google Test 框架
#include "../../../src/Clock/MinimalClock/MinimalClock.h" // MinimalClock 實作
#include "../../../src/Clock/Concept/ClockConcept.h"      // Clock 概念定義

// 檢查 MinimalClock 是否符合 Clock 概念
TEST(MinimalClockTests, ConformsToClockConcept) {
    EXPECT_TRUE(ClockConcept<MinimalClock>::value)
        << "MinimalClock 應符合 Clock 概念";
}

// 測試 MinimalClock 實例是否可被 tick
TEST(MinimalClockTests, CanTick) {
    MinimalClock clock;
    clock.tick(); // 呼叫 tick 方法
    // 如果 MinimalClock 有可測量的內部狀態，可在這裡新增斷言
}