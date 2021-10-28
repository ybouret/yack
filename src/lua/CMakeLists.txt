
message( STATUS "[YACK] <lua>" )

set(lualib_public_decl lua.h luaconf.h lualib.h lauxlib.h lua.hpp)

install( FILES ${lua_public_decl}
DESTINATION include/yack/lua )


set(lualib_srcs
lapi.c
lauxlib.c
lbaselib.c
lcode.c
lcorolib.c
lctype.c
ldblib.c
ldebug.c
ldo.c
ldump.c
lfunc.c
lgc.c
linit.c
liolib.c
llex.c
lmathlib.c
lmem.c
loadlib.c
lobject.c
lopcodes.c
loslib.c
lparser.c
lstate.c
lstring.c
lstrlib.c
ltable.c
ltablib.c
ltm.c
lundump.c
lutf8lib.c
lvm.c
lzio.c
)

message( STATUS "[YACK]  <lib/>" )
add_library(yack-lua STATIC ${lualib_srcs} ${lualib_public_decl})

message( STATUS "[YACK]  <exe/>" )
add_executable(yack_lua lua.c)
target_link_libraries(yack_lua yack-lua)
if(NOT YACK_MICROSOFT)
	target_link_libraries(yack_lua m)
endif()

set( HERE  ${CMAKE_CURRENT_SOURCE_DIR} )
set( THERE ${HERE}/tests )

add_custom_command( TARGET yack_lua
POST_BUILD
COMMAND yack_lua ${THERE}/hello.lua
COMMAND yack_lua ${THERE}/bisect.lua
COMMAND yack_lua ${THERE}/sieve.lua
COMMAND yack_lua ${THERE}/sort.lua
COMMENT "[YACK]  <lua tests/>")


message( STATUS "[YACK] <lua/>" )




