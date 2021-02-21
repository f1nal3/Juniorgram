FROM gcc:latest as build

COPY . /usr/src/build

WORKDIR /usr/src/build

RUN apt-get update && apt-get upgrade -y && apt-get install -y build-essential make bash git python3 \
libx11-xcb-dev libxcb-dri3-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev \
libxcb-randr0-dev libxcb-render-util0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-util0-dev \
libxcb-xfixes0-dev libxcb-xinerama0-dev libxcb-xkb-dev xkb-data xorg-dev
RUN wget -qO- https://bootstrap.pypa.io/get-pip.py | python3 \
&& pip3 install conan cmake mako

RUN cmake -DCMAKE_BUILD_TYPE=Release -DCONAN_SETUP=None . \
&& cmake --build . 
