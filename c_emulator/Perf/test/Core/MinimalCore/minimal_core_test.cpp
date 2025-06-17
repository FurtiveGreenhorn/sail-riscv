#include "gtest/gtest.h"                  // Google Test 框架
#include "../../../src/Core/MinimalCore/MinimalCore.h" // MinimalCore 實作
#include "../../../src/Core/Concept/CoreConcept.h"      // Core 概念定義
#include "../../../src/Instruction/Instruction.h" // Instruction 類別定義

// 檢查 MinimalCore 是否符合 Core 概念
TEST(MinimalCoreTests, ConformsToCoreConcept) {
    EXPECT_TRUE(CoreConcept<MinimalCore>::value)
        << "MinimalCore 應符合 Core 概念";
}

// 測試 MinimalCore 實例是否可被 read_inst
TEST(MinimalCoreTests, CanReadInstruction) {
    MinimalCore core;
    Instruction inst;
    core.read_inst(&inst); // 呼叫 read_inst 方法
    // 如果 MinimalCore 有可測量的內部狀態，可在這裡新增斷言
}