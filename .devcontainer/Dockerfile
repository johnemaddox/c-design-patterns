FROM silkeh/clang:18

RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y --no-install-recommends \
        clangd \
        libllvmlibc-18-dev \
        cmake \
        ninja-build \
        vim \
        git \
        ssh \
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

