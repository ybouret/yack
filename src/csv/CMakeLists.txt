include_directories(. ../core ../jive)

#main
file(GLOB csv_main_decl yack/csv/*.hpp)
file(GLOB csv_main_srcs yack/csv/*.cpp)
set(csv_main ${csv_main_decl} ${csv_main_srcs})
SOURCE_GROUP(csv FILES ${csv_main})
install( FILES ${csv_main_decl} DESTINATION include/yack/csv )


add_library(yack-csv STATIC
${csv_main}
)

install( TARGETS yack-csv ARCHIVE DESTINATION lib )

add_subdirectory(tests)
yack_utest(csv)
yack_libs(yack-csv)

