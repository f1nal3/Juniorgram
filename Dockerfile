FROM gcc:10 as build

COPY . /usr/src/build

WORKDIR /usr/src/build

RUN apt-get update && apt-get upgrade -y && apt-get install -y make bash git python3 \
libx11-xcb-dev libxcb-dri3-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev \
libxcb-randr0-dev libxcb-render-util0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-util0-dev \
libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-xkb-dev xkb-data xorg-dev libxcb-dri2-0-dev libxcb-present-dev libxcb-glx0-dev \
libgl1-mesa-dev && g++ --version

RUN wget -qO- https://bootstrap.pypa.io/get-pip.py | python3 \
&& pip3 install conan==1.36.0 cmake mako aqtinstall

RUN aqt install --outputdir /opt/Qt 5.15.2 linux desktop && export CMAKE_PREFIX_PATH=/opt/Qt/5.15.2/gcc_64/lib/cmake/Qt5/:$CMAKE_PREFIX_PATH && conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan && /usr/local/bin/conan install /usr/src/build/conanfile.txt  \
-s build_type=Release \
-s compiler=gcc -s compiler.libcxx=libstdc++11 -g=cmake --build=missing 
