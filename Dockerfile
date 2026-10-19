FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y \
    gcc-avr \
    binutils-avr \
    avr-libc \
    make \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY . /workspace

CMD ["bash"]