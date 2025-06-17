# This file is located in 'test/Clock'.

# Define a single executable for all tests related to the Clock module.
# List all test source files explicitly here.
add_executable(ClockModuleTests
    Concept/clock_concept_test.cpp
    MinimalClock/minimal_clock_test.cpp # Specify the path relative to this CMakeLists.txt
)

# Set include directories for the ClockModuleTests executable.
# It needs to find:
# 1. Your project's 'src' directory for ClockConcept.h, MinimalClock.h, etc.
# 2. Google Test's include directories.
target_include_directories(ClockModuleTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src             # For src/Clock/ClockConcept.h, src/Clock/MinimalClock/MinimalClock.h
    ${GTEST_INCLUDE_DIRS}               # For Google Test headers
)

# Link the Google Test libraries to your test executable.
target_link_libraries(ClockModuleTests PRIVATE gtest gtest_main)

# Register this executable as a CTest test.
add_test(NAME ClockModuleTests COMMAND ClockModuleTests)

# If you had other Clock-related test files (e.g., specific clock implementations),
# you would add them to the 'add_executable' command above.