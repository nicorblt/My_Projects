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
CMAKE_SOURCE_DIR = /home/nicolas/ensimag/projet-c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nicolas/ensimag/projet-c/cmake

# Include any dependencies generated for this target.
include CMakeFiles/button_limit.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/button_limit.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/button_limit.dir/flags.make

CMakeFiles/button_limit.dir/tests/button_limit.c.o: CMakeFiles/button_limit.dir/flags.make
CMakeFiles/button_limit.dir/tests/button_limit.c.o: ../tests/button_limit.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nicolas/ensimag/projet-c/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/button_limit.dir/tests/button_limit.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/button_limit.dir/tests/button_limit.c.o   -c /home/nicolas/ensimag/projet-c/tests/button_limit.c

CMakeFiles/button_limit.dir/tests/button_limit.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/button_limit.dir/tests/button_limit.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nicolas/ensimag/projet-c/tests/button_limit.c > CMakeFiles/button_limit.dir/tests/button_limit.c.i

CMakeFiles/button_limit.dir/tests/button_limit.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/button_limit.dir/tests/button_limit.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nicolas/ensimag/projet-c/tests/button_limit.c -o CMakeFiles/button_limit.dir/tests/button_limit.c.s

# Object files for target button_limit
button_limit_OBJECTS = \
"CMakeFiles/button_limit.dir/tests/button_limit.c.o"

# External object files for target button_limit
button_limit_EXTERNAL_OBJECTS =

button_limit: CMakeFiles/button_limit.dir/tests/button_limit.c.o
button_limit: CMakeFiles/button_limit.dir/build.make
button_limit: libei.a
button_limit: ../_x11/libeibase64.a
button_limit: CMakeFiles/button_limit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nicolas/ensimag/projet-c/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable button_limit"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/button_limit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/button_limit.dir/build: button_limit

.PHONY : CMakeFiles/button_limit.dir/build

CMakeFiles/button_limit.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/button_limit.dir/cmake_clean.cmake
.PHONY : CMakeFiles/button_limit.dir/clean

CMakeFiles/button_limit.dir/depend:
	cd /home/nicolas/ensimag/projet-c/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nicolas/ensimag/projet-c /home/nicolas/ensimag/projet-c /home/nicolas/ensimag/projet-c/cmake /home/nicolas/ensimag/projet-c/cmake /home/nicolas/ensimag/projet-c/cmake/CMakeFiles/button_limit.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/button_limit.dir/depend

