# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_SOURCE_DIR = /home/himcesjf/Code/Acumen/Assignments/Network_Minimal

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/himcesjf/Code/Acumen/Assignments/Network_Minimal/build

# Utility rule file for server_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/server_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/server_autogen.dir/progress.make

CMakeFiles/server_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/himcesjf/Code/Acumen/Assignments/Network_Minimal/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target server"
	/usr/bin/cmake -E cmake_autogen /home/himcesjf/Code/Acumen/Assignments/Network_Minimal/build/CMakeFiles/server_autogen.dir/AutogenInfo.json ""

server_autogen: CMakeFiles/server_autogen
server_autogen: CMakeFiles/server_autogen.dir/build.make
.PHONY : server_autogen

# Rule to build all files generated by this target.
CMakeFiles/server_autogen.dir/build: server_autogen
.PHONY : CMakeFiles/server_autogen.dir/build

CMakeFiles/server_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server_autogen.dir/clean

CMakeFiles/server_autogen.dir/depend:
	cd /home/himcesjf/Code/Acumen/Assignments/Network_Minimal/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/himcesjf/Code/Acumen/Assignments/Network_Minimal /home/himcesjf/Code/Acumen/Assignments/Network_Minimal /home/himcesjf/Code/Acumen/Assignments/Network_Minimal/build /home/himcesjf/Code/Acumen/Assignments/Network_Minimal/build /home/himcesjf/Code/Acumen/Assignments/Network_Minimal/build/CMakeFiles/server_autogen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/server_autogen.dir/depend

