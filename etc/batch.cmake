file(GLOB subdirs build*)
#message( "[YACK] build: ${subdirs}" )

foreach(subdir IN ITEMS ${subdirs})
	message( "[YACK]  processing <${subdir}>" )
endforeach()


