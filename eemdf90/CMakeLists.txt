include_directories( ${INCLUDES} )

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
    PARENT_SCOPE
)
