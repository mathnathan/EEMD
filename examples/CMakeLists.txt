# Set the Example programs here
set( EXAMPLES eemd_test meemd_test )

include_directories( ${INCLUDES} )

foreach( EXAMPLE ${EXAMPLES} )

    message( "EXAMPLE = ${EXAMPLE}" )
    add_executable( ${EXAMPLE} ${EXAMPLE}.cpp ) 
    target_link_libraries( ${EXAMPLE} ${LIBRARIES} )

endforeach( EXAMPLE )
