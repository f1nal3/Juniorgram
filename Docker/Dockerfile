FROM gcc:latest as build

COPY /home/runner/work/Juniorgram/Juniorgram /usr/src/build

WORKDIR /usr/src/build

RUN apt-get update && apt-get upgrade -y && apt-get install -y cmake make bash git python3 
RUN wget -qO- https://bootstrap.pypa.io/get-pip.py | python3 \
&& pip3 install conan

RUN cmake -DCMAKE_BUILD_TYPE=Setup -DCONAN_SETUP=None . \
&& cmake --build . 

ENTRYPOINT 
