file(GLOB srcs test-*.cpp)

add_executable(test-mpi++ main.cpp ${srcs})

target_link_libraries(test-mpi++ yack-mpi++ ${YACK_MPI_LIBS})
yack_link_libraries(test-mpi++)

