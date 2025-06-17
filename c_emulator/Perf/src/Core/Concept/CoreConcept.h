#pragma once
#include <type_traits> // 型別特性工具

class Instruction; // Instruction 前向宣告

/**
 * @brief 檢查型別是否符合「核心 (Core)」概念。
 *
 * 合格型別 `T` 需提供 `void read_inst(Instruction* inst)` 方法。
 */
template <typename T, typename = std::void_t<>>
struct CoreConcept : std::false_type {};

// 偏特化：當 T 擁有「void read_inst(Instruction*)」方法時選中。
template <typename T>
struct CoreConcept<T,
    std::void_t<
        decltype(std::declval<T>().read_inst(static_cast<Instruction*>(nullptr))),
        std::enable_if_t<std::is_same_v<void, decltype(std::declval<T>().read_inst(static_cast<Instruction*>(nullptr)))>>
    >> : std::true_type {};