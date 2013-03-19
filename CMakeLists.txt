CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

project( EEMD C CXX Fortran )

set(EEMD_MAJOR_VERSION 0)
set(EEMD_MINOR_VERSION 1)
set(EEMD_PATCH_VERSION 0)
set(EEMD_VERSION
  ${EEMD_MAJOR_VERSION}.${EEMD_MINOR_VERSION}.${EEMD_PATCH_VERSION})

set( EEMD_INCLUDE_DIR ${EEMD_SOURCE_DIR}/include  )
set( DEPENDENCIES GLUT OpenGL Armadillo NetCDF OpenCV )

# A varibale for Gordon *FIX THIS LATER*
#set( ENV{OpenCV_DIR} /Users/erlebach/Documents/src/OpenCV-2.4.3/build )

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

# Build the NetCDF API for accessing data
#   ExternalProject_Add(
#       netcdf-c++4-4.2
#       DOWNLOAD_COMMAND ""
#       CONFIGURE_COMMAND
#   )
 
set( INCLUDES 

    ${GLUT_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${ARMADILLO_INCLUDE_DIRS}
    ${NetCDF_INCLUDE_DIRECTORIES}
    ${OpenCV_INCLUDE_DIRS}
    ${EEMD_INCLUDE_DIR}
)

set( LIBRARIES

    ${GLUT_LIBRARY} 
    ${NetCDF_LIBRARIES}
    ${OPENGL_LIBRARIES} 
    ${ARMADILLO_LIBRARIES}
    ${OpenCV_LIBS}
)

if( ${APPLE} )
    include( "cmake/config/AppleConfig.cmake" )
elseif( ${WIN32} )
    include( "cmake/config/Win32Config.cmake" )
endif()

# Build the F90 EEMD libraries
add_subdirectory( ${EEMD_SOURCE_DIR}/eemdf90 ) 
# Build the C++ EEMD libraries
add_subdirectory( ${EEMD_SOURCE_DIR}/src ) 

# Lastly build the examples
add_subdirectory( ${EEMD_SOURCE_DIR}/examples )
