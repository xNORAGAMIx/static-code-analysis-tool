cmake_minimum_required(VERSION 3.13)
project(JargonMaster)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    core/*.cpp
    rules/*.cpp
    main.cpp
)

add_executable(jargon-master ${SOURCES})

target_link_libraries(jargon-master
    clangTooling
    clangBasic
    clangASTMatchers
)