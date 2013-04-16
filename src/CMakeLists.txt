include_directories( ${INCLUDES} )

# ------ Build the Tools library ------------
add_library( tools

    tools.cpp 
)

target_link_libraries( tools ${LIBRARIES} )

# ------ Build the matplotpp library ------------
add_library( matplotpp

    matplotpp.cc 
    gl2ps.c
)

target_link_libraries( matplotpp

    tools
)

# ------ Build the EEMD library ------------
add_library( eemd

    spline_class.cpp 
    eemd.cpp
)

target_link_libraries( eemd 
    
    matplotpp 
)

# ------ Build the MEEMD library ------------
add_library( meemd

    meemd.cpp
)

target_link_libraries( meemd 

    eemd
)

# Update the LIBRARIES variable
set( LIBRARIES 

    ${LIBRARIES}
    meemd
    PARENT_SCOPE 
)

