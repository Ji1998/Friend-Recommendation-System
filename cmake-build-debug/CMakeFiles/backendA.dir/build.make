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
include CMakeFiles/backendA.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/backendA.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/backendA.dir/flags.make

CMakeFiles/backendA.dir/backend-serverA.cpp.o: CMakeFiles/backendA.dir/flags.make
CMakeFiles/backendA.dir/backend-serverA.cpp.o: ../backend-serverA.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/backendA.dir/backend-serverA.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/backendA.dir/backend-serverA.cpp.o -c /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/backend-serverA.cpp

CMakeFiles/backendA.dir/backend-serverA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/backendA.dir/backend-serverA.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/backend-serverA.cpp > CMakeFiles/backendA.dir/backend-serverA.cpp.i

CMakeFiles/backendA.dir/backend-serverA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/backendA.dir/backend-serverA.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/backend-serverA.cpp -o CMakeFiles/backendA.dir/backend-serverA.cpp.s

# Object files for target backendA
backendA_OBJECTS = \
"CMakeFiles/backendA.dir/backend-serverA.cpp.o"

# External object files for target backendA
backendA_EXTERNAL_OBJECTS =

backendA: CMakeFiles/backendA.dir/backend-serverA.cpp.o
backendA: CMakeFiles/backendA.dir/build.make
backendA: CMakeFiles/backendA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable backendA"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/backendA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/backendA.dir/build: backendA

.PHONY : CMakeFiles/backendA.dir/build

CMakeFiles/backendA.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/backendA.dir/cmake_clean.cmake
.PHONY : CMakeFiles/backendA.dir/clean

CMakeFiles/backendA.dir/depend:
	cd /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug /Users/Steven/Desktop/USC/2020Fall/EE450/socket_project/project_code/cmake-build-debug/CMakeFiles/backendA.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/backendA.dir/depend

