project(sqlite3 C)

add_definitions(-DSQLITE_THREADSAFE=0)
add_definitions(-DSQLITE_ENABLE_EXPLAIN_COMMENTS)
#-DSQLITE_HAVE_ZLIB
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
add_executable(yack_sqlite3_shell shell.c)
target_link_libraries(yack_sqlite3_shell yack-sqlite3 m)

if(YACK_LINUX)
	target_link_libraries(yack_sqlite3_shell dl)
endif()

if(YACK_WINDOWS)
	if(YACK_GNU)
		target_link_libraries(yack_sqlite3_shell -static-libgcc -static-libstdc++) 
	endif()
endif()
