FROM ubuntu

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y make gcc flex 
RUN rm -rf /var/lib/apt/lists/*

VOLUME /root/env
WORKDIR /root/env

