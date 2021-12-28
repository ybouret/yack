project(bz2 C)

yack_no_warnings()

add_library(yack-bz2 STATIC 
	blocksort.c  
	huffman.c    
	crctable.c    
	randtable.c  
	compress.c    
	decompress.c 
	bzlib.c
)

add_executable(yack_bzip2 bzip2.c)
target_link_libraries(yack_bzip2 yack-bz2)

add_custom_command( TARGET yack_bzip2
POST_BUILD
COMMAND yack_bzip2 -1 < sample1.ref > sample1.rb2
COMMAND ${CMAKE_COMMAND} -E compare_files sample1.bz2 sample1.rb2 
COMMAND ${CMAKE_COMMAND} -E ${cmake_rm} -f  sample1.rb2 
COMMAND ${CMAKE_COMMAND} -E echo "[YACK] done [bzip2]"
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
COMMENT "[YACK] test [bzip2]"
VERBATIM)
 
