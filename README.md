# SlimValue.hpp

A modern C++ type system providing flexible, type-safe value containers with integrated error handling and key-value storage.

---

## Features

- **Type-Safe Value Container** — Hold multiple data types in a single, unified interface
- **Integrated Error Handling** — Combine values with error codes and messages
- **Key-Value Storage** — Single and multi-value map structures built-in
- **Zero-Cost Abstractions** — Leverages modern C++ for minimal runtime overhead
- **Automatic Type Conversion** — Smart casting for compatible types
- **Lazy Initialization** — Maps created on-demand

---

## Build & Install

This project uses a Makefile that delegates to CMake.

### Build environment

Builds should be performed either:

- on the native target platform, or  
- inside a fully functional container matching the target system (e.g. Debian for `.deb`, Fedora/RHEL for `.rpm`)

## Docker Build Environment

A Dockerfile can be used to provide a consistent build environment for generating both `.deb` and `.rpm` packages.

### Example Dockerfile

```dockerfile
FROM ubuntu:26.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    rpm \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . .

```

Build container
```
docker build -t slimvalue-package-maker .
```

Build packages
```
docker run -it --rm -v $PWD:/dist slimvalue-package-maker bash -c "cd /workspace && make packages LOCAL_SRC=OFF DIST_DIR=/dist"
```

### Build

``` bash
make build
```

### Install

``` bash
make install
```

Default install prefix: - /usr/local

Custom prefix:

``` bash
make install INSTALL_PREFIX=/your/path
```
