// Perf/test/Board/Concept/simple_board_concept_test.cpp

#include "gtest/gtest.h"
#include "../../../src/Board/Concept/SimpleBoardConcept.h" // 引入 SimpleBoardConcept
#include "../../../src/Clock/Concept/ClockConcept.h"       // 引入 ClockConcept
#include "../../../src/Core/Concept/CoreConcept.h"         // 引入 CoreConcept
#include "../../../src/Instruction/Instruction.h"          // 引入 Instruction 類別定義

// --- 輔助測試類別 ---

// 1. 符合 ClockConcept 的最小類別
class MinClock {
public:
    void tick() {}
    uint64_t get_count() const { return 0; }
};
static_assert(ClockConcept<MinClock>::value, "MinClock 必須符合 ClockConcept 用於測試。");

// 2. 不符合 ClockConcept 的類別 (缺少 tick())
class BadClock_NoTick {
public:
    uint64_t get_count() const { return 0; }
    // 缺少 void tick()
};
static_assert(!ClockConcept<BadClock_NoTick>::value, "BadClock_NoTick 不應符合 ClockConcept 用於測試。");

// 3. 符合 CoreConcept 的最小類別
class MinCore {
public:
    void read_inst(Instruction* inst) {}
};
static_assert(CoreConcept<MinCore>::value, "MinCore 必須符合 CoreConcept 用於測試。");

// 4. 不符合 CoreConcept 的類別 (缺少 read_inst())
class BadCore_NoReadInst {
public:
    // 缺少 void read_inst(Instruction*)
};
static_assert(!CoreConcept<BadCore_NoReadInst>::value, "BadCore_NoReadInst 不應符合 CoreConcept 用於測試。");


// --- SimpleBoardConcept 測試類別 ---

// 1. 完全符合 SimpleBoardConcept 的類別
class FullyConformingSimpleBoard {
public:
    // BoardConcept 要求
    void process_instruction(const Instruction& inst) {}
    uint64_t get_cycle_count() const { return 0; }

    // SimpleBoardConcept 要求：暴露 ClockType 和 CoreType
    using ClockType = MinClock;
    using CoreType = MinCore;
};

// 2. 缺少 ClockType 別名的類別 (不再繼承)
class BoardMissingClockType {
public:
    // BoardConcept 要求
    void process_instruction(const Instruction& inst) {}
    uint64_t get_cycle_count() const { return 0; }
    // 故意缺少 using ClockType = ...;
    using CoreType = MinCore; // 提供 CoreType，以測試僅缺少 ClockType 的情況
};

// 3. 缺少 CoreType 別名的類別 (不再繼承)
class BoardMissingCoreType {
public:
    // BoardConcept 要求
    void process_instruction(const Instruction& inst) {}
    uint64_t get_cycle_count() const { return 0; }
    using ClockType = MinClock; // 提供 ClockType，以測試僅缺少 CoreType 的情況
    // 故意缺少 using CoreType = ...;
};

// 4. 符合 BoardConcept 但 ClockType 不符合 ClockConcept 的類別
class BoardWithBadClockConcept : public FullyConformingSimpleBoard {
public:
    // BoardConcept 要求已滿足
    using ClockType = BadClock_NoTick; // 使用不符合 ClockConcept 的型別
    using CoreType = MinCore;
};

// 5. 符合 BoardConcept 但 CoreType 不符合 CoreConcept 的類別
class BoardWithBadCoreConcept : public FullyConformingSimpleBoard {
public:
    // BoardConcept 要求已滿足
    using ClockType = MinClock;
    using CoreType = BadCore_NoReadInst; // 使用不符合 CoreConcept 的型別
};

// 6. 不符合 BoardConcept 的類別 (例如缺少方法)
class NonBoardTypeButHasAliases {
public:
    // 缺少 process_instruction 和 get_cycle_count
    using ClockType = MinClock;
    using CoreType = MinCore;
};

// --- SimpleBoardConcept 測試案例 ---

TEST(SimpleBoardConceptTests, FullyConformingSimpleBoardShouldPass) {
    // 測試完全符合 SimpleBoardConcept 的類別
    EXPECT_TRUE(SimpleBoardConcept<FullyConformingSimpleBoard>::value)
        << "FullyConformingSimpleBoard 應該符合 SimpleBoardConcept。";
}

TEST(SimpleBoardConceptTests, BoardMissingClockTypeShouldFail) {
    // 測試缺少 ClockType 別名的類別
    EXPECT_FALSE(SimpleBoardConcept<BoardMissingClockType>::value)
        << "缺少 ClockType 別名的板子不應符合 SimpleBoardConcept。";
}

TEST(SimpleBoardConceptTests, BoardMissingCoreTypeShouldFail) {
    // 測試缺少 CoreType 別名的類別
    EXPECT_FALSE(SimpleBoardConcept<BoardMissingCoreType>::value)
        << "缺少 CoreType 別名的板子不應符合 SimpleBoardConcept。";
}

TEST(SimpleBoardConceptTests, BoardWithBadClockConceptShouldFail) {
    // 測試 ClockType 不符合 ClockConcept 的類別
    EXPECT_FALSE(SimpleBoardConcept<BoardWithBadClockConcept>::value)
        << "ClockType 不符合 ClockConcept 的板子不應符合 SimpleBoardConcept。";
}

TEST(SimpleBoardConceptTests, BoardWithBadCoreConceptShouldFail) {
    // 測試 CoreType 不符合 CoreConcept 的類別
    EXPECT_FALSE(SimpleBoardConcept<BoardWithBadCoreConcept>::value)
        << "CoreType 不符合 CoreConcept 的板子不應符合 SimpleBoardConcept。";
}

TEST(SimpleBoardConceptTests, NonBoardTypeButHasAliasesShouldFail) {
    // 測試不符合 BoardConcept 基礎要求，但有提供別名的類別
    EXPECT_FALSE(SimpleBoardConcept<NonBoardTypeButHasAliases>::value)
        << "不符合 BoardConcept 的板子不應符合 SimpleBoardConcept。";
}

TEST(SimpleBoardConceptTests, BasicTypeShouldFail) {
    // 測試基本型別，不應符合 SimpleBoardConcept
    EXPECT_FALSE(SimpleBoardConcept<int>::value)
        << "int 型別不應符合 SimpleBoardConcept。";
}