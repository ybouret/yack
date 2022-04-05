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
	if( DEFINED ENV{MSMPI_INC} )
		# get include
		set(YACK_MSMPI_INC $ENV{MSMPI_INC})
		message( STATUS "[YACK]  MSMPI @'${YACK_MSMPI_INC}'")
		include_directories(${YACK_MSMPI_INC})
		
		if("4" STREQUAL "${CMAKE_SIZEOF_VOID_P}")
			#message( STATUS "will use 32 bits" )
			set(YACK_MSMPI_DIR $ENV{MSMPI_LIB32})
		else()
			#message( STATUS "will use 64 bits" )
			set(YACK_MSMPI_DIR $ENV{MSMPI_LIB64})
		endif()
		message( STATUS "[YACK]  MSMPI @'${YACK_MSMPI_DIR}'")
		
		set(YACK_MPI_LIBS "${YACK_MSMPI_DIR}/msmpi.lib") 
		
		# finalize
		add_definitions( -DMSMPI_NO_SAL=1 )
		set(YACK_MPI ON)
		
	else()
		message( STATUS "[YACK]  MSMPI not found")
	endif()
	
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
		message( STATUS "[YACK] [${MPI_INFO}]")
			 
		message( STATUS "[YACK] <MPI::INCLUDE>" )
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
		message( STATUS "[YACK] <MPI::INCLUDE/>" )

 		
 		# --------------------------------------------------------------
		# parse libraries to link
		# --------------------------------------------------------------
		message( STATUS "[YACK] <MPI::LIBRARIES>" )
		foreach(word IN LISTS MPI_INFO)
			message( STATUS " -- trying '${word}'" )
			if(${word} MATCHES "^-l.*")
				message( STATUS " -- OK")
				string(SUBSTRING "${word}" 2 -1 tmp)
				list(APPEND YACK_MPI_LIBS "${tmp}")
			else()
				message( STATUS " -- NO")
			endif()
		endforeach()
		message( STATUS "[YACK]  MPI: LIBS=[${YACK_MPI_LIBS}]" )
 		message( STATUS "[YACK] <MPI::LIBRARIES/>" )

		# --------------------------------------------------------------
		# parse library directories
		# --------------------------------------------------------------
		foreach(word IN LISTS MPI_INFO)
			if(${word} MATCHES "-L.*")
				string(SUBSTRING "${word}" 2 -1 tmp)
				list(APPEND YACK_MPI_LIBRARY_DIRS "${tmp}")
				message( STATUS "[YACK]  MPI: in ${tmp}")
				#link_directories(${tmp})
				string(APPEND CMAKE_EXE_LINKER_FLAGS " -L${tmp}")
			endif()
		endforeach()
		
		
		
		# --------------------------------------------------------------
		# finalize
		# --------------------------------------------------------------
		set(YACK_MPI ON)
	endif()
endif()

message( STATUS "[YACL] <MPI/>")

