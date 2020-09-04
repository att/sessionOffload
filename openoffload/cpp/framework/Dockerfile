FROM centos:7.6.1810
USER root
RUN mkdir -p /home/grpc/local
WORKDIR /home/grpc/local
ENV GRPC_INSTALL /home/grpc/local
RUN echo $GRPC_INSTALL
RUN yum -y install wget
RUN yum -y group install "Development Tools"
RUN wget --no-check-certificate -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.17.0/cmake-3.17.0-Linux-x86_64.sh
RUN ["sh", "cmake-linux.sh",  "--", "--skip-license", "--prefix=/home/grpc/local"]
RUN rm cmake-linux.sh
ENV PATH="$GRPC_INSTALL/bin:${PATH}"
RUN echo $PATH
RUN git config --global  http.sslVerify false
RUN git clone --recurse-submodules -b v1.30.0 https://github.com/grpc/grpc
RUN cd grpc
RUN mkdir -p /home/grpc/local/grpc/cmake/build
WORKDIR /home/grpc/local/grpc/cmake/build
RUN echo $PATH
#CMD ["sh", "-c", "tail -f /dev/null"]
RUN pwd
RUN cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=/home/grpc/local ../..
RUN make -j 4
RUN make install
RUN cd ../..
#
RUN mkdir /home/grpc/local/tests
WORKDIR /home/grpc/local/tests
COPY *.cc /home/grpc/local/tests/
COPY *.h /home/grpc/local/tests/
COPY tests/src/*.c /home/grpc/local/tests/src/
COPY tests/src/*.cc /home/grpc/local/tests/src/
COPY tests/include/*.h /home/grpc/local/tests/include/
COPY tests/ssl/* /home/grpc/local/tests/ssl/
COPY tests/Makefile_Centos /home/grpc/local/tests/Makefile
RUN mkdir obj
RUN mkdir bin
RUN mkdir log
RUN make
CMD ["sh", "-c", "tail -f /dev/null"]
