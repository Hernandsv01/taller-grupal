# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/local/Escritorio/taller-grupal/src/client/gui

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/local/Escritorio/taller-grupal/src/client/gui/build

# Include any dependencies generated for this target.
include CMakeFiles/testJazzRunRight.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/testJazzRunRight.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/testJazzRunRight.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testJazzRunRight.dir/flags.make

CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.o: CMakeFiles/testJazzRunRight.dir/flags.make
CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.o: ../testfiles/testJazzRunRight.cpp
CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.o: CMakeFiles/testJazzRunRight.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/local/Escritorio/taller-grupal/src/client/gui/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.o -MF CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.o.d -o CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.o -c /home/local/Escritorio/taller-grupal/src/client/gui/testfiles/testJazzRunRight.cpp

CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/local/Escritorio/taller-grupal/src/client/gui/testfiles/testJazzRunRight.cpp > CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.i

CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/local/Escritorio/taller-grupal/src/client/gui/testfiles/testJazzRunRight.cpp -o CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.s

CMakeFiles/testJazzRunRight.dir/render.cpp.o: CMakeFiles/testJazzRunRight.dir/flags.make
CMakeFiles/testJazzRunRight.dir/render.cpp.o: ../render.cpp
CMakeFiles/testJazzRunRight.dir/render.cpp.o: CMakeFiles/testJazzRunRight.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/local/Escritorio/taller-grupal/src/client/gui/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/testJazzRunRight.dir/render.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/testJazzRunRight.dir/render.cpp.o -MF CMakeFiles/testJazzRunRight.dir/render.cpp.o.d -o CMakeFiles/testJazzRunRight.dir/render.cpp.o -c /home/local/Escritorio/taller-grupal/src/client/gui/render.cpp

CMakeFiles/testJazzRunRight.dir/render.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testJazzRunRight.dir/render.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/local/Escritorio/taller-grupal/src/client/gui/render.cpp > CMakeFiles/testJazzRunRight.dir/render.cpp.i

CMakeFiles/testJazzRunRight.dir/render.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testJazzRunRight.dir/render.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/local/Escritorio/taller-grupal/src/client/gui/render.cpp -o CMakeFiles/testJazzRunRight.dir/render.cpp.s

# Object files for target testJazzRunRight
testJazzRunRight_OBJECTS = \
"CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.o" \
"CMakeFiles/testJazzRunRight.dir/render.cpp.o"

# External object files for target testJazzRunRight
testJazzRunRight_EXTERNAL_OBJECTS =

testJazzRunRight: CMakeFiles/testJazzRunRight.dir/testfiles/testJazzRunRight.cpp.o
testJazzRunRight: CMakeFiles/testJazzRunRight.dir/render.cpp.o
testJazzRunRight: CMakeFiles/testJazzRunRight.dir/build.make
testJazzRunRight: SDL2pp/libSDL2pp.so.8.3.0
testJazzRunRight: /usr/local/lib/libSDL2-2.0.so.0.2600.5
testJazzRunRight: /usr/local/lib/libSDL2main.a
testJazzRunRight: /usr/local/lib/libSDL2_image.so
testJazzRunRight: /usr/local/lib/libSDL2_ttf.so
testJazzRunRight: CMakeFiles/testJazzRunRight.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/local/Escritorio/taller-grupal/src/client/gui/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable testJazzRunRight"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testJazzRunRight.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testJazzRunRight.dir/build: testJazzRunRight
.PHONY : CMakeFiles/testJazzRunRight.dir/build

CMakeFiles/testJazzRunRight.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testJazzRunRight.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testJazzRunRight.dir/clean

CMakeFiles/testJazzRunRight.dir/depend:
	cd /home/local/Escritorio/taller-grupal/src/client/gui/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/local/Escritorio/taller-grupal/src/client/gui /home/local/Escritorio/taller-grupal/src/client/gui /home/local/Escritorio/taller-grupal/src/client/gui/build /home/local/Escritorio/taller-grupal/src/client/gui/build /home/local/Escritorio/taller-grupal/src/client/gui/build/CMakeFiles/testJazzRunRight.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testJazzRunRight.dir/depend

