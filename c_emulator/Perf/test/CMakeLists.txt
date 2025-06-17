# This file is located in the 'test' directory.

# Find Google Test if it hasn't been found by the top-level CMakeLists.txt
# This ensures it's available for subdirectories if they are added directly.
find_package(GTest REQUIRED)
if(NOT GTEST_FOUND)
    message(FATAL_ERROR "Google Test library not found. Please install libgtest-dev or ensure it's in CMAKE_PREFIX_PATH.")
endif()

# Enable CTest for this project
enable_testing()

# Add the 'Clock' test subdirectory
add_subdirectory(Clock)
add_subdirectory(Core)
add_subdirectory(Board)
# ... (其他測試子目錄) ...

# You would add other test subdirectories here if you had them, e.g.:
# add_subdirectory(Core)
# add_subdirectory(Instruction)