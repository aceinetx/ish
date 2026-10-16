cmake_minimum_required(VERSION 3.20)
project(ish)

include(cmake/bin2h.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_C_STANDARD 99)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	set(CMAKE_BUILD_TYPE "Debug")
endif()

find_package(Lua REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")
add_executable(ish ${SOURCES})
target_link_libraries(ish readline ${LUA_LIBRARIES})
target_include_directories(ish PRIVATE "include" ${LUA_INCLUDE_DIR})

install(TARGETS ish)

#region linux/debug specific options
if(LINUX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options(ish PRIVATE "-fsanitize=address")
	target_link_options(ish PRIVATE "-fsanitize=address")
	target_link_options(ish PRIVATE "-Wall")
endif()
#endregion

#region baked files
file(GLOB_RECURSE LUA_SOURCES CONFIGURE_DEPENDS "src/lua/*.lua" "src/lua/**/*.lua")
foreach(FILE ${LUA_SOURCES})
	get_filename_component(FILE_BASE ${FILE} NAME)
	bin2h(SOURCE_FILE ${FILE} HEADER_FILE "${CMAKE_CURRENT_BINARY_DIR}/${FILE_BASE}.h")
endforeach()
target_include_directories(ish PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
#endregion
