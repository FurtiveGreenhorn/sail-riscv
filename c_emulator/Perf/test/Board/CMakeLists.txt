# This file is located in 'test/Board'.

# Define a single executable for all tests related to the Board module.
# 我們會將 BoardConcept 和 SimpleBoardConcept 的測試放在同一個執行檔中。
add_executable(BoardModuleTests
    Concept/board_concept_test.cpp
    Concept/simple_board_concept_test.cpp
    # 如果未來有其他 Board 相關的測試檔案 (例如 MinimalBoard 的測試)，可以加在這裡
)

# Set include directories for the BoardModuleTests executable.
# 這裡需要包含所有相關的 src 路徑和 Google Test 的頭文件路徑。
target_include_directories(BoardModuleTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src                          # 讓測試檔案能找到 src/ 目錄下的所有程式碼，
                                                     # 例如 src/Board/Concept/*.h, src/Instruction/*.h,
                                                     # src/Clock/Concept/*.h, src/Core/Concept/*.h
    ${GTEST_INCLUDE_DIRS}                            # Google Test 的頭文件路徑
)

# Link the Google Test libraries.
# 測試執行檔需要連結 Google Test 的主程式和主要函式庫。
target_link_libraries(BoardModuleTests PRIVATE gtest gtest_main)

# Register this executable as a CTest test.
# 這會讓 CTest 能夠發現並執行這個測試集。
add_test(NAME BoardModuleTests COMMAND BoardModuleTests)