# Takatori - A generic SQL intermediate representation

## Requirements

* CMake `>= 3.16`
* C++ Compiler `>= C++17`
* and see *Dockerfile* section

```sh
# retrieve third party modules
git submodule update --init --recursive
```

### Dockerfile

```dockerfile
FROM ubuntu:22.04

RUN apt update -y && apt install -y git build-essential cmake ninja-build libboost-container-dev libboost-stacktrace-dev libicu-dev
```

optional packages:

* `doxygen`
* `graphviz`
* `clang-tidy-14`

### Install dependent modules

#### mpdecimal++ (`>= 2.5.1`)

NOTE: in Ubuntu 22.04, `libmpdec-dev` package is available.

```sh
curl https://www.bytereef.org/software/mpdecimal/releases/mpdecimal-2.5.1.tar.gz | tar zxv

cd mpdecimal-2.5.1
./configure --prefix=/path/to/install
make -j4
make install # or sudo make install
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
