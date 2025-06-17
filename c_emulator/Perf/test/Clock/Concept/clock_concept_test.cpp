#include "gtest/gtest.h"          // Google Test 框架
#include "../../../src/Clock/Concept/ClockConcept.h" // Clock 概念定義

// --- 測試用型別定義 ---

// 符合概念的時脈
class GoodClock {
public:
    void tick() {}
};

// 缺少 tick() 方法
class BadClock_NoTick {
public:
    void someOtherMethod() {}
};

// tick() 回傳型別錯誤
class BadClock_WrongReturn {
public:
    int tick() { return 0; }
};

// tick() 帶參數
class BadClock_WithParam {
public:
    void tick(int val) {}
};

// --- Google Test 測試案例 ---

// 檢查 GoodClock 是否符合概念
TEST(ClockConceptTests, GoodClockShouldPass) {
    EXPECT_TRUE(ClockConcept<GoodClock>::value) << "GoodClock 應符合 Clock 概念";
}

// 檢查 BadClock_NoTick 是否不符合概念
TEST(ClockConceptTests, BadClockNoTickShouldFail) {
    EXPECT_FALSE(ClockConcept<BadClock_NoTick>::value) << "BadClock_NoTick 不應符合 (缺少 tick())";
}

// 檢查 BadClock_WrongReturn 是否不符合概念
TEST(ClockConceptTests, BadClockWrongReturnShouldFail) {
    EXPECT_FALSE(ClockConcept<BadClock_WrongReturn>::value) << "BadClock_WrongReturn 不應符合 (回傳值錯誤)";
}

// 檢查 BadClock_WithParam 是否不符合概念
TEST(ClockConceptTests, BadClockWithParamShouldFail) {
    EXPECT_FALSE(ClockConcept<BadClock_WithParam>::value) << "BadClock_WithParam 不應符合 (參數錯誤)";
}