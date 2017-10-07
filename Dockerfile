FROM polyhx/cpp-server

ADD . .

EXPOSE 3000

RUN make
CMD ["./ai"]
