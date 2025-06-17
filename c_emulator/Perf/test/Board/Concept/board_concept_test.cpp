// Perf/test/Board/board_concept_test.cpp

#include "gtest/gtest.h"
#include "../../../src/Board/Concept/BoardConcept.h" // **路徑已更新**
#include "../../../src/Instruction/Instruction.h" // 假設 Instruction.h 在 src/Instruction/ (路徑不變)

// --- 測試用型別定義 ---

// 1. 符合 Board 概念的類別 (應通過)
// 這裡創建一個獨立的 GoodBoard，用於驗證 BoardConcept 的正確性
class GoodBoard {
public:
    void process_instruction(const Instruction& inst) { /* 實作不重要 */ }
    uint64_t get_cycle_count() const { return 0; }
};

// 2. 缺少 process_instruction 方法的類別 (應失敗)
class BadBoard_NoProcessInst {
public:
    uint64_t get_cycle_count() const { return 0; }
};

// 3. process_instruction 回傳型別不符的類別 (應失敗)
class BadBoard_ProcessInstWrongReturn {
public:
    int process_instruction(const Instruction& inst) { return 0; } // 回傳 int
    uint64_t get_cycle_count() const { return 0; }
};

// 4. process_instruction 參數型別不符的類別 (應失敗)
class BadBoard_ProcessInstWrongParam {
public:
    void process_instruction(int value) {} // 錯誤參數型別
    uint64_t get_cycle_count() const { return 0; }
};

// 5. 缺少 get_cycle_count 方法的類別 (應失敗)
class BadBoard_NoGetCycleCount {
public:
    void process_instruction(const Instruction& inst) {}
};

// 6. get_cycle_count 回傳型別不符的類別 (應失敗)
class BadBoard_GetCycleCountWrongReturn {
public:
    void process_instruction(const Instruction& inst) {}
    int get_cycle_count() const { return 0; } // 回傳 int
};

// 7. get_cycle_count 沒有 const 的類別 (這個測試會成功通過，因為 BoardConcept 不強制 const)
class BadBoard_GetCycleCountNonConst {
public:
    void process_instruction(const Instruction& inst) {}
    uint64_t get_cycle_count() { return 0; } // 沒有 const
};


// --- Google Test 測試案例 ---

// 測試案例 1：使用 GoodBoard 應該符合 Board 概念
TEST(BoardConceptTests, GoodBoardShouldPass) {
    EXPECT_TRUE(BoardConcept<GoodBoard>::value)
        << "GoodBoard 應符合 Board 概念";
}

// 測試案例 2：BadBoard_NoProcessInst 不應符合 Board 概念
TEST(BoardConceptTests, BadBoardNoProcessInstShouldFail) {
    EXPECT_FALSE(BoardConcept<BadBoard_NoProcessInst>::value)
        << "BadBoard_NoProcessInst 不應符合 (缺少 process_instruction)";
}

// 測試案例 3：BadBoard_ProcessInstWrongReturn 不應符合 Board 概念
TEST(BoardConceptTests, BadBoardProcessInstWrongReturnShouldFail) {
    EXPECT_FALSE(BoardConcept<BadBoard_ProcessInstWrongReturn>::value)
        << "BadBoard_ProcessInstWrongReturn 不應符合 (process_instruction 回傳型別錯誤)";
}

// 測試案例 4：BadBoard_ProcessInstWrongParam 不應符合 Board 概念
TEST(BoardConceptTests, BadBoardProcessInstWrongParamShouldFail) {
    EXPECT_FALSE(BoardConcept<BadBoard_ProcessInstWrongParam>::value)
        << "BadBoard_ProcessInstWrongParam 不應符合 (process_instruction 參數型別錯誤)";
}

// 測試案例 5：BadBoard_NoGetCycleCount 不應符合 Board 概念
TEST(BoardConceptTests, BadBoardNoGetCycleCountShouldFail) {
    EXPECT_FALSE(BoardConcept<BadBoard_NoGetCycleCount>::value)
        << "BadBoard_NoGetCycleCount 不應符合 (缺少 get_cycle_count)";
}

// 測試案例 6：BadBoard_GetCycleCountWrongReturn 不應符合 Board 概念
TEST(BoardConceptTests, BadBoardGetCycleCountWrongReturnShouldFail) {
    EXPECT_FALSE(BoardConcept<BadBoard_GetCycleCountWrongReturn>::value)
        << "BadBoard_GetCycleCountWrongReturn 不應符合 (get_cycle_count 回傳型別錯誤)";
}

// 測試案例 7：BadBoard_GetCycleCountNonConst 應該會通過，因為 BoardConcept 不強制 const
TEST(BoardConceptTests, BadBoardGetCycleCountNonConstShouldPassGivenCurrentConcept) {
    EXPECT_TRUE(BoardConcept<BadBoard_GetCycleCountNonConst>::value)
        << "BadBoard_GetCycleCountNonConst 應符合 (get_cycle_count 存在且回傳正確，即使非 const)";
}