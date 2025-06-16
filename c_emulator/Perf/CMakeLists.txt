cmake_minimum_required(VERSION 3.10)
project(Perf LANGUAGES CXX)

# --- 專案設定 ---
# 設定輸出目錄 (與原Makefile的build目錄一致)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# 設定C++標準和編譯選項
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3")

# 偵測SAIL庫路徑
execute_process(
  COMMAND sail --dir
  OUTPUT_VARIABLE SAIL_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT SAIL_DIR)
  message(FATAL_ERROR "SAIL not found. Please install SAIL-RISCV.")
endif()

# !!! 移除所有 find_package(GMP) 相關的邏輯 !!!

# !!! 手動設定 GMP 的包含和庫路徑 !!!
# 根據你的 find 命令結果：/usr/include/x86_64-linux-gnu/gmp.h
# 將包含目錄設定為 /usr/include/x86_64-linux-gnu
set(GMP_INCLUDE_DIRS "/usr/include/x86_64-linux-gnu" CACHE INTERNAL "GMP include directories")
# 函式庫目錄通常是 /usr/lib/x86_64-linux-gnu
set(GMP_LIBRARIES "/usr/lib/x86_64-linux-gnu" CACHE INTERNAL "GMP library directories")

# 檢查這些路徑是否存在 (建議保留，用於早期錯誤檢測)
if(NOT EXISTS "${GMP_INCLUDE_DIRS}/gmp.h")
    message(FATAL_ERROR "Could not find gmp.h at expected path ${GMP_INCLUDE_DIRS}/gmp.h. Please ensure GMP development package is installed (e.g., libgmp-dev) or adjust GMP_INCLUDE_DIRS.")
endif()
# 庫文件檢查可以在編譯時由連結器處理，通常不需要額外的 CMake EXISTS 檢查。


# --- 主要應用程式邏輯 ---
# 添加model_interface子目錄（包含你的主要應用程式程式碼）
# model_interface/CMakeLists.txt 會使用這些變數。
add_subdirectory(model_interface)

# 安裝目標（僅安裝perf_model）
install(TARGETS perf_model
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

# --- 測試程式碼管理 ---

# 定義一個選項，讓使用者可以選擇是否建構測試
# 預設值為 OFF (不建構測試)
option(BUILD_TESTING "Build the project's tests" OFF)

# 只有當 BUILD_TESTING 選項為 ON 時才啟用測試並添加測試目錄
if(BUILD_TESTING)
    # 啟用測試功能 (這是 CMake 測試框架的必需步驟)
    enable_testing()
    # 添加測試目錄。測試目錄下應該有它自己的 CMakeLists.txt
    add_subdirectory(test)
endif()
