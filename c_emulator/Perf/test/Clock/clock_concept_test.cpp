// Perf/test/Clock/clock_concept_test.cpp

#include "gtest/gtest.h" // 包含 Google Test 框架
#include "../../src/Clock/ClockConcept.h" // 包含你的 ClockConcept.h (確保這個路徑在 CMake 中有被 target_include_directories 包含)

// --- 測試用型別定義 (與之前相同，但可以把命名調整得更符合 Google Test 風格，例如 MyGoodClock) ---

// 1. 符合 Clock 概念的型別 (這個應該要通過測試)
class GoodClock {
public:
    void tick() {
        // 這個函式的實際實作內容不重要，重要的是它存在且簽名正確
    }
};

// 2. 缺少 tick() 方法的型別 (這個應該要導致測試失敗)
class BadClock_NoTick {
public:
    void someOtherMethod() {}
};

// 3. tick() 方法簽名不符的型別 (例如回傳值不是 void，這個應該要導致測試失敗)
class BadClock_WrongReturn {
public:
    int tick() { return 0; } // 回傳 int，不符合 void tick()
};

// 4. tick() 方法有參數的型別 (這個應該要導致測試失敗，因為我們要求無參數)
class BadClock_WithParam {
public:
    void tick(int val) {}
};

// --- Google Test 測試案例 ---
// 使用 TEST(TestSuiteName, TestName) 宏來定義測試案例。
// TestSuiteName (例如 ClockConceptTests) 會將相關測試分組。
// TestName 應該是測試案例的具體描述。
// 每個 TEST 宏內部就是一個獨立的測試案例。

// 測試案例 1：GoodClock 應該符合 Clock 概念
TEST(ClockConceptTests, GoodClockShouldPass) {
    // 使用 EXPECT_TRUE 斷言：如果條件為 false，測試將會失敗，但會繼續執行同一個 TEST 塊內的其他斷言。
    // << "訊息" 會在斷言失敗時顯示，這比 static_assert 的編譯期錯誤更友好。
    EXPECT_TRUE(HasTickMethod<GoodClock>::value)
        << "GoodClock 應符合 Clock 概念 (必須有 void tick())";
}

// 測試案例 2：BadClock_NoTick 不應符合 Clock 概念
TEST(ClockConceptTests, BadClockNoTickShouldFail) {
    EXPECT_FALSE(HasTickMethod<BadClock_NoTick>::value)
        << "BadClock_NoTick 不應符合 Clock 概念 (缺少 tick())";
}

// 測試案例 3：BadClock_WrongReturn 不應符合 Clock 概念
TEST(ClockConceptTests, BadClockWrongReturnShouldFail) {
    EXPECT_FALSE(HasTickMethod<BadClock_WrongReturn>::value)
        << "BadClock_WrongReturn 不應符合 Clock 概念 (回傳值型別不符)";
}

// 測試案例 4：BadClock_WithParam 不應符合 Clock 概念
TEST(ClockConceptTests, BadClockWithParamShouldFail) {
    EXPECT_FALSE(HasTickMethod<BadClock_WithParam>::value)
        << "BadClock_WithParam 不應符合 Clock 概念 (參數不符)";
}

// 注意：這裡不需要 main 函式，因為 gtest_main 會提供它。
// 當 CMake 連結你的測試執行檔到 gtest_main 時，它會自動為你提供運行所有 TEST 案例的 main 函式。
