# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/victor/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/192.6817.18/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/victor/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/192.6817.18/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/victor/gitRepos/IG_T1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/victor/gitRepos/IG_T1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/IG_T1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/IG_T1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/IG_T1.dir/flags.make

CMakeFiles/IG_T1.dir/IG_T3/main.cpp.o: CMakeFiles/IG_T1.dir/flags.make
CMakeFiles/IG_T1.dir/IG_T3/main.cpp.o: ../IG_T3/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victor/gitRepos/IG_T1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/IG_T1.dir/IG_T3/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/IG_T1.dir/IG_T3/main.cpp.o -c /home/victor/gitRepos/IG_T1/IG_T3/main.cpp

CMakeFiles/IG_T1.dir/IG_T3/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/IG_T1.dir/IG_T3/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victor/gitRepos/IG_T1/IG_T3/main.cpp > CMakeFiles/IG_T1.dir/IG_T3/main.cpp.i

CMakeFiles/IG_T1.dir/IG_T3/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/IG_T1.dir/IG_T3/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victor/gitRepos/IG_T1/IG_T3/main.cpp -o CMakeFiles/IG_T1.dir/IG_T3/main.cpp.s

# Object files for target IG_T1
IG_T1_OBJECTS = \
"CMakeFiles/IG_T1.dir/IG_T3/main.cpp.o"

# External object files for target IG_T1
IG_T1_EXTERNAL_OBJECTS =

IG_T1: CMakeFiles/IG_T1.dir/IG_T3/main.cpp.o
IG_T1: CMakeFiles/IG_T1.dir/build.make
IG_T1: CMakeFiles/IG_T1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/victor/gitRepos/IG_T1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable IG_T1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/IG_T1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/IG_T1.dir/build: IG_T1

.PHONY : CMakeFiles/IG_T1.dir/build

CMakeFiles/IG_T1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/IG_T1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/IG_T1.dir/clean

CMakeFiles/IG_T1.dir/depend:
	cd /home/victor/gitRepos/IG_T1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/gitRepos/IG_T1 /home/victor/gitRepos/IG_T1 /home/victor/gitRepos/IG_T1/cmake-build-debug /home/victor/gitRepos/IG_T1/cmake-build-debug /home/victor/gitRepos/IG_T1/cmake-build-debug/CMakeFiles/IG_T1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/IG_T1.dir/depend

