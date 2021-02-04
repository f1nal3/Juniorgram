FROM alpine:latest

COPY . /usr/src/build

WORKDIR /usr/src/build

RUN apk update && apk upgrade && apk add --no-cache cmake make gcc g++ bash git python3 linux-headers

RUN wget -qO- https://bootstrap.pypa.io/get-pip.py | python3 \
&& pip3 install conan

RUN cmake -DCMAKE_BUILD_TYPE=Release . \
&& cmake --build . 

CMD ["./bin/server"]
