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
include CMakeFiles/test_ei_application.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_ei_application.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_ei_application.dir/flags.make

CMakeFiles/test_ei_application.dir/units/test_ei_application.c.o: CMakeFiles/test_ei_application.dir/flags.make
CMakeFiles/test_ei_application.dir/units/test_ei_application.c.o: ../units/test_ei_application.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nicolas/ensimag/projet-c/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test_ei_application.dir/units/test_ei_application.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_ei_application.dir/units/test_ei_application.c.o   -c /home/nicolas/ensimag/projet-c/units/test_ei_application.c

CMakeFiles/test_ei_application.dir/units/test_ei_application.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_ei_application.dir/units/test_ei_application.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/nicolas/ensimag/projet-c/units/test_ei_application.c > CMakeFiles/test_ei_application.dir/units/test_ei_application.c.i

CMakeFiles/test_ei_application.dir/units/test_ei_application.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_ei_application.dir/units/test_ei_application.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/nicolas/ensimag/projet-c/units/test_ei_application.c -o CMakeFiles/test_ei_application.dir/units/test_ei_application.c.s

# Object files for target test_ei_application
test_ei_application_OBJECTS = \
"CMakeFiles/test_ei_application.dir/units/test_ei_application.c.o"

# External object files for target test_ei_application
test_ei_application_EXTERNAL_OBJECTS =

test_ei_application: CMakeFiles/test_ei_application.dir/units/test_ei_application.c.o
test_ei_application: CMakeFiles/test_ei_application.dir/build.make
test_ei_application: libei.a
test_ei_application: ../_x11/libeibase64.a
test_ei_application: CMakeFiles/test_ei_application.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nicolas/ensimag/projet-c/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_ei_application"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_ei_application.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_ei_application.dir/build: test_ei_application

.PHONY : CMakeFiles/test_ei_application.dir/build

CMakeFiles/test_ei_application.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_ei_application.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_ei_application.dir/clean

CMakeFiles/test_ei_application.dir/depend:
	cd /home/nicolas/ensimag/projet-c/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nicolas/ensimag/projet-c /home/nicolas/ensimag/projet-c /home/nicolas/ensimag/projet-c/cmake /home/nicolas/ensimag/projet-c/cmake /home/nicolas/ensimag/projet-c/cmake/CMakeFiles/test_ei_application.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_ei_application.dir/depend

