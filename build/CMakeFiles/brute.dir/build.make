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
CMAKE_SOURCE_DIR = /home/jacobsayono/multi-agent-case-swap

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jacobsayono/multi-agent-case-swap/build

# Include any dependencies generated for this target.
include CMakeFiles/brute.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/brute.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/brute.dir/flags.make

CMakeFiles/brute.dir/src/node.cpp.o: CMakeFiles/brute.dir/flags.make
CMakeFiles/brute.dir/src/node.cpp.o: ../src/node.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jacobsayono/multi-agent-case-swap/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/brute.dir/src/node.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/brute.dir/src/node.cpp.o -c /home/jacobsayono/multi-agent-case-swap/src/node.cpp

CMakeFiles/brute.dir/src/node.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/brute.dir/src/node.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jacobsayono/multi-agent-case-swap/src/node.cpp > CMakeFiles/brute.dir/src/node.cpp.i

CMakeFiles/brute.dir/src/node.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/brute.dir/src/node.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jacobsayono/multi-agent-case-swap/src/node.cpp -o CMakeFiles/brute.dir/src/node.cpp.s

CMakeFiles/brute.dir/src/tree.cpp.o: CMakeFiles/brute.dir/flags.make
CMakeFiles/brute.dir/src/tree.cpp.o: ../src/tree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jacobsayono/multi-agent-case-swap/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/brute.dir/src/tree.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/brute.dir/src/tree.cpp.o -c /home/jacobsayono/multi-agent-case-swap/src/tree.cpp

CMakeFiles/brute.dir/src/tree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/brute.dir/src/tree.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jacobsayono/multi-agent-case-swap/src/tree.cpp > CMakeFiles/brute.dir/src/tree.cpp.i

CMakeFiles/brute.dir/src/tree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/brute.dir/src/tree.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jacobsayono/multi-agent-case-swap/src/tree.cpp -o CMakeFiles/brute.dir/src/tree.cpp.s

CMakeFiles/brute.dir/src/main.cpp.o: CMakeFiles/brute.dir/flags.make
CMakeFiles/brute.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jacobsayono/multi-agent-case-swap/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/brute.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/brute.dir/src/main.cpp.o -c /home/jacobsayono/multi-agent-case-swap/src/main.cpp

CMakeFiles/brute.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/brute.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jacobsayono/multi-agent-case-swap/src/main.cpp > CMakeFiles/brute.dir/src/main.cpp.i

CMakeFiles/brute.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/brute.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jacobsayono/multi-agent-case-swap/src/main.cpp -o CMakeFiles/brute.dir/src/main.cpp.s

# Object files for target brute
brute_OBJECTS = \
"CMakeFiles/brute.dir/src/node.cpp.o" \
"CMakeFiles/brute.dir/src/tree.cpp.o" \
"CMakeFiles/brute.dir/src/main.cpp.o"

# External object files for target brute
brute_EXTERNAL_OBJECTS =

brute: CMakeFiles/brute.dir/src/node.cpp.o
brute: CMakeFiles/brute.dir/src/tree.cpp.o
brute: CMakeFiles/brute.dir/src/main.cpp.o
brute: CMakeFiles/brute.dir/build.make
brute: CMakeFiles/brute.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jacobsayono/multi-agent-case-swap/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable brute"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/brute.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/brute.dir/build: brute

.PHONY : CMakeFiles/brute.dir/build

CMakeFiles/brute.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/brute.dir/cmake_clean.cmake
.PHONY : CMakeFiles/brute.dir/clean

CMakeFiles/brute.dir/depend:
	cd /home/jacobsayono/multi-agent-case-swap/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jacobsayono/multi-agent-case-swap /home/jacobsayono/multi-agent-case-swap /home/jacobsayono/multi-agent-case-swap/build /home/jacobsayono/multi-agent-case-swap/build /home/jacobsayono/multi-agent-case-swap/build/CMakeFiles/brute.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/brute.dir/depend
