# test/CMakeLists.txt

# 只有在 BUILD_TESTING 選項為 ON 時才建構測試
if(BUILD_TESTING)
    # 尋找 Google Test 函式庫
    # REQUIRED 表示如果找不到 Google Test 則 CMake 配置失敗。
    # 請確保您的系統已安裝 Google Test 的開發包 (例如：libgtest-dev)。
    find_package(GTest REQUIRED)

    # 檢查 Google Test 是否找到，如果沒有則終止配置。
    if(NOT GTEST_FOUND)
        message(FATAL_ERROR "Google Test library not found. Please install libgtest-dev or ensure it's in CMAKE_PREFIX_PATH.")
    endif()

    # 使用 file(GLOB) 收集 'test/Clock/' 目錄下所有的 .cpp 檔案
    # !!! 修正：將 "clocks" 改為 "Clock" 以匹配實際目錄大小寫 !!!
    # 注意：file(GLOB) 不會自動追蹤新檔案，新增測試檔案後需要重新運行 configure.sh
    file(GLOB CLOCK_TEST_SOURCES "Clock/*.cpp")

    # !!! 新增這行來檢查 file(GLOB) 是否找到檔案 !!!
    message(STATUS "Detected test sources in Clock/: ${CLOCK_TEST_SOURCES}")

    # 為每一個找到的測試源檔案創建一個獨立的測試執行檔和 CTest 註冊
    foreach(TEST_SOURCE ${CLOCK_TEST_SOURCES})
        # 從完整路徑中提取測試名稱 (例如 "Clock/clock_concept_test.cpp" -> "clock_concept_test")
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

        # 定義測試執行檔
        add_executable(${TEST_NAME} ${TEST_SOURCE})

        # 為測試目標添加包含目錄
        # 這確保測試可以找到你的源碼目錄 (src/) 下的標頭檔
        target_include_directories(${TEST_NAME} PRIVATE 
            ${CMAKE_SOURCE_DIR}/src
            ${GTEST_INCLUDE_DIRS} # 加入 Google Test 的包含目錄
        )

        # 連結 Google Test 函式庫
        # 需要同時連結 gtest (核心功能) 和 gtest_main (提供 main 函式)
        target_link_libraries(${TEST_NAME} PRIVATE gtest gtest_main) # <--- 這裡變更了！

        # 將可執行檔註冊為一個 CTest 測試
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()

    # 如果您有其他類型的測試，可以重複上述模式，或者為它們單獨定義
    # 例如：
    # file(GLOB CORE_TEST_SOURCES "core/*.cpp")
    # foreach(TEST_SOURCE ${CORE_TEST_SOURCES})
    #     get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    #     add_executable(${TEST_NAME} ${TEST_SOURCE})
    #     target_include_directories(${TEST_NAME} PRIVATE 
    #         ${CMAKE_SOURCE_DIR}/src
    #         ${GTEST_INCLUDE_DIRS}
    #     )
    #     target_link_libraries(${TEST_NAME} PRIVATE gtest gtest_main)
    #     add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    # endforeach()
endif()
