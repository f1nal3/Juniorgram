[![Windows Build](https://github.com/f1nal3/Juniorgram/actions/workflows/Windows%20Build.yml/badge.svg?branch=master)](https://github.com/f1nal3/Juniorgram/actions/workflows/Windows%20Build.yml)
[![Linux GCC Release Build](https://github.com/f1nal3/Juniorgram/actions/workflows/Linux%20GCC%20Release%20Build.yml/badge.svg)](https://github.com/f1nal3/Juniorgram/actions/workflows/Linux%20GCC%20Release%20Build.yml)
[![Documentation](https://github.com/f1nal3/Juniorgram/actions/workflows/documentation.yml/badge.svg?branch=master)](https://github.com/f1nal3/Juniorgram/actions/workflows/documentation.yml)

![](https://i.ibb.co/sC8WJQs/logo2.png)

# Introduction 
A C++ messenger for learning purposes in EPAM Brest C++ R&D Lab

# [Documentation](https://f1nal3.github.io/Juniorgram/)

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
 
 - Set your CMAKE_PREFIX_PATH and QT_QPA_PLATFORM_PLUGIN_PATH environment variable to the Qt5 installation prefix.
 Example:
 
 ![image](https://user-images.githubusercontent.com/57706182/117034264-ca95ea00-ad0b-11eb-8e64-5e38bf20a305.png)
 ![image](https://user-images.githubusercontent.com/57706182/117034299-d4b7e880-ad0b-11eb-9165-f911b13a5e7c.png)

 - You may install Qt Desktop or other versions.

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

 
