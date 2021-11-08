message( STATUS "cleaning temporary files" )
set(to_remove)

if(APPLE)
	message( STATUS "collecting .DS_Store...")
	file(GLOB_RECURSE ds_store ".DS_Store")
	list(APPEND to_remove ${ds_store})
endif()

macro(remove_pattern p)
	message( STATUS "collecting '${p}'..." )
	file(GLOB tmp ${p})
	list(APPEND to_remove ${tmp})	
endmacro()

remove_pattern(*.dat)

foreach(f IN LISTS to_remove )
	message( STATUS "removing ${f}" )
	execute_process(COMMAND ${CMAKE_COMMAND} -E rm -f ${f})
endforeach()
