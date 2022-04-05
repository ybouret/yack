# ----------------------------------------------------------------------
#
# try to find flags for mpi
#
# ----------------------------------------------------------------------
message( STATUS "[YACL] <MPI>")

set(YACK_MPI OFF)
set(YACK_MPI_LIBS "")

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
		message( STATUS "[YACK]  MPI: MPICXX not found!" )
	else()
		# --------------------------------------------------------------
		#
		# using MPI!
		#
		# --------------------------------------------------------------
		message( STATUS "[YACK]  MPI: MPICXX='${MPICXX}'" )
			
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
		message( STATUS "[YACK]  MPI: [${MPI_INFO}]")
		string(REGEX REPLACE " +" ";" MPI_INFO "${MPI_INFO}")
	    #message( STATUS "[YACK] [${MPI_INFO}]")
		list(REMOVE_DUPLICATES MPI_INFO)
	    #message( STATUS "[YACK] [${MPI_INFO}]")
		list(REMOVE_AT MPI_INFO 0)
		#message( STATUS "[YACK] [${MPI_INFO}]")
			    
		# --------------------------------------------------------------
		# parse include directories
		# --------------------------------------------------------------
		foreach(word IN LISTS MPI_INFO)
			if(${word} MATCHES "-I.*")
				string(SUBSTRING "${word}" 2 -1 tmp)
				message( STATUS "[YACK]  MPI: -I${tmp}")
 				include_directories(${tmp})
			endif()
		endforeach()
 		
		# --------------------------------------------------------------
		# parse library directories
		# --------------------------------------------------------------
		foreach(word IN LISTS MPI_INFO)
			if(${word} MATCHES "-L.*")
				string(SUBSTRING "${word}" 2 -1 tmp)
				list(APPEND YACK_MPI_LIBRARY_DIRS "${tmp}")
				message( STATUS "[YACK]  MPI: -L${tmp}")
				link_directories(${tmp})
			endif()
		endforeach()
		
		# --------------------------------------------------------------
		# parse libraries to link
		# --------------------------------------------------------------
		foreach(word IN LISTS MPI_INFO)
			if(${word} MATCHES "-l.*")
				string(SUBSTRING "${word}" 2 -1 tmp)
				list(APPEND YACK_MPI_LIBS "${tmp}")
			endif()
		endforeach()
		message( STATUS "[YACK]  MPI: LIBS=[${YACK_MPI_LIBS}]" )
		
		# --------------------------------------------------------------
		# finalize
		# --------------------------------------------------------------
		set(YACK_MPI ON)
	endif()
endif()

message( STATUS "[YACL] <MPI/>")

