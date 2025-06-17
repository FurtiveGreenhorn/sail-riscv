# This file is located in 'test/Core'.

# Define a single executable for all tests related to the Core module.
add_executable(CoreModuleTests
    Concept/core_concept_test.cpp
    MinimalCore/minimal_core_test.cpp
    # Add other core-related test files here later if needed
)

# Set include directories for the CoreModuleTests executable.
target_include_directories(CoreModuleTests PRIVATE
    ${CMAKE_SOURCE_DIR}/src            # For src/Core/CoreConcept.h, src/Instruction/instruction.h, src/Core/MinimalCore/MinimalCore.h
    ${GTEST_INCLUDE_DIRS}              # For Google Test headers
)

# Link the Google Test libraries.
target_link_libraries(CoreModuleTests PRIVATE gtest gtest_main)

# Register this executable as a CTest test.
add_test(NAME CoreModuleTests COMMAND CoreModuleTests)