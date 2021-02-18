FROM gcc-latest

COPY . /usr/src/build

WORKDIR /usr/src/build

RUN apk update && apk upgrade && apk add --no-cache cmake make linux-headers bash git python3 
RUN wget -qO- https://bootstrap.pypa.io/get-pip.py | python3 \
&& pip3 install conan

RUN cmake -DCMAKE_BUILD_TYPE=Setup -DCONAN_SETUP=True . \
&& cmake --build . 
