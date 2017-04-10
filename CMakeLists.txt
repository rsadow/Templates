cmake_minimum_required(VERSION 3.1)
project(draft)
set (CMAKE_CXX_STANDARD 14)
FIND_PACKAGE( Boost 1.50)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(draft main.cpp)
TARGET_LINK_LIBRARIES( draft ${Boost_LIBRARIES} )

install(TARGETS draft RUNTIME DESTINATION bin)
