include_directories(. ../core)
include(../../etc/mpi.cmake)

if(YACK_MPI)
	#include_directories(${YACK_MPI_INCLUDE_DIRS})
	#link_directories(   ${YACK_MPI_LIBRARY_DIRS})
	
	#main
	file(GLOB mpi_main_decl yack/mpi++/*.hpp)
	file(GLOB mpi_main_srcs yack/mpi++/*.cpp)
	set(mpi_main ${mpi_main_decl} ${mpi_main_srcs})
	SOURCE_GROUP(mpi FILES ${mpi_main})
	install( FILES ${mpi_main_decl} DESTINATION include/yack/mpi++ )
	
	#data
	file(GLOB mpi_data_decl yack/mpi++/data/*.hpp)
	file(GLOB mpi_data_srcs yack/mpi++/data/*.cpp)
	set(mpi_data ${mpi_data_decl} ${mpi_data_srcs})
	SOURCE_GROUP(mpi/data FILES ${mpi_data})
	install( FILES ${mpi_data_decl} DESTINATION include/yack/mpi++/data )
	
	add_library(yack-mpi++ STATIC ${mpi_main} ${mpi_data})
	
	install( TARGETS yack-mpi++ ARCHIVE DESTINATION lib )
	
	add_subdirectory(tests)

endif()
