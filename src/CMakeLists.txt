# ------ Build the matplot++ tools ------------
add_library( matplotpp 

    matplotpp.cc 
    gl2ps.c
)

target_link_libraries( matplotpp
	${GLUT_LIBRARY} 
	${OPENGL_LIBRARIES} 
)

# ------ Build the EEMD libraries ------------
add_library( utils

    spline_class.cpp 
    util.cpp 
    eemd1d.cpp
)

set( EEMD_LIBRARIES matplotpp utils CACHE INTERNAL "Internal EEMD libraries" )
