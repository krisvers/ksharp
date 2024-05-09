cmake_minimum_required(VERSION 3.12)
project(ksharpc)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /wd4100 /wd4189")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-but-set-variable")
endif()

file(GLOB_RECURSE CXXSRC "src/*.cpp")

set(OBJDIR obj)
set(BUILDDIR build)
set(BINARY ksharpc)

file(MAKE_DIRECTORY ${OBJDIR} ${BUILDDIR})

add_executable(${BINARY} ${CXXSRC})

target_compile_options(${BINARY} PRIVATE ${CMAKE_CXX_FLAGS})

set_target_properties(${BINARY} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILDDIR}
)

target_include_directories(${BINARY} PRIVATE ${OBJDIR})

target_link_libraries(${BINARY} PRIVATE)
