# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bugthedebugger/Documents/cpp/opencv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bugthedebugger/Documents/cpp/opencv

# Include any dependencies generated for this target.
include CMakeFiles/bugs_scanner.hpp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bugs_scanner.hpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bugs_scanner.hpp.dir/flags.make

CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.o: CMakeFiles/bugs_scanner.hpp.dir/flags.make
CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.o: bugs_scanner.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bugthedebugger/Documents/cpp/opencv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.o -c /home/bugthedebugger/Documents/cpp/opencv/bugs_scanner.cpp

CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bugthedebugger/Documents/cpp/opencv/bugs_scanner.cpp > CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.i

CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bugthedebugger/Documents/cpp/opencv/bugs_scanner.cpp -o CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.s

# Object files for target bugs_scanner.hpp
bugs_scanner_hpp_OBJECTS = \
"CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.o"

# External object files for target bugs_scanner.hpp
bugs_scanner_hpp_EXTERNAL_OBJECTS =

libbugs_scanner.hpp.a: CMakeFiles/bugs_scanner.hpp.dir/bugs_scanner.cpp.o
libbugs_scanner.hpp.a: CMakeFiles/bugs_scanner.hpp.dir/build.make
libbugs_scanner.hpp.a: CMakeFiles/bugs_scanner.hpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bugthedebugger/Documents/cpp/opencv/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libbugs_scanner.hpp.a"
	$(CMAKE_COMMAND) -P CMakeFiles/bugs_scanner.hpp.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bugs_scanner.hpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bugs_scanner.hpp.dir/build: libbugs_scanner.hpp.a

.PHONY : CMakeFiles/bugs_scanner.hpp.dir/build

CMakeFiles/bugs_scanner.hpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bugs_scanner.hpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bugs_scanner.hpp.dir/clean

CMakeFiles/bugs_scanner.hpp.dir/depend:
	cd /home/bugthedebugger/Documents/cpp/opencv && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bugthedebugger/Documents/cpp/opencv /home/bugthedebugger/Documents/cpp/opencv /home/bugthedebugger/Documents/cpp/opencv /home/bugthedebugger/Documents/cpp/opencv /home/bugthedebugger/Documents/cpp/opencv/CMakeFiles/bugs_scanner.hpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bugs_scanner.hpp.dir/depend

