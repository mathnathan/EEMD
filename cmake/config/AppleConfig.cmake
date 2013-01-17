message( "\nIn AppleConfig.cmake\n" )

# A varibale for Gordon *FIX THIS LATER*
set( OpenCV_DIR /Users/erlebach/Documents/src/OpenCV-2.4.3 )

# Update the INCLUDES variable
set( INCLUDES ${INCLUDES} ${EEMD_INCLUDE_DIR} )

# Update the LIBRARIES variable (mac needs lapack and BLAS)
set( LIBRARIES ${LIBRARIES} lapack BLAS )

# Build EEMD libraries
add_subdirectory( ${EEMD_SOURCE_DIR}/lib ) 

# Lastly build the examples
add_subdirectory( ${EEMD_SOURCE_DIR}/examples )
