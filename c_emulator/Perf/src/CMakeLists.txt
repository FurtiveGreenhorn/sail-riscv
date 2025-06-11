# 自動收集所有源文件
file(GLOB_RECURSE SOURCES "*.cpp")

# 添加包含路徑 (包含原Makefile的-I./src設定)
include_directories(
    ${CMAKE_SOURCE_DIR}/src   # 對應原Makefile的-I./src
    ${CMAKE_SOURCE_DIR}/include
    ${SAIL_LIB_DIR}
)

# 創建靜態庫目標
add_library(perflib STATIC ${SOURCES})

# 設置目標屬性：移除lib前綴，直接輸出到build目錄根層
set_target_properties(perflib PROPERTIES
    OUTPUT_NAME "perflib"
    PREFIX ""
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# 設置安裝規則 (已移至根目錄CMakeLists.txt)