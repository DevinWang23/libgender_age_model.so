# An insightface gender age model dynamic lib (Python callable)

## How to use
download the required 3rdparty dynamic lib folder from
[link](https://pan.baidu.com/s/1-eprnH5eic7uuTZ2qRR6jg), extraction
code-**d3hh** put it in project's root path, run `docker build -t
'$IMAGENAME:TAG' .` for building opencv3.4, python2.7, gcc and cmake etc
environment and then run `bash ./run.sh` will create a shared lib -
**./build/libexport_gender_age.so** and run python demo - **predict.py**
at the same time.

## Config file
the config file: **config.ini**, you can change the file paths etc here

## Logger
modify the [LOG]->LEVEL in the **config.ini** to change logging.
```cpp
enum LoggerLevel
{
    none = 0,
    fatal = 1,
    error = 2,
    warning = 3,
    info = 4,
    debug = 5,
    verbose = 6
};
```

## Reference 
[1] insightface:<https://github.com/deepinsight/insightface>  
[2] mxnet c++ inference example:<https://mxnet.apache.org/versions/1.6/api/cpp/docs/tutorials/cpp_inference.html>  
[3] extract feature from pretrained model by mxnet c++ api:<https://blog.csdn.net/muyouhang/article/details/85059352>  
[4] build opencv from source in ubtunu16.04:<https://www.learnopencv.com/install-opencv-4-on-ubuntu-16-04/>  
[5] dockerfile source:<https://hub.docker.com/r/dymat/opencv/dockerfile>  
