# 添加所有源文件
file(GLOB_RECURSE ALL_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp" # 包含你的 src/ 目錄下的所有 .cpp 檔案
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" # 包含 model_interface/ 目錄下的所有 .cpp 檔案
)

# 創建單一的效能模型庫
add_library(perf_model STATIC ${ALL_SOURCES})

# 設置目標屬性
# 將函式庫名稱設定為 "perf_model.a" 而非預設的 "libperf_model.a"
set_target_properties(perf_model PROPERTIES
    OUTPUT_NAME "perf_model"
    PREFIX ""
)

# 添加包含目錄
target_include_directories(perf_model PRIVATE
    ${CMAKE_SOURCE_DIR}/src      # 你的 src/ 目錄下的頭文件
    ${SAIL_DIR}/lib/nostd      # Sail 的頭文件路徑 (根據你 find_sail_includes.sh 的結果)
    ${GMP_INCLUDE_DIRS}        # GMP 的頭文件路徑 (由頂層 CMakeLists.txt 手動設定)
)

# 連結 GMP 庫
# target_link_directories 告訴連結器去哪裡找庫文件 (設置搜索路徑)
target_link_directories(perf_model PRIVATE ${GMP_LIBRARIES})
# target_link_libraries 告訴連結器要連結哪個具體的庫
# 'gmp' 是庫的名稱，CMake 會自動補齊前綴 (lib) 和後綴 (.a/.so)
target_link_libraries(perf_model PRIVATE gmp)
