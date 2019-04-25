cmake_minimum_required(VERSION 3.12...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.14)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(CXX20-Example VERSION 1.0.0 DESCRIPTION "C++20 Module Example" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-fmodules-ts" MODULES_TS)

if(MODULES_TS)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts -fprebuilt-module-path=${CMAKE_BINARY_DIR} -fmodule-file=${CMAKE_BINARY_DIR}/foo.pcm")
else()
	message(FATAL_ERROR "C++20 Modules required!")
endif()

set(MODULE_FLAGS -std=c++2a -fmodules-ts -fprebuilt-module-path=${CMAKE_BINARY_DIR})

#If a c++ module is precompiled with a file extension other than *.cppm, '-x c++-module' need to be passed to force the module compilation
add_custom_command(
	OUTPUT std.pcm foo.pcm foobar.pcm
	COMMAND ${CMAKE_CXX_COMPILER} ARGS ${MODULE_FLAGS} --precompile ${CMAKE_SOURCE_DIR}/std.cppm -o ${CMAKE_BINARY_DIR}/std.pcm
	COMMAND ${CMAKE_CXX_COMPILER} ARGS ${MODULE_FLAGS} --precompile ${CMAKE_SOURCE_DIR}/foo.cppm -o ${CMAKE_BINARY_DIR}/foo.pcm
	COMMAND ${CMAKE_CXX_COMPILER} ARGS ${MODULE_FLAGS} --precompile -x c++-module ${CMAKE_SOURCE_DIR}/foobar.cpp -o ${CMAKE_BINARY_DIR}/foobar.pcm
	COMMENT "Create C++ example modules"
	VERBATIM
	SOURCES std.cppm foo.cppm foobar.cpp
)
add_custom_target(modules DEPENDS std.pcm foo.pcm foobar.pcm)

add_executable(Split main.cpp bar.cpp foobar.cpp)
add_dependencies(Split modules)
