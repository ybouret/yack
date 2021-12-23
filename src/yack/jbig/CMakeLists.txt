project(jbig C)

add_library(yack-jbig STATIC 
jbig.h
jbig.c 
jbig_ar.h
jbig_ar.c)

add_executable(yack_jbig tstcodec.c)
target_link_libraries(yack_jbig yack-jbig)

add_custom_command( TARGET yack_jbig
POST_BUILD
COMMAND           $<TARGET_FILE:yack_jbig>
DEPENDS           yack_jbig
COMMENT           "[YACK]  testing [jbig]"
VERBATIM)
