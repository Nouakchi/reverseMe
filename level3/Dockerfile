FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    binutils \
    libc6-i386 \
    vim \
    file \
    ltrace \
    gdb \
    curl \
    python3 \
    git \
    ca-certificates \
    python3-pip \
    locales


# Optional: Upgrade pip and install Python packages for GEF
RUN pip3 install --upgrade pip \
    && pip3 install pygments keystone-engine unicorn capstone ropper

# Install GEF (GDB Enhanced Features)
RUN curl -fsSL https://gef.blah.cat/sh | bash

# Set working directory
WORKDIR /workspace

# Copy contents into the container
COPY . .

# Default entry point
ENTRYPOINT ["/bin/bash"]

