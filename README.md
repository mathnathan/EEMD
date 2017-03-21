------------------------------------------------
# Ensemble Emprical Mode Decomposition Library

------------------------------------------------

** This project is still under very heavy development **

#### Authors

Gordon Erlebacher - gerlebacher@fsu.edu <br />
Nathan Crock - mathnathan@gmail.com

#### Institution
##### Florida State University

* Department of Scientific Computing
* Center For Ocean-Atmospheric Predition Studies

<br />

## Compilation and Installation

#### OS Support
* Mac OSX
* Ubuntu 12.04

#### Dependencies
[GLUT]: http://www.opengl.org/resources/libraries/glut/
[GLUT][] - OpenGL Utility Toolkit

[OpenGL]: http://www.opengl.org/
[OpenGL][] - Cross-language, multi-platform API for rendering 2D and 3D computer graphics

[NetCDF]: http://www.unidata.ucar.edu/downloads/netcdf/netcdf-cxx/index.jsp
[NetCDF][] - C++ interface to the NetCDF data format

[OpenCV]: http://opencv.org/
[OpenCV][] - Open source computer vision and machine learning software library.

[Armadillo]: http://arma.sourceforge.net/ 
[Armadillo][] - C++ Linear Algebra Library

  
##### Notes on Dependencies

Installation flags for OpenCV

    cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local
    -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON
    -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON
    -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..

#### To Compile

The project is built with cmake 2.8 or higher. From the project's root directory...


    mkdir build
    cd build
    cmake ..
    make

#### To Cite

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.437113.svg)](https://doi.org/10.5281/zenodo.437113)
