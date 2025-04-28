# CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(ant_lora LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------
# Coverage option
# --------------------------------------------------
option(CODE_COVERAGE "Build with --coverage" ON)

if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(COVERAGE_COMPILE_FLAGS -O0 -g --coverage)
    set(COVERAGE_LINK_FLAGS    --coverage)
endif()


# --------------------------------------------------
# ant-lora library target
# --------------------------------------------------
file(GLOB_RECURSE ANT_LORA_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/port/esp32/*.cpp"
)
add_library(ant-lora STATIC ${ANT_LORA_SOURCES})


target_include_directories(ant-lora PUBLIC
    # so consumers do: #include <ant-lora/LoRaRadio.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Link in LogAnywhere so our HAL & core can log
target_link_libraries(ant-lora PUBLIC LogAnywhere)

# Apply coverage flags to library as well (so its code is instrumented)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ant-lora PRIVATE ${COVERAGE_COMPILE_FLAGS})
    target_link_options   (ant-lora PRIVATE ${COVERAGE_LINK_FLAGS})
endif()

# --------------------------------------------------
# Test dependencies (Catch2 header‐only in tests folder)
# --------------------------------------------------
add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/tests)

add_library(MockHal INTERFACE)
target_include_directories(MockHal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/tests/mock)

# --------------------------------------------------
# Discover & build host-side unit tests
# --------------------------------------------------
enable_testing()

file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp")

foreach(TEST_SRC IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC})

    # Instrument tests for coverage
    if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TEST_NAME} PRIVATE ${COVERAGE_COMPILE_FLAGS})
        target_link_options   (${TEST_NAME} PRIVATE ${COVERAGE_LINK_FLAGS})
    endif()

    target_link_libraries(${TEST_NAME} PRIVATE ant-lora Catch2 MockHal)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

# --------------------------------------------------
# Coverage report target
# --------------------------------------------------
add_custom_target(coverage
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMAND ${CMAKE_COMMAND} -E env "LCOV_OUTPUT_DIR=coverage" 
            lcov --directory . --capture --output-file coverage.info
    COMMAND lcov --remove coverage.info '/usr/*' "${CMAKE_SOURCE_DIR}/*/third_party/*" --output-file coverage.cleaned.info
    COMMAND genhtml coverage.cleaned.info --output-directory coverage_html
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating LCOV report in build/coverage_html"
)
