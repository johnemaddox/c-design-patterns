FROM gcc:11

RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y --no-install-recommends \
        cmake \
        ninja-build \
        clang-tidy \
        clangd \
        lldb \
        git \
        vim \
    && rm -rf /var/lib/apt/lists/*

# https://code.visualstudio.com/remote/advancedcontainers/add-nonroot-user
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# create user and configure passwordless sudo
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME

# login as user
USER $USERNAME

