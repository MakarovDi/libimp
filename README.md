# libimp

Building blocks and complete algorithms for image processing.


## Submodules

* **libex**: some C++ extensions https://github.com/MakarovDi/libex;
* **Eigen**: C++ template library for linear algebra http://eigen.tuxfamily.org;

## Requirements ##

#### Linux/MinGW/Cygwin ####

 * CMake >= 3.1
 * Make
 * GCC (with C++14 support) / Clang
 
#### Windows ####

 * CMake >= 3.1
 * Microsoft Visual Studio 2017 Toolchain
  

## Usage

#### Get code

	# git clone https://github.com/MakarovDi/libimp.git imp
	# cd ./imp
	# git checkout v0.1.0
	# git submodule init
	# git submodule update

#### CMake Project [Linux/MinGW/Cygwin] ####

Link it with a project:

``` cmake 
cmake_minimum_required(VERSION 3.1)

project(your_project)


set(CMAKE_CXX_STANDARD 14)


add_subdirectory(imp)

...

add_executable(your_project ...)

target_link_libraries(your_project imp)
```

#### MSVC/Windows ####

VS2017 has cmake support or you can generate project with:

    # cmake -G "Visual Studio 15"


