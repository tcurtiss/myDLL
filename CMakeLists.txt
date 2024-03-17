cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("mydll")
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE)


add_library(mydll SHARED "mydll.cpp" "mydll.h")
set_target_properties(mydll PROPERTIES C_VISIBILITY_PRESET hidden)
set_target_properties(mydll PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(mydll PROPERTIES VISIBILITY_INLINES_HIDDEN 1)

add_executable (mydll_test "mydll_test.cpp" "mydll.h")
if(UNIX)
target_link_libraries(mydll_test dl)
endif()
