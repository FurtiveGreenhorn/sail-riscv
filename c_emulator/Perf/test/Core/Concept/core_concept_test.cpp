// 首先，包含必要頭檔
#include "gtest/gtest.h"
#include "../../../src/Core/Concept/CoreConcept.h"     // Core 概念定義

// --- CoreConcept 測試類別 ---

// 1. 符合 Core 概念的類別 (應通過)
class GoodCore {
public:
    void read_inst(Instruction* inst) { /* 實作不重要 */ }
};

// 2. 缺少 read_inst 方法的類別 (應失敗)
class BadCore_NoMethod {
public:
    void process_data() {}
};

// 3. read_inst 回傳型別不符的類別 (應失敗)
class BadCore_WrongReturn {
public:
    int read_inst(Instruction* inst) { return 0; }
};

// 4. read_inst 參數型別不符的類別 (應失敗)
class BadCore_WrongParam {
public:
    void read_inst(int value) {}
};

// 5. read_inst 參數數量不符的類別 (應失敗)
class BadCore_NoParam {
public:
    void read_inst() {}
};

// 6. 參數為 const Instruction*，但仍符合概念 (應通過，因可隱式轉換)
class GoodCore_ConstParam {
public:
    void read_inst(const Instruction* inst) { /* 實作不重要 */ }
};

// Perf/test/Core/core_concept_test.cpp (接續)

// --- Google Test 測試案例 ---
TEST(CoreConceptTests, GoodCoreShouldPass) {
    EXPECT_TRUE(CoreConcept<GoodCore>::value)
        << "GoodCore 應符合 Core 概念";
}

TEST(CoreConceptTests, BadCoreNoMethodShouldFail) {
    EXPECT_FALSE(CoreConcept<BadCore_NoMethod>::value)
        << "BadCore_NoMethod 不應符合 (缺少 read_inst)";
}

TEST(CoreConceptTests, BadCoreWrongReturnShouldFail) {
    EXPECT_FALSE(CoreConcept<BadCore_WrongReturn>::value)
        << "BadCore_WrongReturn 不應符合 (回傳型別錯誤)";
}

TEST(CoreConceptTests, BadCoreWrongParamShouldFail) {
    EXPECT_FALSE(CoreConcept<BadCore_WrongParam>::value)
        << "BadCore_WrongParam 不應符合 (參數型別錯誤)";
}

TEST(CoreConceptTests, BadCoreNoParamShouldFail) {
    EXPECT_FALSE(CoreConcept<BadCore_NoParam>::value)
        << "BadCore_NoParam 不應符合 (參數數量錯誤)";
}

TEST(CoreConceptTests, GoodCoreConstParamShouldPass) {
    EXPECT_TRUE(CoreConcept<GoodCore_ConstParam>::value)
        << "GoodCore_ConstParam 應符合 (const Instruction* 相容)";
}

// 注意: 無需 main 函式，gtest_main 會提供。