# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/fileReader.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fileReader.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fileReader.dir/flags.make

CMakeFiles/fileReader.dir/fileReader.cpp.o: CMakeFiles/fileReader.dir/flags.make
CMakeFiles/fileReader.dir/fileReader.cpp.o: ../fileReader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fileReader.dir/fileReader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fileReader.dir/fileReader.cpp.o -c /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/fileReader.cpp

CMakeFiles/fileReader.dir/fileReader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fileReader.dir/fileReader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/fileReader.cpp > CMakeFiles/fileReader.dir/fileReader.cpp.i

CMakeFiles/fileReader.dir/fileReader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fileReader.dir/fileReader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/fileReader.cpp -o CMakeFiles/fileReader.dir/fileReader.cpp.s

# Object files for target fileReader
fileReader_OBJECTS = \
"CMakeFiles/fileReader.dir/fileReader.cpp.o"

# External object files for target fileReader
fileReader_EXTERNAL_OBJECTS =

fileReader: CMakeFiles/fileReader.dir/fileReader.cpp.o
fileReader: CMakeFiles/fileReader.dir/build.make
fileReader: CMakeFiles/fileReader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fileReader"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fileReader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fileReader.dir/build: fileReader

.PHONY : CMakeFiles/fileReader.dir/build

CMakeFiles/fileReader.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fileReader.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fileReader.dir/clean

CMakeFiles/fileReader.dir/depend:
	cd /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug/CMakeFiles/fileReader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fileReader.dir/depend
