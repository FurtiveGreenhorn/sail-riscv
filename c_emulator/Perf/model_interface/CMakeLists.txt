# 添加所有源文件
file(GLOB_RECURSE ALL_SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# 創建單一的效能模型庫
add_library(perf_model STATIC ${ALL_SOURCES})

# 設置目標屬性
set_target_properties(perf_model PROPERTIES
    OUTPUT_NAME "perf_model"
    PREFIX ""
)

# 添加包含目錄
target_include_directories(perf_model PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${SAIL_LIB_DIR}
)

# 安裝規則
install(TARGETS perf_model
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)