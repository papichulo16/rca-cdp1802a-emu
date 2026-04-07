FROM ubuntu
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y make gcc flex git libyaml-dev

RUN git clone https://github.com/tlsa/libcyaml.git /tmp/libcyaml \
    && cd /tmp/libcyaml \
    && make \
    && make install \
    && ldconfig \
    && rm -rf /tmp/libcyaml

RUN rm -rf /var/lib/apt/lists/*

VOLUME /root/env
WORKDIR /root/env

