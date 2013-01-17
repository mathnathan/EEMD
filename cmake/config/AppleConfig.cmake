message( "\nIn AppleConfig.cmake\n" )

# Update the INCLUDES variable
set( INCLUDES ${INCLUDES} ${EEMD_INCLUDE_DIR} )

# Update the LIBRARIES variable (mac needs lapack and BLAS)
set( LIBRARIES ${LIBRARIES} lapack BLAS )

# Build EEMD libraries
add_subdirectory( ${EEMD_SOURCE_DIR}/lib ) 

# Lastly build the examples
add_subdirectory( ${EEMD_SOURCE_DIR}/examples )
