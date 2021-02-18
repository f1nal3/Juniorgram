FROM frolvlad/alpine-glibc

COPY . /usr/src/build

WORKDIR /usr/src/build

RUN apk update && apk upgrade && apk add --no-cache qt5-qtbase-dev qt5-qtbase-x11 qt5-qtbase-dbg cmake make gcc g++ linux-headers bash git python3 
RUN wget -qO- https://bootstrap.pypa.io/get-pip.py | python3 \
&& pip3 install conan

RUN cmake -DCMAKE_BUILD_TYPE=Release . \
&& cmake --build . 

CMD ["./bin/Gui_Qt"]
