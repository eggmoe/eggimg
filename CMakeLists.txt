# CMakeList.txt : CMake project for eggimg, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("eggimg")

add_executable (eggimg WIN32)

target_sources(eggimg PUBLIC
src/eggimg.cpp
src/glad.c
)


target_include_directories(eggimg PUBLIC include)

target_compile_definitions(eggimg PRIVATE UNICODE _UNICODE)

target_link_libraries(eggimg PRIVATE opengl32 gdi32)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET eggimg PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
