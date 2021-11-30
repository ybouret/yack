project(sqlite3 C)

add_library(yack-sqlite3 STATIC sqlite3.c)
add_executable(yack_sqlite3_shell shell.c)
target_link_libraries(yack_sqlite3_shell yack-sqlite3)
