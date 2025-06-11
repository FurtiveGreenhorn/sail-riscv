cmake_minimum_required(VERSION 3.10)
project(Perf LANGUAGES CXX)

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

set(SAIL_LIB_DIR "${SAIL_DIR}/lib")
include_directories(${SAIL_LIB_DIR})

# 添加model_interface子目錄（包含所有程式碼）
add_subdirectory(model_interface)

# 安裝目標（僅安裝perf_model）
install(TARGETS perf_model
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)