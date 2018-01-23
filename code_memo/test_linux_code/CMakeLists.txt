cmake_minimum_required(VERSION 2.4)

#project name
SET(PROJECTNAME hbb_test)

#output path
SET(OUTPUT ./)

project(${PROJECTNAME})


SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)



# include directories
INCLUDE_DIRECTORIES( 
	#
	./
	)

SET(LIBRARIES
	-lpthread
    )

# link directories
LINK_DIRECTORIES(
	)

#definition
ADD_DEFINITIONS(
	-g
	)

SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT})

# src files
aux_source_directory("src" SRC_FILES)

# executable
ADD_EXECUTABLE(
	${PROJECTNAME}
	${SRC_FILES} 
	)

#add link library
TARGET_LINK_LIBRARIES(${PROJECTNAME} ${LIBRARIES})
