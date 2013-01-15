CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

project( EEMD C CXX )

set( DEPENDENCIES GLUT OpenGL Armadillo  )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${EEMD_BINARY_DIR}/libs)

message("\n")
foreach(DEP ${DEPENDENCIES})

	message("Looking for ${DEP}...")

    find_package( ${DEP} REQUIRED )

    string( TOUPPER ${DEP} DEP )
    if( ${DEP}_FOUND )
        message("${DEP}_Found = TRUE\n")
    else()
        message("${DEP}_Found = FALSE\n")
    endif()

endforeach()

set( EEMD_INCLUDE_DIR ${EEMD_SOURCE_DIR}/include )
include_directories( 

    ${EEMD_INCLUDE_DIR}
	${GLUT_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${ARMADILLO_INCLUDE_DIRS}
    )

link_directories( 

    ${EEMD_BINARY_DIR}/libs 
    )

add_subdirectory( src )
add_executable( test test.cpp ) 
target_link_libraries( test

    ${EEMD_LIBRARIES}
	${GLUT_LIBRARY} 
	${OPENGL_LIBRARIES} 
	${ARMADILLO_LIBRARIES} 
	lapack
	BLAS
)

