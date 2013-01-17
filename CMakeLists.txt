CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

project( EEMD C CXX )

set(EEMD_MAJOR_VERSION 0)
set(EEMD_MINOR_VERSION 1)
set(EEMD_PATCH_VERSION 0)
set(EEMD_VERSION
  ${EEMD_MAJOR_VERSION}.${EEMD_MINOR_VERSION}.${EEMD_PATCH_VERSION})

set( EEMD_INCLUDE_DIR ${EEMD_SOURCE_DIR}/include )
set( DEPENDENCIES GLUT OpenGL OpenCV Armadillo )

foreach( DEP ${DEPENDENCIES} )
    
    find_package( ${DEP} REQUIRED )

    string( TOUPPER ${DEP} UDEP ) # Capitalize
    if( ${DEP}_FOUND OR ${UDEP}_FOUND )
        message("\n${DEP}_Found = TRUE\n")
    else()
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
    ${OpenCV_INCLUDE_DIR}
)

set( LIBRARIES

    ${GLUT_LIBRARY} 
    ${OPENGL_LIBRARIES} 
    ${ARMADILLO_LIBRARIES}
    ${OpenCV_LIBS}
)

 
if( ${APPLE} )
    include( "cmake/config/AppleConfig.cmake" )
elseif( ${WIN32} )
    include( "cmake/config/WindowsConfig.cmake" )
else()
    include( "cmake/config/LinuxConfig.cmake" )
endif()
