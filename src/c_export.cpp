#include <iostream>
#include <exception>
#include <thread>
#include <chrono>
#include "singleton.h"
#include "ThreadPool.h"
#include "predictor.h"

extern "C" {
    ThreadPool pool(1);
    Predictor* predictor = Singleton<Predictor>::Instance();

    bool is_destroyed() {
        return predictor->isDestroyed();
    }

    // std::vector<double>* predict(unsigned char *img_data, int img_h, int img_w) {
    //     std::vector<double> *ret = nullptr;
    //     try {
    //     cv::Mat img(img_h, img_w, CV_8UC3, (void*)img_data);
    //     ret = predictor->predict(img);
    //         return ret;
    //     } catch(const std::exception& e) {
    //         std::cerr << e.what() << '\n';
    //         return ret;
    //     }
    // }

    PyObject* predict(unsigned char *img_data, int img_h, int img_w) {
        PyObject* ret = PyList_New(0);
        PyList_Append(ret, PyFloat_FromDouble((double)0));
        PyList_Append(ret, PyFloat_FromDouble((double)0));
        try {
        cv::Mat img(img_h, img_w, CV_8UC3, (void*)img_data);
        ret = predictor->Predict(img);
            return ret;
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
            return ret;
        }
    }

}
