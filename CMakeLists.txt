cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Top level project name and version
project (mydll VERSION 1.0.0 LANGUAGES CXX)

# Set C/C++ standard to C++17/C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
	string(REGEX REPLACE "/W[3|4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") #Bug in CMake, remove /W3 or /W4 before adding custom -Wlevel below
	add_compile_options(-WX -W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Enable command line verbose output
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)

# DLL/.so generation
add_library(${PROJECT_NAME} SHARED "mydll.cpp" "mydll.h")
set_target_properties(${PROJECT_NAME} PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)

# Test harness executable
add_executable (${PROJECT_NAME}_test "mydll_test.cpp" "mydll.h")
if(UNIX)
target_link_libraries(${PROJECT_NAME}_test dl)
endif()