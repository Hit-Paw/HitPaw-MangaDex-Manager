cmake_minimum_required(VERSION 3.20)
project(HitPawTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Test)

enable_testing()

# Export header tests — no app linkage needed, headers are self-contained
add_executable(test_export
  test_export.cpp
)

target_include_directories(test_export PRIVATE ${CMAKE_SOURCE_DIR}/..)

target_link_libraries(test_export PRIVATE Qt6::Core Qt6::Test)

add_test(NAME test_export COMMAND test_export)
