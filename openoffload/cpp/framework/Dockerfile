FROM grpcbuild:v1beta1
#
USER root
ENV PATH="/home/grpc/local/bin:${PATH}"
RUN mkdir -p /home/grpc/local/tests
RUN mkdir -p /home/grpc/local/tests/src/
RUN mkdir -p /home/grpc/local/tests/include/
RUN mkdir -p /home/grpc/local/tests/ssl/
WORKDIR /home/grpc/local/tests
COPY src/*.c /home/grpc/local/tests/src/
COPY src/*.cc /home/grpc/local/tests/src/
COPY include/*.h /home/grpc/local/tests/include/
COPY ssl/* /home/grpc/local/tests/ssl/
COPY Makefile /home/grpc/local/tests/Makefile
RUN mkdir -p /home/protos
COPY openoffload.proto /home/protos/
RUN ls ../../../protos
RUN mkdir -p bin
RUN mkdir -p obj
RUN mkdir -p log
RUN make clean
RUN make HOME=/home/grpc all
CMD ["sh", "-c", "tail -f /dev/null"]
