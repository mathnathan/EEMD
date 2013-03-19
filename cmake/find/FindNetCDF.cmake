#
# Find NetCDF include directories and libraries
#
# NetCDF_INCLUDE_DIRECTORIES - where to find netcdf.h
# NetCDF_LIBRARIES           - list of libraries to link against when using NetCDF
# NetCDF_FOUND               - Do not attempt to use NetCDF if "no", "0", or undefined.

set( NetCDF_PREFIX "" CACHE PATH "Path to search for NetCDF header and library files" )

find_path( NetCDF_INCLUDE_DIRECTORIES netcdf.h
  /usr/local/include
  /usr/include
)

find_library( NetCDF_C_LIBRARY
  NAMES netcdf
  ${NetCDF_PREFIX}
  ${NetCDF_PREFIX}/lib64
  ${NetCDF_PREFIX}/lib
  /usr/local/lib64
  /usr/lib64
  /usr/lib64/netcdf-3
  /usr/local/lib
  /usr/lib
  /usr/lib/netcdf-3
)

find_library( NetCDF_CXX_LIBRARY
  NAMES netcdf_c++
  ${NetCDF_PREFIX}
  ${NetCDF_PREFIX}/lib64
  ${NetCDF_PREFIX}/lib
  /usr/local/lib64
  /usr/lib64
  /usr/lib64/netcdf-3
  /usr/local/lib
  /usr/lib
  /usr/lib/netcdf-3
)

find_library( NetCDF_FORTRAN_LIBRARY
  NAMES netcdf_g77 netcdf_ifc netcdf_x86_64
  ${NetCDF_PREFIX}
  ${NetCDF_PREFIX}/lib64
  ${NetCDF_PREFIX}/lib
  /usr/local/lib64
  /usr/lib64
  /usr/lib64/netcdf-3
  /usr/local/lib
  /usr/lib
  /usr/lib/netcdf-3
)

set( NetCDF_LIBRARIES
  ${NetCDF_C_LIBRARY}
  ${NetCDF_CXX_LIBRARY}
)

set( NetCDF_FORTRAN_LIBRARIES
  ${NetCDF_FORTRAN_LIBRARY}
)

if ( NetCDF_INCLUDE_DIRECTORIES AND NetCDF_LIBRARIES )
  set( NetCDF_FOUND 1 )
else ( NetCDF_INCLUDE_DIRECTORIES AND NetCDF_LIBRARIES )
  set( NetCDF_FOUND 0 )
endif ( NetCDF_INCLUDE_DIRECTORIES AND NetCDF_LIBRARIES )

mark_as_advanced(
  NetCDF_PREFIX
  NetCDF_INCLUDE_DIRECTORIES
  NetCDF_C_LIBRARY
  NetCDF_CXX_LIBRARY
  NetCDF_FORTRAN_LIBRARY
)
