# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/casper/Work/AED_PUBLIC_2.0.0alpha6/FABM-new/fabm-git/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build

# Include any dependencies generated for this target.
include models/niva/CMakeFiles/fabm_models_niva.dir/depend.make

# Include the progress variables for this target.
include models/niva/CMakeFiles/fabm_models_niva.dir/progress.make

# Include the compile flags for this target's objects.
include models/niva/CMakeFiles/fabm_models_niva.dir/flags.make

models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o: models/niva/CMakeFiles/fabm_models_niva.dir/flags.make
models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o: /home/casper/Work/AED_PUBLIC_2.0.0alpha6/FABM-new/fabm-git/src/models/niva/niva_model_library.F90
	$(CMAKE_COMMAND) -E cmake_progress_report /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building Fortran object models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o"
	cd /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build/models/niva && /opt/intel/bin/ifort  $(Fortran_DEFINES) $(Fortran_FLAGS) -c /home/casper/Work/AED_PUBLIC_2.0.0alpha6/FABM-new/fabm-git/src/models/niva/niva_model_library.F90 -o CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o

models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.requires:
.PHONY : models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.requires

models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.provides: models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.requires
	$(MAKE) -f models/niva/CMakeFiles/fabm_models_niva.dir/build.make models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.provides.build
.PHONY : models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.provides

models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.provides.build: models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o

models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o: models/niva/CMakeFiles/fabm_models_niva.dir/flags.make
models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o: /home/casper/Work/AED_PUBLIC_2.0.0alpha6/FABM-new/fabm-git/src/models/niva/oxydep/oxydep.F90
	$(CMAKE_COMMAND) -E cmake_progress_report /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building Fortran object models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o"
	cd /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build/models/niva && /opt/intel/bin/ifort  $(Fortran_DEFINES) $(Fortran_FLAGS) -c /home/casper/Work/AED_PUBLIC_2.0.0alpha6/FABM-new/fabm-git/src/models/niva/oxydep/oxydep.F90 -o CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o

models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.requires:
.PHONY : models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.requires

models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.provides: models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.requires
	$(MAKE) -f models/niva/CMakeFiles/fabm_models_niva.dir/build.make models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.provides.build
.PHONY : models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.provides

models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.provides.build: models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o

fabm_models_niva: models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o
fabm_models_niva: models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o
fabm_models_niva: models/niva/CMakeFiles/fabm_models_niva.dir/build.make
.PHONY : fabm_models_niva

# Rule to build all files generated by this target.
models/niva/CMakeFiles/fabm_models_niva.dir/build: fabm_models_niva
.PHONY : models/niva/CMakeFiles/fabm_models_niva.dir/build

models/niva/CMakeFiles/fabm_models_niva.dir/requires: models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.requires
models/niva/CMakeFiles/fabm_models_niva.dir/requires: models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.requires
.PHONY : models/niva/CMakeFiles/fabm_models_niva.dir/requires

models/niva/CMakeFiles/fabm_models_niva.dir/clean:
	cd /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build/models/niva && $(CMAKE_COMMAND) -P CMakeFiles/fabm_models_niva.dir/cmake_clean.cmake
.PHONY : models/niva/CMakeFiles/fabm_models_niva.dir/clean

models/niva/CMakeFiles/fabm_models_niva.dir/depend:
	cd /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/casper/Work/AED_PUBLIC_2.0.0alpha6/FABM-new/fabm-git/src /home/casper/Work/AED_PUBLIC_2.0.0alpha6/FABM-new/fabm-git/src/models/niva /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build/models/niva /home/casper/Work/AED_PUBLIC_2.0.0alpha6/fabm-git/build/models/niva/CMakeFiles/fabm_models_niva.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : models/niva/CMakeFiles/fabm_models_niva.dir/depend

