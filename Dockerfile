FROM 	ubuntu:16.04

RUN apt-get update && \
	apt-get upgrade -y && \
	apt-get install -y --no-install-recommends qtbase5-dev python3 python3-dev python-dev python3-pip python3-setuptools build-essential cmake git pkg-config libjpeg8-dev libtiff5-dev libjasper-dev libpng12-dev libgtk2.0-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libatlas-base-dev gfortran libavresample-dev libgphoto2-dev libgstreamer-plugins-base1.0-dev libdc1394-22-dev  && \
	pip3 install numpy==1.18.5 -i https://pypi.tuna.tsinghua.edu.cn/simple/

WORKDIR /home
COPY . .
RUN cd ./opencv-3.4.5/ && \
	mkdir build && \
	cd build && \
	cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      -D INSTALL_C_EXAMPLES=ON \
      -D INSTALL_PYTHON_EXAMPLES=ON \
      -D BUILD_EXAMPLES=ON \
      -D WITH_V4L=ON \
      -D WITH_QT=ON \
      -D OPENCV_EXTRA_MODULES_PATH= ../../opencv_contrib-3.4.5/modules .. && \
	make -j $(nproc) && \
	make install && \
	ldconfig
CMD /bin/bash
