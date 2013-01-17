# Set the Example programs here
set( EXAMPLES eemd_test meemd_test )

# Point CMake at our own libraries
#link_directories( ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
include_directories( ${INCLUDES} )

message( "CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}\n" )
message( "EXAMPLES = ${EXAMPLES}\n" )
message( "INCLUDES = ${INCLUDES}\n" )
message( "LIBRARIES = ${LIBRARIES}\n" )

foreach( EXAMPLE ${EXAMPLES} )

    message( "EXAMPLE = ${EXAMPLE}" )
    add_executable( ${EXAMPLE} ${EXAMPLE}.cpp ) 
    target_link_libraries( ${EXAMPLE} ${LIBRARIES} )

endforeach( EXAMPLE )
