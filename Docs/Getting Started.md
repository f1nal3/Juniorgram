# Getting Started {#gettingstarted}

There are a few steps to get up and running on own system:

#### 1. Software dependencies

[Conan](https://conan.io/index.html)

This is an open source, decentralized and multi-platform package manager for C/C++.

Recommended install:

1. Install Python: 

 - Windows: [Python 3.8.7.exe](https://www.python.org/ftp/python/3.8.7/python-3.8.7-amd64.exe)

 - Linux: 
``$ sudo apt-get update``
``$ sudo apt-get install python3.8``

2. Install Conan: 

 - Windows/Linux: `` $ pip install conan``

[CMake](https://cmake.org)

CMake is a cross-platform, open-source build system generator. 

Install CMake:

 - Windows: [cmake-3.19.3-win64-x64.msi](https://github.com/Kitware/CMake/releases/download/v3.19.3/cmake-3.19.3-win32-x86.msi)

 - Linux: [cmake-3.19.3-Linux-x86_64.sh](https://github.com/Kitware/CMake/releases/download/v3.19.3/cmake-3.19.3-Linux-x86_64.sh)

#### 2. Installation process

## Windows: 

## Visual Studio 2019/2022:
 1. Clone this repository or download as Zip from Github.
 2. Run Visual Studio and open this repository as folder. It will automatically detect the root CMakeLists.txt and execute it, after which you can start/debug the kernel and client sides.

## Linux:

__Visual Code__:

 1. Clone this repository or download as Zip from Github.
 2. Run VS Code and open the root CMakeLists.txt. It will automatically detect the root CMakeLists.txt and execute it, after which you can start/debug the kernel and client sides.

 #### 3. Workflows and guides

 1. [Workflow](Workflow.md)

 2. [Style guide](Style_guide.md)

 #### 4. Various

 1. [Local kernel setup](Setup kernel on localhost.md)

 2. [Debugging](Improved debugging.md)
