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
CMAKE_SOURCE_DIR = /home/victor/4o/IG/IG_T1/bibliotecas

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/victor/4o/IG/IG_T1/bibliotecas/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/bibliotecas.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bibliotecas.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bibliotecas.dir/flags.make

CMakeFiles/bibliotecas.dir/main.cpp.o: CMakeFiles/bibliotecas.dir/flags.make
CMakeFiles/bibliotecas.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/victor/4o/IG/IG_T1/bibliotecas/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bibliotecas.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bibliotecas.dir/main.cpp.o -c /home/victor/4o/IG/IG_T1/bibliotecas/main.cpp

CMakeFiles/bibliotecas.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bibliotecas.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/victor/4o/IG/IG_T1/bibliotecas/main.cpp > CMakeFiles/bibliotecas.dir/main.cpp.i

CMakeFiles/bibliotecas.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bibliotecas.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/victor/4o/IG/IG_T1/bibliotecas/main.cpp -o CMakeFiles/bibliotecas.dir/main.cpp.s

# Object files for target bibliotecas
bibliotecas_OBJECTS = \
"CMakeFiles/bibliotecas.dir/main.cpp.o"

# External object files for target bibliotecas
bibliotecas_EXTERNAL_OBJECTS =

bibliotecas: CMakeFiles/bibliotecas.dir/main.cpp.o
bibliotecas: CMakeFiles/bibliotecas.dir/build.make
bibliotecas: CMakeFiles/bibliotecas.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/victor/4o/IG/IG_T1/bibliotecas/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bibliotecas"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bibliotecas.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bibliotecas.dir/build: bibliotecas

.PHONY : CMakeFiles/bibliotecas.dir/build

CMakeFiles/bibliotecas.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bibliotecas.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bibliotecas.dir/clean

CMakeFiles/bibliotecas.dir/depend:
	cd /home/victor/4o/IG/IG_T1/bibliotecas/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/4o/IG/IG_T1/bibliotecas /home/victor/4o/IG/IG_T1/bibliotecas /home/victor/4o/IG/IG_T1/bibliotecas/cmake-build-debug /home/victor/4o/IG/IG_T1/bibliotecas/cmake-build-debug /home/victor/4o/IG/IG_T1/bibliotecas/cmake-build-debug/CMakeFiles/bibliotecas.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bibliotecas.dir/depend

