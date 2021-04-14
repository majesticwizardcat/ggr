FROM gcc

ADD . /ggr
WORKDIR /ggr

RUN make example0

CMD ["/ggr/example0", "64", "16"]
