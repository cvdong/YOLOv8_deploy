#ifndef _MAIN_YOLOV8_POSE_H
#define _MAIN_YOLOV8_POSE_H


#include <opencv2/opencv.hpp>
#include "../include/common/arg_parsing.h"
#include "../include/common/cv_cpp_utils.h"
#include "./yolov8_app/yolov8_pose_app/yolov8_pose_detector.h"

void yolov8_pose_trt_inference(ai::arg_parsing::Settings *s);

#endif