#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include <python2.7/Python.h>
#include "RrConfig.h"
#include "mxnet-cpp/MxNetCpp.h"
#include <opencv2/opencv.hpp>

using namespace mxnet::cpp;

class Predictor {
 public:
    Predictor();
    ~Predictor();
    void destroy();
    void init();
    bool isDestroyed();
    PyObject* Predict(cv::Mat rgb);

 private:
    void LoadParameters(std::string model_name);
    void LoadModel();
    void GetModelOutput(std::string symbol_name);
    int GetGender(NDArray model_output);
    int GetAge(NDArray model_output);
    NDArray Mat2NDArray(cv::Mat image);
    std::map<std::string, NDArray> args_map;
    std::map<std::string, NDArray> aux_map;
    Symbol net;
    Executor *executor;
    Context ctx_cpu = Context::cpu();
    rr::RrConfig config;
    bool destroyed = true;
    int image_size = 112;
    std::string output_layer = "fc1_output";
};
#endif
