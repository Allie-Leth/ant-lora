cmake_minimum_required(VERSION 3.14)
project(BufferUtilsTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------- coverage option unchanged ----------------
option(CODE_COVERAGE "Build with --coverage" ON)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(COVERAGE_COMPILE_FLAGS -O0 -g --coverage)
    set(COVERAGE_LINK_FLAGS    --coverage)
endif()

# ---------------- header‑only libs --------------------------
add_library(buffer_utils INTERFACE)
target_include_directories(buffer_utils INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include      #  ← added
)

add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/test)

# ---------------- LoRa driver objects ----------------------
file(GLOB LORA_SOURCES
     "src/radios/SX1262/*.cpp"
     "src/device/esp32/*.cpp"
)
add_library(ant_lora STATIC ${LORA_SOURCES})
target_include_directories(ant_lora PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(ant_lora PUBLIC buffer_utils)

# ---------------- discover & build tests -------------------
enable_testing()
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp")

foreach(TEST_SRC IN LISTS TEST_SOURCES)
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC})

    if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${TEST_NAME} PRIVATE -O0 -g --coverage)
        target_link_options   (${TEST_NAME} PRIVATE --coverage)
    endif()

    target_link_libraries(${TEST_NAME} PRIVATE buffer_utils Catch2 ant_lora)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

# -------------- coverage custom target unchanged -----------
add_custom_target(coverage
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMAND C:\\msys64\\usr\\bin\\bash.exe -lc
            "lcov  --directory '${CMAKE_BINARY_DIR}' \
                    --capture         --output-file coverage.info  && \
             lcov  --remove  coverage.info '/usr/*' 'C:/msys64/*' \
                    --output-file coverage.cleaned.info            && \
             genhtml coverage.cleaned.info --output-directory coverage_html"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Generating LCOV report in build/coverage_html"
)
