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
CMAKE_SOURCE_DIR = /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/build-constructor-Desktop_Qt_5_12_2_GCC_64bit-Default

# Include any dependencies generated for this target.
include CMakeFiles/constructor.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/constructor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/constructor.dir/flags.make

CMakeFiles/constructor.dir/main.cpp.o: CMakeFiles/constructor.dir/flags.make
CMakeFiles/constructor.dir/main.cpp.o: /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/build-constructor-Desktop_Qt_5_12_2_GCC_64bit-Default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/constructor.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/constructor.dir/main.cpp.o -c /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor/main.cpp

CMakeFiles/constructor.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/constructor.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor/main.cpp > CMakeFiles/constructor.dir/main.cpp.i

CMakeFiles/constructor.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/constructor.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor/main.cpp -o CMakeFiles/constructor.dir/main.cpp.s

CMakeFiles/constructor.dir/constructor.cpp.o: CMakeFiles/constructor.dir/flags.make
CMakeFiles/constructor.dir/constructor.cpp.o: /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor/constructor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/build-constructor-Desktop_Qt_5_12_2_GCC_64bit-Default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/constructor.dir/constructor.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/constructor.dir/constructor.cpp.o -c /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor/constructor.cpp

CMakeFiles/constructor.dir/constructor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/constructor.dir/constructor.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor/constructor.cpp > CMakeFiles/constructor.dir/constructor.cpp.i

CMakeFiles/constructor.dir/constructor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/constructor.dir/constructor.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor/constructor.cpp -o CMakeFiles/constructor.dir/constructor.cpp.s

# Object files for target constructor
constructor_OBJECTS = \
"CMakeFiles/constructor.dir/main.cpp.o" \
"CMakeFiles/constructor.dir/constructor.cpp.o"

# External object files for target constructor
constructor_EXTERNAL_OBJECTS =

constructor: CMakeFiles/constructor.dir/main.cpp.o
constructor: CMakeFiles/constructor.dir/constructor.cpp.o
constructor: CMakeFiles/constructor.dir/build.make
constructor: CMakeFiles/constructor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/build-constructor-Desktop_Qt_5_12_2_GCC_64bit-Default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable constructor"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/constructor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/constructor.dir/build: constructor

.PHONY : CMakeFiles/constructor.dir/build

CMakeFiles/constructor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/constructor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/constructor.dir/clean

CMakeFiles/constructor.dir/depend:
	cd /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/build-constructor-Desktop_Qt_5_12_2_GCC_64bit-Default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/constructor /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/build-constructor-Desktop_Qt_5_12_2_GCC_64bit-Default /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/build-constructor-Desktop_Qt_5_12_2_GCC_64bit-Default /home/tianyu/doc/LearnCpp/C++Notes/c++Primer/construction/build-constructor-Desktop_Qt_5_12_2_GCC_64bit-Default/CMakeFiles/constructor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/constructor.dir/depend
