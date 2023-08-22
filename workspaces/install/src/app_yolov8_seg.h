#ifndef _MAIN_YOLOV8_SEG_H
#define _MAIN_YOLOV8_SEG_H

#include <opencv2/opencv.hpp>
#include "../include/common/arg_parsing.h"
#include "../include/common/cv_cpp_utils.h"
#include "./yolov8_app/yolov8_seg_app/yolov8_seg_detctor.h"

void yolov8_seg_trt_inference(ai::arg_parsing::Settings* s);

#endif