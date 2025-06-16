#pragma once
#include <type_traits> // 包含 std::void_t, std::true_type, std::false_type, std::is_same_v, std::enable_if_t 等型別特性工具

// --- Clock 概念檢查 ---
//
// 作用：
//   此結構定義了「時鐘型別 (Clock Type)」所需具備的精確基本介面：
//   任何作為時鐘使用的型別 (T)，都必須提供一個名為 'tick()' 的公有成員函式，
//   且該函式必須不帶任何參數，並回傳 'void'。
//   此檢查會在編譯時期執行，確保型別符合要求。
//
// 使用方式：
//   在需要約束時鐘型別的樣板中，使用 static_assert 搭配 HasTickMethod<T>::value。
//   範例：
//     template <typename ClockT>
//     class SomeModule {
//         static_assert(HasTickMethod<ClockT>::value, "ClockT 必須提供一個 'void tick()' 方法 (無參數，回傳 void)");
//         // ...
//     };
//
// 內部實作：
//   使用 C++17 的 SFINAE (Substitution Failure Is Not An Error) 特性。
//   透過嘗試呼叫 'T::tick()' 且不傳遞任何參數，同時檢查其回傳型別是否為 'void' 來判斷。
//   若不符合這些條件 (例如：方法不存在、有參數、回傳非 void)，則會導致替換失敗，
//   從而選擇通用模板，使得 HasTickMethod<T>::value 為 false。

template <typename T, typename = std::void_t<>>
struct HasTickMethod : std::false_type {};

// HasTickMethod 的偏特化版本：
// 僅當 T 滿足以下所有條件時才被選中：
// 1. T 具有一個名為 'tick()' 的方法。
// 2. 'tick()' 方法可以被無參數呼叫 (std::declval<T>().tick())。
// 3. 'tick()' 方法的回傳型別是 'void' (std::is_same_v<void, decltype(std::declval<T>().tick())>)。
// 如果任何條件不滿足，都會導致替換失敗，從而回退到通用版本 (繼承 std::false_type)。
template <typename T>
struct HasTickMethod<T, std::void_t<decltype(std::declval<T>().tick()),
                                     std::enable_if_t<std::is_same_v<void, decltype(std::declval<T>().tick())>>
                                    >> : std::true_type {};

// --- 範例程式碼 (請勿直接包含於實際專案中，僅為示範) ---
/*
// 正確的時鐘範例
class MyGoodClock {
public:
    void tick() { // 符合 'void tick()' 要求
        // 執行時鐘推進邏輯
    }
};

// 錯誤的時鐘範例 (缺少 tick() 方法)
class MyBadClock_NoTick {
public:
    void do_something_else() {
        // ...
    }
};

// 錯誤的時鐘範例 (回傳值不是 void)
class MyBadClock_WrongReturn {
public:
    int tick() { return 0; } // 回傳 int，不符合 void tick()
};

// 錯誤的時鐘範例 (帶有參數)
class MyBadClock_WithParam {
public:
    void tick(int val) {} // 帶有參數，不符合無參數 tick()
};


// 如何在樣板中使用這個概念檢查
template <typename ClockT>
void run_simulation(ClockT& clock_instance) {
    // 編譯時期斷言：確保 ClockT 符合 HasTickMethod 概念
    static_assert(HasTickMethod<ClockT>::value, "傳入的 Clock 型別必須有 'void tick()' 方法 (無參數，回傳 void)！");

    // 如果檢查通過，就可以安全地呼叫 tick()
    clock_instance.tick();
}

// 在 main 函式中測試：
int main() {
    MyGoodClock goodClock;
    run_simulation(goodClock); // 成功編譯並執行

    // MyBadClock_NoTick badClock1;
    // run_simulation(badClock1); // 編譯錯誤：不符合 Clock 概念

    // MyBadClock_WrongReturn badClock2;
    // run_simulation(badClock2); // 編譯錯誤：不符合 Clock 概念

    // MyBadClock_WithParam badClock3;
    // run_simulation(badClock3); // 編譯錯誤：不符合 Clock 概念
    return 0;
}
*/
