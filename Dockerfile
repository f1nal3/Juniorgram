FROM gcc:latest as build

COPY . /usr/src/build

WORKDIR /usr/src/build

RUN apt-get update && apt-get upgrade -y && apt-get install -y make bash git python3 
RUN wget -qO- https://bootstrap.pypa.io/get-pip.py | python3 \
&& pip3 install conan cmake

RUN /usr/local/bin/conan install /usr/src/build/conanfile.txt -s build_type=None -s compiler=gcc -s compiler.version=10 -s compiler.libcxx=libstdc++11 -g=cmake --build=missing
