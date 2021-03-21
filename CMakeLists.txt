#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

#set c++14 standard
set (CMAKE_CXX_STANDARD 14)

project(cern_tec CXX)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Set total list of headers and sources
#
#set(sources ${pkg_sources})
#set(headers ${pkg_headers})

#----------------------------------------------------------------------------
# Setup ROOT include directories and compile definitions
#
find_package(ROOT)
if(NOT ROOT_FOUND OR WITHOUT_ROOT)
  message(WARNING "ROOT not found or disabled by hand")
  add_definitions(-D__WITHOUT_ROOT__)
else()
  message(STATUS "ROOT found in ${ROOT_INCLUDE_DIRS}")
  include_directories(${ROOT_INCLUDE_DIRS})
endif()

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
# add_executable(loop.x loop.cpp ${sources} ${headers})
add_executable(main.x main.cpp ${sources} ${headers})
if(ROOT_FOUND)
  target_link_libraries(main.x ${ROOT_LIBRARIES})
endif()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS main.x DESTINATION bin)
