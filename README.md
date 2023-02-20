# nash
Compute mixed nash equilibria of RPS-style games via regret minimization.

CMake is required.

To build Debug:
* mkdir Debug
* cd Debug
* cmake -D CMAKE_BUILD_TYPE=Debug ..
* make

To build Release:
* mkdir Release
* cd Release
* cmake -D CMAKE_BUILD_TYPE=Release ..
* make

When running the executable, make sure the working directory is the
project root directory.  e.g., from the project root, type
Debug/src/nash.
