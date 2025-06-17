#pragma once
#include <type_traits> // 包含型別特性工具
#include "../../Instruction/Instruction.h" // 包含 Instruction 類別定義，因為 Board 會用到

/**
 * @brief 檢查型別是否符合「板子 (Board)」概念。
 *
 * 合格的「板子」型別 `T` 必須提供以下公有成員函式：
 * 1. `void process_instruction(const Instruction& inst)`: 處理一條指令。
 * 2. `uint64_t get_cycle_count() const`: 取得當前模擬的總週期數。
 *
 * @tparam T 要檢查的型別。
 */
template <typename T, typename = std::void_t<>>
struct BoardConcept : std::false_type {};

// 偏特化版本：當 T 擁有「void process_instruction(const Instruction& inst)」
//           和「uint64_t get_cycle_count() const」方法時才被選中。
// 內部使用 SFINAE 機制判斷。
template <typename T>
struct BoardConcept<T,
    std::void_t<
        // 檢查 process_instruction 方法的存在和簽名
        decltype(std::declval<T>().process_instruction(std::declval<const Instruction&>())),
        std::enable_if_t<std::is_same_v<void, decltype(std::declval<T>().process_instruction(std::declval<const Instruction&>()))>>,

        // 檢查 get_cycle_count 方法的存在和簽名
        decltype(std::declval<T>().get_cycle_count()),
        std::enable_if_t<std::is_same_v<uint64_t, decltype(std::declval<T>().get_cycle_count())>>
    >> : std::true_type {};