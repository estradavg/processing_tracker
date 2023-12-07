cmake_minimum_required(VERSION 2.8)

project( tracker )

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
endif()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( tracker 
                constrain_tracker/constrain_tracker.cpp )
target_link_libraries( tracker ${OpenCV_LIBS} )