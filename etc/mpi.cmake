# try to find flags for mpi

set(YACK_MPI OFF)

if(YACK_WINDOWS)
	# relying on MSMPI

else()
	# relying on mpicxx
	find_program(MPICXX mpicxx)
	if("${MPICXX}" STREQUAL "MPICXX-NOTFOUND")
		message( STATUS "[YACK] no MPICXX" )
	else()
		message( STATUS "[YACK] found MPICXX='${MPICXX}'" )
		set(YACK_MPI ON)
	endif()
endif()
