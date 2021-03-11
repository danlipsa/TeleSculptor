# fletch External Project
#
set(KWIVER_DEPENDENCIES)

message(STATUS "Configuring external fletch")

list(APPEND KWIVER_DEPENDENCIES fletch)
list(APPEND TELESCULPTOR_DEPENDENCIES fletch)

if(NOT WIN32)
  set(FLETCH_ADDITIONAL_OPTIONS "-Dfletch_ENABLE_libxml2:BOOL=ON")
endif()

ExternalProject_Add(fletch
  PREFIX ${TELESCULPTOR_BINARY_DIR}
  GIT_REPOSITORY "git://github.com/Kitware/fletch.git"
  GIT_TAG 3b86acd1ed15e566550e43a35634c35b594e0329
  #GIT_SHALLOW 1
  SOURCE_DIR ${TELESCULPTOR_EXTERNAL_DIR}/fletch
  BINARY_DIR ${TELESCULPTOR_EXTERNAL_DIR}/fletch-build
  STAMP_DIR ${TELESCULPTOR_STAMP_DIR}
  CMAKE_CACHE_ARGS
    -DBUILD_SHARED_LIBS:BOOL=ON
    -Dfletch_BUILD_CXX11:BOOL=ON
    -Dfletch_BUILD_WITH_PYTHON:BOOL=${TELESCULPTOR_ENABLE_PYTHON}
    -Dfletch_ENABLE_Boost:BOOL=ON
    -DBoost_SELECT_VERSION:STRING=1.65.1
    -Dfletch_ENABLE_Caffe:BOOL=OFF
    -Dfletch_ENABLE_Caffe_Segnet:BOOL=OFF
    -Dfletch_ENABLE_Ceres:BOOL=ON
    -Dfletch_ENABLE_CppDB:BOOL=OFF
    -Dfletch_ENABLE_Darknet:BOOL=OFF
    -Dfletch_ENABLE_Darknet_OpenCV:BOOL=OFF
    -Dfletch_ENABLE_Eigen:BOOL=ON
    -Dfletch_ENABLE_FFmpeg:BOOL=ON
    -DFFmpeg_SELECT_VERSION:STRING=3.3.3
    -Dfletch_ENABLE_GDAL:BOOL=ON
    -Dfletch_ENABLE_GEOS:BOOL=ON
    -Dfletch_ENABLE_GFlags:BOOL=OFF
    -Dfletch_ENABLE_GLog:BOOL=ON
    -Dfletch_ENABLE_GTest:BOOL=${TELESCULPTOR_ENABLE_TESTING}
    -Dfletch_ENABLE_GeographicLib:BOOL=OFF
    -Dfletch_ENABLE_HDF5:BOOL=OFF
    -Dfletch_ENABLE_ITK:BOOL=OFF
    -Dfletch_ENABLE_LMDB:BOOL=OFF
    -Dfletch_ENABLE_LevelDB:BOOL=OFF
    -Dfletch_ENABLE_OpenBLAS:BOOL=OFF    
    -Dfletch_ENABLE_OpenCV:BOOL=ON
    -DOpenCV_SELECT_VERSION:STRING=3.4.0
    -Dfletch_ENABLE_OpenCV_FFmpeg:BOOL=ON
    -Dfletch_ENABLE_OpenCV_contrib:BOOL=ON
    -Dfletch_ENABLE_OpenCV_highgui:BOOL=ON
    -Dfletch_ENABLE_PDAL:BOOL=ON
    -Dfletch_ENABLE_PNG:BOOL=ON
    -Dfletch_ENABLE_PROJ4:BOOL=ON
    -Dfletch_ENABLE_PostGIS:BOOL=OFF
    -Dfletch_ENABLE_PostgresSQL:BOOL=OFF
    -Dfletch_ENABLE_Protobuf:BOOL=OFF
    -Dfletch_ENABLE_Qt:BOOL=ON
    -DBUILD_Qt_MINIMAL:BOOL=ON
    -DQt_SELECT_VERSION:STRING=5.11.2
    -Dfletch_ENABLE_Snappy:BOOL=OFF
    -Dfletch_ENABLE_SuiteSparse:BOOL=ON
    -Dfletch_ENABLE_TinyXML:BOOL=OFF
    -Dfletch_ENABLE_VTK:BOOL=ON
    -DVTK_SELECT_VERSION:STRING=8.2
    -Dfletch_ENABLE_VXL:BOOL=ON
    -Dfletch_ENABLE_YAMLcpp:BOOL=OFF
    -Dfletch_ENABLE_ZLib:BOOL=ON
    -Dfletch_ENABLE_libgeotiff:BOOL=ON
    -Dfletch_ENABLE_libjpeg-turbo:BOOL=ON
    -Dfletch_ENABLE_libjson:BOOL=OFF
    -Dfletch_ENABLE_libkml:BOOL=OFF
    -Dfletch_ENABLE_libtiff:BOOL=ON
    -Dfletch_ENABLE_log4cplus:BOOL=ON
    -Dfletch_ENABLE_openjpeg:BOOL=OFF
    -Dfletch_ENABLE_qtExtensions:BOOL=ON
    -Dfletch_ENABLE_pybind11:BOOL=${TELESCULPTOR_ENABLE_PYTHON}
    -Dfletch_ENABLE_shapelib:BOOL=OFF
    -Dfletch_BUILD_WITH_CUDA:BOOL=${TELESCULPTOR_ENABLE_CUDA}
    -DCUDA_TOOLKIT_ROOT_DIR:PATH=${CUDA_TOOLKIT_ROOT_DIR}
    -Dfletch_PYTHON_MAJOR_VERSION:STRING=3
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    ${CMAKE_CXX_COMPILER_LAUNCHER_FLAG}
    ${CMAKE_C_COMPILER_LAUNCHER_FLAG}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DADDITIONAL_C_FLAGS:STRING=${ADDITIONAL_C_FLAGS}
    -DADDITIONAL_CXX_FLAGS:STRING=${ADDITIONAL_CXX_FLAGS}
    ${FLETCH_ADDITIONAL_OPTIONS}
  INSTALL_COMMAND ${CMAKE_COMMAND} -E echo "Skipping install step."
)

set(fletch_DIR "${TELESCULPTOR_EXTERNAL_DIR}/fletch-build")

