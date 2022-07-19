file(GLOB subdirs build*)
#message( "[YACK] build: ${subdirs}" )

foreach(subdir IN ITEMS ${subdirs})
	message( "[YACK] processing <${subdir}>" )
	set(YACK "all")
	if(EXISTS ${subdir}/yack.xcodeproj)
		message( "[YACK]   found XCode" )
		set(YACK "ALL_BUILD")
	endif()

	if(EXISTS ${subdir}/yack.sln)
		message( "[YACK]   found Visual Studio" )
		set(YACK "ALL_BUILD")
	endif()

	set(cmd "${CMAKE_COMMAND} --build ${subdir} --target ${YACK} --config Release" )
	message( "[YACL]   executing '${cmd}'")
endforeach()


