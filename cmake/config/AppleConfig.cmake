message( "\nIn AppleConfig.cmake\n" )

# Add special libraries for mac (needs lapack and BLAS)
set( LIBRARIES ${LIBRARIES} lapack BLAS PARENT_SCOPE )
