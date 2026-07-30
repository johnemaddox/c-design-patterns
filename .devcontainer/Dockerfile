FROM ubuntu:24.04

RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y --no-install-recommends \
        clang \
        clangd \
        clang-tidy \
        lld \
        lldb \
        libllvmlibc-18-dev \
        cmake \
        ninja-build \
        vim \
        git \
    && rm -rf /var/lib/apt/lists/*

# login as user
USER ubuntu

