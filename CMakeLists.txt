PROJECT(WeatherLess)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW) # Required by CMake 2.7+
endif(COMMAND cmake_policy)

# OpenCV library
FIND_PACKAGE(OpenCV 2.4.6 REQUIRED )

# Boost library
if(APPLE)
  set( Boost_USE_STATIC_LIBS ON )
else()
  add_definitions( -DBOOST_ALL_DYN_LINK )
endif()

FIND_PACKAGE( Boost REQUIRED
   	COMPONENTS
   		system
   		filesystem
     	program_options
)
INCLUDE_DIRECTORIES( BEFORE ${Boost_INCLUDE_DIR} )

# # ITK library
# FIND_PACKAGE( ITK REQUIRED )
# INCLUDE( ${ITK_USE_FILE} )

include_directories( BEFORE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
#   ${CMAKE_CURRENT_SOURCE_DIR}/include/boost/serialization
)

set( WeatherLessExe ${CMAKE_BINARY_DIR}/WeatherLess )

# Declare the target (an executable)
ADD_EXECUTABLE(WeatherLess main.cxx)

TARGET_LINK_LIBRARIES(WeatherLess 
    ${OpenCV_LIBS}
    # ${ITK_LIBRARIES}
    ${Boost_LIBRARIES}
)

# Set optimized building:
IF(CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mtune=native")
ENDIF(CMAKE_COMPILER_IS_GNUCXX AND NOT CMAKE_BUILD_TYPE MATCHES "Debug")
