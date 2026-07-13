# docker image from https://hub.docker.com/r/aminya/setup-cpp-ubuntu 
FROM aminya/setup-cpp-ubuntu-llvm:24.04 AS builder

WORKDIR /filesync