cmake_minimum_required(VERSION 2.8)
project( BugsDocScanner )
find_package( OpenCV REQUIRED )
include_directories( includes ${OpenCV_INCLUDE_DIRS} )
add_executable( BugsDocScanner bugsscanner.cpp )
target_link_libraries( BugsDocScanner ${OpenCV_LIBS} )