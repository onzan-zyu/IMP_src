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
CMAKE_SOURCE_DIR = /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build

# Include any dependencies generated for this target.
include CMakeFiles/hycube_simulator_lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hycube_simulator_lib.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hycube_simulator_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hycube_simulator_lib.dir/flags.make

CMakeFiles/hycube_simulator_lib.dir/CGRA.o: CMakeFiles/hycube_simulator_lib.dir/flags.make
CMakeFiles/hycube_simulator_lib.dir/CGRA.o: ../CGRA.cpp
CMakeFiles/hycube_simulator_lib.dir/CGRA.o: CMakeFiles/hycube_simulator_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hycube_simulator_lib.dir/CGRA.o"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hycube_simulator_lib.dir/CGRA.o -MF CMakeFiles/hycube_simulator_lib.dir/CGRA.o.d -o CMakeFiles/hycube_simulator_lib.dir/CGRA.o -c /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/CGRA.cpp

CMakeFiles/hycube_simulator_lib.dir/CGRA.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hycube_simulator_lib.dir/CGRA.i"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/CGRA.cpp > CMakeFiles/hycube_simulator_lib.dir/CGRA.i

CMakeFiles/hycube_simulator_lib.dir/CGRA.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hycube_simulator_lib.dir/CGRA.s"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/CGRA.cpp -o CMakeFiles/hycube_simulator_lib.dir/CGRA.s

CMakeFiles/hycube_simulator_lib.dir/CGRATile.o: CMakeFiles/hycube_simulator_lib.dir/flags.make
CMakeFiles/hycube_simulator_lib.dir/CGRATile.o: ../CGRATile.cpp
CMakeFiles/hycube_simulator_lib.dir/CGRATile.o: CMakeFiles/hycube_simulator_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/hycube_simulator_lib.dir/CGRATile.o"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hycube_simulator_lib.dir/CGRATile.o -MF CMakeFiles/hycube_simulator_lib.dir/CGRATile.o.d -o CMakeFiles/hycube_simulator_lib.dir/CGRATile.o -c /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/CGRATile.cpp

CMakeFiles/hycube_simulator_lib.dir/CGRATile.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hycube_simulator_lib.dir/CGRATile.i"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/CGRATile.cpp > CMakeFiles/hycube_simulator_lib.dir/CGRATile.i

CMakeFiles/hycube_simulator_lib.dir/CGRATile.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hycube_simulator_lib.dir/CGRATile.s"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/CGRATile.cpp -o CMakeFiles/hycube_simulator_lib.dir/CGRATile.s

CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.o: CMakeFiles/hycube_simulator_lib.dir/flags.make
CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.o: ../IMP/IPD.cpp
CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.o: CMakeFiles/hycube_simulator_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.o"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.o -MF CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.o.d -o CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.o -c /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/IMP/IPD.cpp

CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.i"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/IMP/IPD.cpp > CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.i

CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.s"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/IMP/IPD.cpp -o CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.s

CMakeFiles/hycube_simulator_lib.dir/SPM/spm.o: CMakeFiles/hycube_simulator_lib.dir/flags.make
CMakeFiles/hycube_simulator_lib.dir/SPM/spm.o: ../SPM/spm.cpp
CMakeFiles/hycube_simulator_lib.dir/SPM/spm.o: CMakeFiles/hycube_simulator_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/hycube_simulator_lib.dir/SPM/spm.o"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hycube_simulator_lib.dir/SPM/spm.o -MF CMakeFiles/hycube_simulator_lib.dir/SPM/spm.o.d -o CMakeFiles/hycube_simulator_lib.dir/SPM/spm.o -c /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/SPM/spm.cpp

CMakeFiles/hycube_simulator_lib.dir/SPM/spm.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hycube_simulator_lib.dir/SPM/spm.i"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/SPM/spm.cpp > CMakeFiles/hycube_simulator_lib.dir/SPM/spm.i

CMakeFiles/hycube_simulator_lib.dir/SPM/spm.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hycube_simulator_lib.dir/SPM/spm.s"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/SPM/spm.cpp -o CMakeFiles/hycube_simulator_lib.dir/SPM/spm.s

CMakeFiles/hycube_simulator_lib.dir/hycube_sim.o: CMakeFiles/hycube_simulator_lib.dir/flags.make
CMakeFiles/hycube_simulator_lib.dir/hycube_sim.o: ../hycube_sim.cpp
CMakeFiles/hycube_simulator_lib.dir/hycube_sim.o: CMakeFiles/hycube_simulator_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/hycube_simulator_lib.dir/hycube_sim.o"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hycube_simulator_lib.dir/hycube_sim.o -MF CMakeFiles/hycube_simulator_lib.dir/hycube_sim.o.d -o CMakeFiles/hycube_simulator_lib.dir/hycube_sim.o -c /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/hycube_sim.cpp

CMakeFiles/hycube_simulator_lib.dir/hycube_sim.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hycube_simulator_lib.dir/hycube_sim.i"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/hycube_sim.cpp > CMakeFiles/hycube_simulator_lib.dir/hycube_sim.i

CMakeFiles/hycube_simulator_lib.dir/hycube_sim.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hycube_simulator_lib.dir/hycube_sim.s"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/hycube_sim.cpp -o CMakeFiles/hycube_simulator_lib.dir/hycube_sim.s

CMakeFiles/hycube_simulator_lib.dir/log.o: CMakeFiles/hycube_simulator_lib.dir/flags.make
CMakeFiles/hycube_simulator_lib.dir/log.o: ../log.cpp
CMakeFiles/hycube_simulator_lib.dir/log.o: CMakeFiles/hycube_simulator_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/hycube_simulator_lib.dir/log.o"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hycube_simulator_lib.dir/log.o -MF CMakeFiles/hycube_simulator_lib.dir/log.o.d -o CMakeFiles/hycube_simulator_lib.dir/log.o -c /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/log.cpp

CMakeFiles/hycube_simulator_lib.dir/log.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hycube_simulator_lib.dir/log.i"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/log.cpp > CMakeFiles/hycube_simulator_lib.dir/log.i

CMakeFiles/hycube_simulator_lib.dir/log.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hycube_simulator_lib.dir/log.s"
	/usr/lib/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/log.cpp -o CMakeFiles/hycube_simulator_lib.dir/log.s

# Object files for target hycube_simulator_lib
hycube_simulator_lib_OBJECTS = \
"CMakeFiles/hycube_simulator_lib.dir/CGRA.o" \
"CMakeFiles/hycube_simulator_lib.dir/CGRATile.o" \
"CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.o" \
"CMakeFiles/hycube_simulator_lib.dir/SPM/spm.o" \
"CMakeFiles/hycube_simulator_lib.dir/hycube_sim.o" \
"CMakeFiles/hycube_simulator_lib.dir/log.o"

# External object files for target hycube_simulator_lib
hycube_simulator_lib_EXTERNAL_OBJECTS =

libhycube_simulator_lib.a: CMakeFiles/hycube_simulator_lib.dir/CGRA.o
libhycube_simulator_lib.a: CMakeFiles/hycube_simulator_lib.dir/CGRATile.o
libhycube_simulator_lib.a: CMakeFiles/hycube_simulator_lib.dir/IMP/IPD.o
libhycube_simulator_lib.a: CMakeFiles/hycube_simulator_lib.dir/SPM/spm.o
libhycube_simulator_lib.a: CMakeFiles/hycube_simulator_lib.dir/hycube_sim.o
libhycube_simulator_lib.a: CMakeFiles/hycube_simulator_lib.dir/log.o
libhycube_simulator_lib.a: CMakeFiles/hycube_simulator_lib.dir/build.make
libhycube_simulator_lib.a: CMakeFiles/hycube_simulator_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX static library libhycube_simulator_lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/hycube_simulator_lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hycube_simulator_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hycube_simulator_lib.dir/build: libhycube_simulator_lib.a
.PHONY : CMakeFiles/hycube_simulator_lib.dir/build

CMakeFiles/hycube_simulator_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hycube_simulator_lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hycube_simulator_lib.dir/clean

CMakeFiles/hycube_simulator_lib.dir/depend:
	cd /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build /home/lxf/workspace/zaz/morpher-v2/cppsimulator/src/build/CMakeFiles/hycube_simulator_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hycube_simulator_lib.dir/depend

