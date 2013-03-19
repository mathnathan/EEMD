include_directories( ${INCLUDES} )

# ------ Build the library ------------
add_library( caller

    cppcall.cpp 
)

target_link_libraries( caller ${LIBRARIES} )

# Update the LIBRARIES variable
set( LIBRARIES 

    ${LIBRARIES}
    caller
    PARENT_SCOPE 
)

