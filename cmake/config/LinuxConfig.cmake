message( "\nIn LinuxConfig.cmake\n" )

# Update the INCLUDES variable
set( INCLUDES ${INCLUDES} ${EEMD_INCLUDE_DIR} )

# Build EEMD libraries
add_subdirectory( ${EEMD_SOURCE_DIR}/lib ) 

# Lastly build the examples
#add_subdirectory( ${EEMD_SOURCE_DIR}/examples )




