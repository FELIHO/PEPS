
option(BUILD_DOC "Build documentation" ON)
	
find_package(Doxygen)

# check if Doxygen is installed
if(DOXYGEN_FOUND)
    	# set input and output files
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doxy.rc ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif(DOXYGEN_FOUND)
