#include <iostream>
#include <cstring>
#include <sstream>
#include <thread>
#include <chrono>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <typeinfo>

#include "plog/Log.h"
#include "predictor.h"

using namespace std;

Predictor::Predictor() {
    plog::init(
        plog::severityFromString(config.ReadString("LOG", "Level", "DEBUG").c_str()),
        config.ReadString("LOG", "File", "predictor.log").c_str()
    ); // initialize the logger
    PLOG_DEBUG << "====================== Predictor init ================================";
    init();
}

NDArray Predictor::Mat2NDArray(cv::Mat image) {
	vector<float> array;
	vector<cv::Mat> channels;
	cv::split(image,channels);//BGR or RGB
	cv::Mat new_channels[3] = {channels.at(0),channels.at(1),channels.at(2)};
    // cv::Mat &new_channels = channels; 
	cv::Mat merge_image; 
    cv::merge(new_channels,3,merge_image);
	cv::resize(merge_image, image, cv::Size(Predictor::image_size, Predictor::image_size));
	for (int c = 0; c < 3; ++c) {
		for (int i = 0; i < image_size; ++i) {
			for (int j = 0; j < image_size; ++j) {
				array.push_back(static_cast<float>(image.data[(i * image_size + j) * 3 + c]));
			}
		}
	}
	NDArray ret(Shape(1, 3, image_size, image_size), ctx_cpu, false);
	ret.SyncCopyFromCPU(array.data(), 1 * 3 * image_size * image_size);
	NDArray::WaitAll();
	return ret;
}

void Predictor::LoadParameters(std::string model_name) {
	map<string, NDArray> paramters;
	NDArray::Load(model_name, 0, &paramters);
	for (const auto &k : paramters) {
		if (k.first.substr(0, 4) == "aux:") {
			auto name = k.first.substr(4, k.first.size() - 4);
			aux_map[name] = k.second.Copy(ctx_cpu);
		}
		if (k.first.substr(0, 4) == "arg:") {
			auto name = k.first.substr(4, k.first.size() - 4);
			args_map[name] = k.second.Copy(ctx_cpu);
		}
	}
	/*WaitAll is need when we copy data between GPU and the main memory*/
	NDArray::WaitAll();
}

void Predictor::LoadModel() {
    config.ReadConfig("config.ini");
    string modelFile = config.ReadString("MODEL", "File", "");
    PLOG_DEBUG << "model graph: "<<modelFile;
    string modelParams = config.ReadString("MODEL", "Params", "");
    PLOG_DEBUG <<"model params: "<<modelParams;
	GetModelOutput(modelFile);
	LoadParameters(modelParams);
	NDArray data = Mat2NDArray(cv::Mat::zeros(cv::Size(image_size,image_size),CV_8UC3));
	args_map["data"] = data;
	executor = net.SimpleBind(ctx_cpu, args_map, map<string, NDArray>(),
		map<string, OpReqType>(), aux_map);
}


void Predictor::GetModelOutput(string symbol_name) {
	net = Symbol::Load(symbol_name).GetInternals()[output_layer];
}

Predictor::~Predictor() {
    destroy();
}

void Predictor::destroy() {
    // TODO: unload the model
    destroyed = true;
}

void Predictor::init() {
    if (!isDestroyed()) {
        destroy();
    }
    // TODO: load the model here
    destroyed = false;
}

bool Predictor::isDestroyed() {
    return destroyed;
}

int Predictor::GetGender(NDArray model_output){
    if (model_output.At(0,0)>model_output.At(0,1)) {
        return 0;
    }
    return 1;
}

int Predictor::GetAge(NDArray model_output){
    int ret = 0;
    int len = model_output.Size()/2 - 2;
    for (int i =0; i<len;i++){
        if (model_output.At(0,2*i+2)>model_output.At(0,2*i+3)){
            ret += 0;
        }
        else{
            ret += 1;
        }

    }

    return ret;
}

PyObject* Predictor::Predict(cv::Mat rgb) {
    if (rgb.empty()){
        PLOG_DEBUG << "input cannot be empty";
    }

    NDArray data = Mat2NDArray(rgb);
    LoadModel();
    data.CopyTo(&args_map["data"]);
        
    executor->Forward(false);
    auto model_output = executor->outputs[0].Copy(ctx_cpu);
    NDArray::WaitAll();
    
    int gender = GetGender(model_output);
    int age = GetAge(model_output);
   
    PyObject* ret = PyList_New(0);
    PyList_Append(ret, PyFloat_FromDouble((double)gender));  // just cast to double for avoiding PyInt_FronLong bug
    PyList_Append(ret, PyFloat_FromDouble((double)age));

    PLOG_DEBUG << "age is: "<<age;
    PLOG_DEBUG << "gender is: "<<gender;
    PLOG_DEBUG << "====================== Predict Done ======================";
   
    return ret;
}

