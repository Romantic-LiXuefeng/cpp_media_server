# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.20.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.20.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/shiwei05/Documents/code/cpp-code/cpp_media_server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/shiwei05/Documents/code/cpp-code/cpp_media_server/__cmake

# Include any dependencies generated for this target.
include CMakeFiles/httpClientDemo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/httpClientDemo.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/httpClientDemo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/httpClientDemo.dir/flags.make

CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.o: CMakeFiles/httpClientDemo.dir/flags.make
CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.o: ../net/httpClientDemo.cpp
CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.o: CMakeFiles/httpClientDemo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/shiwei05/Documents/code/cpp-code/cpp_media_server/__cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.o -MF CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.o.d -o CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.o -c /Users/shiwei05/Documents/code/cpp-code/cpp_media_server/net/httpClientDemo.cpp

CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shiwei05/Documents/code/cpp-code/cpp_media_server/net/httpClientDemo.cpp > CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.i

CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shiwei05/Documents/code/cpp-code/cpp_media_server/net/httpClientDemo.cpp -o CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.s

# Object files for target httpClientDemo
httpClientDemo_OBJECTS = \
"CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.o"

# External object files for target httpClientDemo
httpClientDemo_EXTERNAL_OBJECTS =

httpClientDemo: CMakeFiles/httpClientDemo.dir/net/httpClientDemo.cpp.o
httpClientDemo: CMakeFiles/httpClientDemo.dir/build.make
httpClientDemo: CMakeFiles/httpClientDemo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/shiwei05/Documents/code/cpp-code/cpp_media_server/__cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable httpClientDemo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/httpClientDemo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/httpClientDemo.dir/build: httpClientDemo
.PHONY : CMakeFiles/httpClientDemo.dir/build

CMakeFiles/httpClientDemo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/httpClientDemo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/httpClientDemo.dir/clean

CMakeFiles/httpClientDemo.dir/depend:
	cd /Users/shiwei05/Documents/code/cpp-code/cpp_media_server/__cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/shiwei05/Documents/code/cpp-code/cpp_media_server /Users/shiwei05/Documents/code/cpp-code/cpp_media_server /Users/shiwei05/Documents/code/cpp-code/cpp_media_server/__cmake /Users/shiwei05/Documents/code/cpp-code/cpp_media_server/__cmake /Users/shiwei05/Documents/code/cpp-code/cpp_media_server/__cmake/CMakeFiles/httpClientDemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/httpClientDemo.dir/depend

