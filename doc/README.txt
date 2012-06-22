Doesn't do much anymore. I didn't like the previous design (or lack of), so I'm 
rewriting alot of the code. Also started using googletest to write unit tests. 
All tests are in the tests directory, and can be built from the Makefile in the 
make directory.

Added a lot of documentation. Using the Doxyfile, the doxygen tool can generate 
html and latex documentation for easier viewing.

Changed project to use C++. Started refactoring the code. Completed refactoring
of the Z80InstructionSet implementation.

Now using cmake to generate build files. To build:
From the root directory, 
mkdir build
cd build
cmake ..
make
This will generate the make file. Any additional changes to the build process
should be made to the CMakeLists.txt file in the root directory. Note tests are
not yet built using cmake.
