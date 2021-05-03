![build](https://github.com/f1nal3/Juniorgram/workflows/build/badge.svg?branch=master)  

![](https://i.ibb.co/sC8WJQs/logo2.png)

# Introduction 
A C++ messenger for learning purposes in EPAM Brest C++ R&D Lab

# Getting Started

There are a few steps to get up and running on own system:

#### 1. Software dependencies

<font size="4">[Conan](https://conan.io/index.html)<font size/>

This is an open source, decentralized and multi-platform package manager for C/C++.

Recommended install:

1. Install Python: 

 - Windows: [Python 3.8.7.exe](https://www.python.org/ftp/python/3.8.7/python-3.8.7-amd64.exe)

 - Linux: 
```$ sudo apt-get update ```
```$ sudo apt-get install python3.8```

2. Install Conan: 

 - Windows/Linux: ``` $ pip install conan```

3. Install Qt 5.15.2
 - Recommended:
   - install [aqtinstall](https://pypi.org/project/aqtinstall/): ``` $ pip install aqtinstall```
   - using aqtinstall get Qt (see the link above for the reference)
 
 - Set your CMAKE_PREFIX_PATH environment variable to the Qt 5 installation prefix.
 Example: ``` C:\Qt5\ ```
 
 - You may install Qt Desktop or other versions if you want so or already have one.

---

<font size="4">[CMake](https://cmake.org)<font size/>

CMake is a cross-platform, open-source build system generator. 

Install CMake:

 - Windows: [cmake-3.19.3-win64-x64.msi](https://github.com/Kitware/CMake/releases/download/v3.19.3/cmake-3.19.3-win32-x86.msi)

 - Linux: [cmake-3.19.3-Linux-x86_64.sh](https://github.com/Kitware/CMake/releases/download/v3.19.3/cmake-3.19.3-Linux-x86_64.sh)

#### 2. Installation process

<font size="4">Windows:<font size/>

<font size="4">Visual Studio 2019: <font size/>
 1. Clone this repository or download as Zip from Github.
 2. Run Visual Studio and open this repository as folder. It will automatically detect the root CMakeLists.txt and execute it, after which you can start/debug the server and client sides.

<font size="4">Linux:<font size/>

Qt Creator:

 1. Clone this repository or download as Zip from Github.
 2. Run Qt Creator and open the root CMakeLists.txt. It will automatically detect the root CMakeLists.txt and execute it, after which you can start/debug the server and client sides.

 
