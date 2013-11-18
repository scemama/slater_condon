Slater-Condon
=============

Files for the Slater-Condon paper 2013. The Intel Fortran compiler and the
GNU C compiler are needed.

To run the benchmarks, run

  make tests

This command will build the four benchmarks:

* test_cu_avx
* test_cu_sse2
* test_h2o_avx
* test_h2o_sse2


Source files:
-------------

* nsubst1.x.f90 : Find the number of substitutions between two determinants
* exc0.x.f90 : Dispatch function for getting the excitation operator from determinants
* exc1.x.f90 : Finds the single excitation operator
* exc2.x.f90 : Finds the double excitation operator
* density_matrix.x.f90 : Computation of the one-electron density matrix
* rdtsc.c : Get number of CPU cycles from hardware counter

Data Files
-----------

* cu.dat   : 10 000 determinants obtained from CIPSI in cc-PVDZ basis set for the Cu atom
* cu.coef  : CI Coefficients
* h2o_determinants.dat : 10 000 determinants obtained from CIPSI in cc-PVTZ basis set for the water molecule

