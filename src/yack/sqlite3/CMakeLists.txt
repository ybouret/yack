project(sqlite3 C)

message( STATUS "[YACK] <SQLite3>" )

include_directories(../yack/zlib)

yack_no_warnings()


if(NOT YACK_WINDOWS)
	yack_add_flags("-std=c99")
endif()

message( STATUS "[YACK]  SQLite3.CMAKE_C_FLAGS='${CMAKE_C_FLAGS}'")

#following advices from official docs
add_definitions(-DSQLITE_THREADSAFE=0)
add_definitions(-DSQLITE_ENABLE_EXPLAIN_COMMENTS)
add_definitions(-DSQLITE_HAVE_ZLIB)
add_definitions(-DSQLITE_INTROSPECTION_PRAGMAS)
add_definitions(-DSQLITE_ENABLE_UNKNOWN_SQL_FUNCTION)
add_definitions(-DSQLITE_ENABLE_STMTVTAB)
add_definitions(-DSQLITE_ENABLE_DBPAGE_VTAB)
add_definitions(-DSQLITE_ENABLE_DBSTAT_VTAB)
add_definitions(-DSQLITE_ENABLE_OFFSET_SQL_FUNC)
add_definitions(-DSQLITE_ENABLE_JSON1)
add_definitions(-DSQLITE_ENABLE_RTREE)
add_definitions(-DSQLITE_ENABLE_FTS4)
add_definitions(-DSQLITE_ENABLE_FTS5)
 
add_library(yack-sqlite3 STATIC sqlite3.c)

include_directories(../zlib)
add_executable(yack_sqlite3 shell.c)
target_link_libraries(yack_sqlite3 yack-sqlite3 yack-z)

if(YACK_LINUX)
	target_link_libraries(yack_sqlite3 dl)
endif()

if(NOT YACK_MICROSOFT)
	target_link_libraries(yack_sqlite3 m)
endif()

if(YACK_WINDOWS)
	if(YACK_GNU)
		target_link_libraries(yack_sqlite3 -static-libgcc -static-libstdc++) 
	endif()
endif()

message( STATUS "[YACK] <SQLite3/>" )
