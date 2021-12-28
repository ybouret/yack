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

COMMAND yack_bzip2 -2 < sample2.ref > sample2.rb2
COMMAND ${CMAKE_COMMAND} -E compare_files sample2.bz2 sample2.rb2 
COMMAND ${CMAKE_COMMAND} -E ${cmake_rm} -f  sample2.rb2 

COMMAND yack_bzip2 -3 < sample3.ref > sample3.rb2
COMMAND ${CMAKE_COMMAND} -E compare_files sample3.bz2 sample3.rb2 
COMMAND ${CMAKE_COMMAND} -E ${cmake_rm} -f  sample3.rb2 

COMMAND yack_bzip2 -d  < sample1.bz2 > sample1.tst
COMMAND ${CMAKE_COMMAND} -E compare_files sample1.ref sample1.tst 
COMMAND ${CMAKE_COMMAND} -E ${cmake_rm} -f  sample1.tst 

COMMAND yack_bzip2 -d  < sample2.bz2 > sample2.tst
COMMAND ${CMAKE_COMMAND} -E compare_files sample2.ref sample2.tst 
COMMAND ${CMAKE_COMMAND} -E ${cmake_rm} -f  sample2.tst 

COMMAND yack_bzip2 -ds  < sample3.bz2 > sample3.tst
COMMAND ${CMAKE_COMMAND} -E compare_files sample3.ref sample3.tst 
COMMAND ${CMAKE_COMMAND} -E ${cmake_rm} -f  sample3.tst 


COMMAND ${CMAKE_COMMAND} -E echo "[YACK] done [bzip2]"
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
COMMENT "[YACK] test [bzip2]"
VERBATIM)
 
