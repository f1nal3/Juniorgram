# To build via docker:  
  
Install:  
- [docker](https://www.docker.com/get-started) 
- [docker-compose](https://docs.docker.com/compose/install/)

Inside container:

```sh
docker-compose run shell

# initial setup
rm -rf ./build && mkdir build && cd ./build && cmake .. -DCMAKE_BUILD_TYPE=Release

# rebuild changes
cmake --build .
```

In the host system:
```sh
# Run GUI client:
./build/bin/Client.**
```
