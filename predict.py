# -- coding: utf-8 --

import time
import platform
import ctypes
import json
import time
import os

import numpy as np
import cv2

LIB_SO = './build/libgender_age_model.so'


class Predictor:
    def __init__(self):
        self.__lib = None

    def load_all(self):
        """
        load share lib
        """
        platform_info = str(platform.system()).lower()
        if 'linux' not in platform_info:
            print('not supported system: ', platform_info)
            return False

        if self.__lib is not None:
            return True

        try:
            self.__lib = ctypes.CDLL(LIB_SO)
        except Exception as e:
            print('load shared object error: ', repr(e))
            return False
        return True

    def is_destroyed(self):
        if self.__lib is None:
            return True
        return self.__lib.is_destroyed()

    def predict(self, rgb):
        """
        rgb: numpy array in rgb order 
        """
        start_time = time.time()
        predictor = self.__lib.predict
        predictor.restype = ctypes.py_object
        ret = predictor(
            rgb.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),
            ctypes.c_int(rgb.shape[0]),
            ctypes.c_int(rgb.shape[1])
        )
        end_time = time.time()
        print('inference cost %2.5f seconds.' % (end_time - start_time))
        return ret


def get_timestamp(dt_str):
    timeArray = time.strptime(dt_str, "%Y-%m-%d %H:%M:%S")
    return str(time.mktime(timeArray))


def main():
    """
    this main fun is an simple example for testing libpredictor.so package
    """
    interface = Predictor()
    loaded = interface.load_all()
    if not loaded:
        return

    path = "./test_imgs/"
    image_list = os.listdir(path)
    for img in image_list:
        img_path = os.path.join(path, img)
        img_arr = cv2.imread(img_path)
        ret = list(map(lambda x: int(x), interface.predict(img_arr)))
        print('%s, gender is %s, age is %s' % (img, ret[0], ret[1]))
        # if img.endswith('.npy'):
        #     img_path = os.path.join(path, img)
        #     tmp = np.load(img_path)
        #     ret = list(map(lambda x:int(x),interface.predict(tmp)))
        #     print('%s, gender is %s, age is %s'%(img, ret[0],ret[1]))


if __name__ == '__main__':
    main()
