# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/damiantworek/projects/sysopy/cw05

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/damiantworek/projects/sysopy/cw05/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/cw05.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/cw05.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cw05.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cw05.dir/flags.make

CMakeFiles/cw05.dir/main.c.o: CMakeFiles/cw05.dir/flags.make
CMakeFiles/cw05.dir/main.c.o: ../main.c
CMakeFiles/cw05.dir/main.c.o: CMakeFiles/cw05.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/damiantworek/projects/sysopy/cw05/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cw05.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/cw05.dir/main.c.o -MF CMakeFiles/cw05.dir/main.c.o.d -o CMakeFiles/cw05.dir/main.c.o -c /Users/damiantworek/projects/sysopy/cw05/main.c

CMakeFiles/cw05.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cw05.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/damiantworek/projects/sysopy/cw05/main.c > CMakeFiles/cw05.dir/main.c.i

CMakeFiles/cw05.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cw05.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/damiantworek/projects/sysopy/cw05/main.c -o CMakeFiles/cw05.dir/main.c.s

# Object files for target cw05
cw05_OBJECTS = \
"CMakeFiles/cw05.dir/main.c.o"

# External object files for target cw05
cw05_EXTERNAL_OBJECTS =

cw05: CMakeFiles/cw05.dir/main.c.o
cw05: CMakeFiles/cw05.dir/build.make
cw05: CMakeFiles/cw05.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/damiantworek/projects/sysopy/cw05/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cw05"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cw05.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cw05.dir/build: cw05
.PHONY : CMakeFiles/cw05.dir/build

CMakeFiles/cw05.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cw05.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cw05.dir/clean

CMakeFiles/cw05.dir/depend:
	cd /Users/damiantworek/projects/sysopy/cw05/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/damiantworek/projects/sysopy/cw05 /Users/damiantworek/projects/sysopy/cw05 /Users/damiantworek/projects/sysopy/cw05/cmake-build-debug /Users/damiantworek/projects/sysopy/cw05/cmake-build-debug /Users/damiantworek/projects/sysopy/cw05/cmake-build-debug/CMakeFiles/cw05.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cw05.dir/depend
