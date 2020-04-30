# Takatori - A generic SQL intermediate representation

## Requirements

* CMake `>= 3.10`
* C++ Compiler `>= C++17`
* and see *Dockerfile* section

```sh
# retrieve third party modules
git submodule update --init --recursive
```

### Dockerfile

```dockerfile
FROM ubuntu:18.04

RUN apt update -y && apt install -y git build-essential cmake ninja-build libboost-container-dev libicu-dev
```

optional packages:

* `doxygen`
* `graphviz`
* `clang-tidy-8`

### Install sub-modules

#### fpdecimal

```sh
cd third_party/fpdecimal
mkdir -p build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release [-DCMAKE_INSTALL_PREFIX=/path/to/install] -DBUILD_TESTS=OFF -DBUILD_DOCUMENTS=OFF ..
cmake --build . --target install --clean-first
```

## How to build

```sh
mkdir -p build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

available options:

* `-DCMAKE_INSTALL_PREFIX=/path/to/install-root` - change install location
* `-DBUILD_SHARED_LIBS=OFF` - create static libraries instead of shared libraries
* `-DBUILD_TESTS=OFF` - don't build test programs
* `-DBUILD_DOCUMENTS=OFF` - don't build documents by doxygen
* `-DFORCE_INSTALL_RPATH=ON` - automatically configure `INSTALL_RPATH` for non-default library paths
* `-DUSE_BOOST_MEMORY_RESOURCE=OFF` - use standard `<memory_resource>` instead of Boost implementation

### install

```sh
cmake --build . --target install
```

### run tests

```sh
ctest
```

### generate documents

```sh
cmake --build . --target doxygen
```

## License

[Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0)
