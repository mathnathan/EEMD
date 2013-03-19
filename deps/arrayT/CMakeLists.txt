cmake_minimum_required(VERSION 2.6)

project(utilities)
set(utils Vec3i.cpp Vec3.cpp main.cpp)
#set(utils Array3D.cpp Vec3i.cpp Vec3.cpp CVec3.cpp matrix3x3f.cpp main.cpp)

add_definitions(-g)

include_directories( . )
add_library(utilities STATIC ${utils})

add_executable(a.out ${utils})
#target_link_libraries(cpu ${libraries})
