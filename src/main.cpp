// @date 2023.8
// cvdong

#include "application/app_yolov8_det.h"
#include "application/app_yolov8_seg.h"
#include "application/app_yolov8_pose.h"

int main(int argc, char *argv[])
{
    ai::arg_parsing::Settings s;
    if (parseArgs(argc, argv, &s) == RETURN_FAIL)
    {
        INFO("Failed to parse the args\n");
        return RETURN_FAIL;
    }
    ai::arg_parsing::printArgs(&s);

    CHECK(cudaSetDevice(s.device_id)); // 设置你用哪块gpu

    // 推理任务选择
    // det
    if (s.infer_task == "yolov8_det")
    {
        if (s.perf)
        {
            yolov8_trt_inference_perf(&s);
        }
        else
        {
            yolov8_trt_inference(&s);
        }
    }
    // seg
    else if (s.infer_task == "yolov8_seg")
    {
        yolov8_seg_trt_inference(&s);
    }
    // pose
    else if (s.infer_task == "yolov8_pose")
    {
        yolov8_pose_trt_inference(&s);
    }
 
    return RETURN_SUCCESS;
}