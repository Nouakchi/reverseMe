# Use an official Ubuntu as a base image
FROM ubuntu:22.04

# Install dependencies for disassembling, debugging, and running the program
RUN apt-get update && apt-get install -y binutils \
    libc6-i386 \
    vim \
    ltrace \
    gdb

WORKDIR /workspace

COPY . .

ENTRYPOINT ["/bin/bash"]
