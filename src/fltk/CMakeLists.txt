PROJECT(fltk)

INCLUDE(../../etc/fltk.cmake)

IF(FLTK_FOUND)

FILE( GLOB decl yack/FLTK/*.hpp)
FILE( GLOB impl yack/FLTK/*.cpp)
SET( srcs ${decl} ${impl} )
INSTALL( FILES ${decl} DESTINATION include/yack/FLTK )
SOURCE_GROUP( FLTK FILES ${srcs} )

INCLUDE_DIRECTORIES( . ../core ../math)

ADD_LIBRARY( yack-fltk STATIC ${srcs} )
INSTALL( TARGETS yack-fltk ARCHIVE DESTINATION lib )

ADD_SUBDIRECTORY(tests)

ENDIF(FLTK_FOUND)

