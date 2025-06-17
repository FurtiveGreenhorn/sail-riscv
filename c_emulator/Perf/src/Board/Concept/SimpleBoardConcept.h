// Perf/src/Board/Concept/SimpleBoardConcept.h

#pragma once
#include <type_traits> // 提供型別特性工具
#include "../../Instruction/Instruction.h" // 假設 Instruction 是必要的
#include "../../Clock/Concept/ClockConcept.h" // 引入 Clock 概念定義
#include "../../Core/Concept/CoreConcept.h"   // 引入 Core 概念定義
#include "BoardConcept.h" // 引入基礎 Board 概念 (SimpleBoard 也是一種 Board)

/**
 * @brief 檢查型別是否符合「簡單板子 (Simple Board)」概念。
 * 此概念適用於內部包含一個明確的時脈型別和一個核心型別，並將其暴露的板子。
 *
 * 合格型別 `T` 必須滿足以下條件：
 * 1. 符合 `BoardConcept` 的所有要求。
 * 2. 透過 `using ClockType = ...;` 宣告其內部時脈的型別別名。
 * 3. 透過 `using CoreType = ...;` 宣告其內部核心的型別別名。
 * 4. 其 `ClockType` 必須符合 `ClockConcept`。
 * 5. 其 `CoreType` 必須符合 `CoreConcept`。
 */
template <typename T, typename = std::void_t<>>
struct SimpleBoardConcept : std::false_type {};

// 偏特化版本：當 T 滿足所有 Simple Board 概念的條件時，此特化會被選中，並繼承 std::true_type。
template <typename T>
struct SimpleBoardConcept<T,
    std::void_t<
        // 首先，確保 T 符合基礎的 BoardConcept。
        // 如果 T 連 BoardConcept 都不符合，這個 enable_if_t 會觸發 SFINAE。
        std::enable_if_t<BoardConcept<T>::value>,

        // 檢查 `T` 是否有 `ClockType` 這個內部型別別名。
        // 如果沒有，`typename T::ClockType` 會導致替換失敗 (SFINAE)。
        typename T::ClockType,
        // 檢查 `T` 是否有 `CoreType` 這個內部型別別名。
        typename T::CoreType,

        // 檢查 `T::ClockType` 是否符合 `ClockConcept`。
        // 如果不符合，`ClockConcept<typename T::ClockType>::value` 會是 false，
        // 導致 `std::enable_if_t` 失敗，從而觸發 SFINAE。
        std::enable_if_t<ClockConcept<typename T::ClockType>::value>,
        // 檢查 `T::CoreType` 是否符合 `CoreConcept`。
        std::enable_if_t<CoreConcept<typename T::CoreType>::value>
    >> : std::true_type {};