include_directories( ${INCLUDES} )

add_library( matplotpp

    matplotpp.cc 
    gl2ps.c
)

target_link_libraries( matplotpp

    ${GLUT_LIBRARY} 
    ${OPENGL_LIBRARIES} 
)

# ------ Build the EEMD library ------------
add_library( eemd

    spline_class.cpp 
    util.cpp 
    eemd1d.cpp
)

target_link_libraries( eemd ${LIBRARIES} matplotpp )

# ------ Build the MEEMD library ------------
add_library( meemd

    meemd.cpp
)

target_link_libraries( meemd 

    ${LIBRARIES} 
    eemd )

# Update the LIBRARIES variable
set( LIBRARIES 

    ${LIBRARIES}
    meemd
    PARENT_SCOPE 
)

