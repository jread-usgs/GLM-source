# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8


models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.requires: models/niva/CMakeFiles/fabm_models_niva.dir/fabm_niva_oxydep.mod.proxy
models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o: models/niva/CMakeFiles/fabm_models_niva.dir/fabm_niva_oxydep.mod.stamp
models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o: modules/fabm_types.mod
models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.mod.proxy: models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.provides
models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.provides.build:
	$(CMAKE_COMMAND) -E cmake_copy_f90_mod models/niva/niva_model_library models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.mod.stamp Intel
	$(CMAKE_COMMAND) -E touch models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.provides.build
models/niva/CMakeFiles/fabm_models_niva.dir/build: models/niva/CMakeFiles/fabm_models_niva.dir/niva_model_library.F90.o.provides.build
models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o: /home/casper/Work/AED_PUBLIC_2.0.0alpha6/FABM-new/fabm-git/src/../include/fabm.h
models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o: /home/casper/Work/AED_PUBLIC_2.0.0alpha6/FABM-new/fabm-git/src/drivers/gotm/fabm_driver.h

models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o: modules/fabm_types.mod
models/niva/CMakeFiles/fabm_models_niva.dir/fabm_niva_oxydep.mod.proxy: models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.provides
models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.provides.build:
	$(CMAKE_COMMAND) -E cmake_copy_f90_mod models/niva/fabm_niva_oxydep models/niva/CMakeFiles/fabm_models_niva.dir/fabm_niva_oxydep.mod.stamp Intel
	$(CMAKE_COMMAND) -E touch models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.provides.build
models/niva/CMakeFiles/fabm_models_niva.dir/build: models/niva/CMakeFiles/fabm_models_niva.dir/oxydep/oxydep.F90.o.provides.build
