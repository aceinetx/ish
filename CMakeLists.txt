cmake_minimum_required(VERSION 3.20)
project(ish)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED YES)

find_library(MATH_LIBRARY m REQUIRED)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.c")
set(LUA_SOURCES "lua/lapi.c" "lua/lauxlib.c" "lua/lbaselib.c" "lua/lcode.c" "lua/lctype.c" "lua/ldebug.c" "lua/ldo.c" "lua/ldump.c" "lua/lfunc.c" "lua/lgc.c" "lua/linit.c" "lua/liolib.c" "lua/llex.c" "lua/lmathlib.c" "lua/lmem.c" "lua/loadlib.c" "lua/lobject.c" "lua/lopcodes.c" "lua/loslib.c" "lua/lparser.c" "lua/lstate.c" "lua/lstring.c" "lua/lstrlib.c" "lua/ltable.c" "lua/ltablib.c" "lua/ltests.c" "lua/ltm.c" "lua/lua.c" "lua/lundump.c" "lua/lutf8lib.c" "lua/lvm.c" "lua/lzio.c" "lua/lcorolib.c" "lua/ldblib.c")

include_directories("include" "lua")

add_library(lua ${LUA_SOURCES})

add_executable(ish ${SOURCES})
target_link_libraries(ish readline lua ${MATH_LIBRARY})
