include_directories( 

    ${EEMD_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIR}
    ${ARMADILLO_INCLUDE_DIR}
    )

link_directories( 

    ${EEMD_BINARY_DIR}/libs 
    )

add_subdirectory( ${EEMD_SOURCE_DIR}/src )

add_executable( test test.cpp ) 

target_link_libraries( test

    ${EEMD_LIBRARIES}
    ${OpenCV_LIBS}
    ${GLUT_LIBRARY} 
    ${OPENGL_LIBRARIES} 
    ${ARMADILLO_LIBRARIES}
    lapack
    BLAS
)
