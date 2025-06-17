#pragma once
#include <type_traits> // 型別特性工具

/**
 * @brief 檢查型別是否符合「時鐘 (Clock)」概念。
 *
 * 合格型別 `T` 需提供 `void tick()` 方法。
 */
template <typename T, typename = std::void_t<>>
struct ClockConcept : std::false_type {};

// 偏特化：當 T 擁有「void tick()」方法時選中。
template <typename T>
struct ClockConcept<T, std::void_t<decltype(std::declval<T>().tick()),
                                   std::enable_if_t<std::is_same_v<void, decltype(std::declval<T>().tick())>>>>
    : std::true_type {};

// 範例已移除，因為是註解。