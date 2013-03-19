CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

project( EEMD C CXX Fortran )

set(EEMD_MAJOR_VERSION 0)
set(EEMD_MINOR_VERSION 1)
set(EEMD_PATCH_VERSION 0)
set(EEMD_VERSION
  ${EEMD_MAJOR_VERSION}.${EEMD_MINOR_VERSION}.${EEMD_PATCH_VERSION})

set( EEMD_INCLUDE_DIR ${EEMD_SOURCE_DIR}/inc/ )
set( DEPENDENCIES GLUT OpenGL Armadillo OpenCV )

message( "EEMD INCLUDE = ${EEMD_INCLUDE_DIR}" )

foreach( DEP ${DEPENDENCIES} )
    
    find_package( ${DEP} )

    string( TOUPPER ${DEP} UDEP ) # Capitalize
    if( ${DEP}_FOUND OR ${UDEP}_FOUND )
        message("\n${DEP}_Found = TRUE\n")
    else()
        message("Find${DEP}.cmake not found... Looking for ${DEP} 
        ourselves.\n")
        include( "cmake/find/Find${DEP}.cmake" ) # Try our Find files
        if( ${DEP}_FOUND OR ${UDEP}_FOUND )
            message("\n${DEP}_Found = TRUE\n")
        else()
            message("\n${DEP}_Found = FALSE\n")
        endif()
    endif() 

endforeach()

set( INCLUDES 

    ${GLUT_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${ARMADILLO_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${EEMD_INCLUDE_DIR}
)

set( LIBRARIES

    ${GLUT_LIBRARY} 
    ${OPENGL_LIBRARIES} 
    ${ARMADILLO_LIBRARIES}
    ${OpenCV_LIBS}
)

# ------ Build the EEMD library ------------
add_library( f90eemd

    standev.f90
    natural_spline.f90
    min_max.f90
    gasdev.f90
    eemd.f90
)

# Update the LIBRARIES variable
set( LIBRARIES 

    ${LIBRARIES}
    f90eemd
)

# Build the C++ EEMD libraries
add_subdirectory( ${EEMD_SOURCE_DIR}/src ) 


add_executable( cppeemd test.cpp )
target_link_libraries( cppeemd ${LIBRARIES} )
