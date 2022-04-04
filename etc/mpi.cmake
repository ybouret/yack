# ----------------------------------------------------------------------
#
# try to find flags for mpi
#
# ----------------------------------------------------------------------

set(YACK_MPI OFF)

if(YACK_WINDOWS)
	# ------------------------------------------------------------------
	#
	# relying on MSMPI
	#
	# ------------------------------------------------------------------	
	add_definitions( -DMSMPI_NO_SAL=1 )
	
else()
	# ------------------------------------------------------------------
	#
	# relying on mpicxx
	#
	# ------------------------------------------------------------------

	find_program(MPICXX mpicxx)
	if("${MPICXX}" STREQUAL "MPICXX-NOTFOUND")
		# --------------------------------------------------------------
		# 
		# no MPI
		#
		# --------------------------------------------------------------
		message( STATUS "[YACK] no MPICXX" )
	else()
		# --------------------------------------------------------------
		#
		# using MPI!
		#
		# --------------------------------------------------------------
		message( STATUS "[YACK] found MPICXX='${MPICXX}'" )
			
		# --------------------------------------------------------------
		# find all flags
		# --------------------------------------------------------------
		execute_process(COMMAND ${MPICXX} --show
		                OUTPUT_VARIABLE MPI_INFO
		                RESULT_VARIABLE MPI_SHOW
						ERROR_QUIET)
		if( NOT( "0" STREQUAL "${MPI_SHOW}" ) )
			message( FATAL_ERROR "[YACK] mpicxx --show failure" )
		endif()
		string( STRIP ${MPI_INFO} MPI_INFO)
		message( STATUS "[YACK] [${MPI_INFO}]")
		
		# --------------------------------------------------------------
		# parse 
		# --------------------------------------------------------------
		
		set(YACK_MPI ON)
	endif()
endif()
