cmake_minimum_required(VERSION 3.10.0)
project(infer)
add_definitions(-std=c++11 -w)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/workspaces)
set(CMAKE_INSTALL_PREFIX ${EXECUTABLE_OUTPUT_PATH}/install/) 

# 请设置为显卡对应的号码参考下面的链接，RTX 3090,对应的是sm_86：
# https://developer.nvidia.com/zh-cn/cuda-gpus#compute
set(CUDA_NVCC_FLAGS "-gencode=arch=compute_86,code=sm_86;-G;-g;-O0;-w")

find_package(CUDA REQUIRED) 
find_package(OpenCV REQUIRED) # sudo apt-get install libopencv-dev
include_directories(${EIGEN3_INCLUDE_DIRS})

set(TensorRT_ROOT "/home/Downloads/TensorRT-8.4.1.5") 
# set(OpenCV_DIR   "")
# set(CUDA_DIR     "")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED -O0")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# include所有要用到的hpp文件路径
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CUDA_INCLUDE_DIRS}
    # tensorrt
    ${TensorRT_ROOT}/include
    ${TensorRT_ROOT}/samples/common 
    # 项目里要用到的
    ${PROJECT_SOURCE_DIR}/src/utils
    ${PROJECT_SOURCE_DIR}/src/application
)

# link要用到的so库路径
# 补充：具体的cuda_lib库命名可以看 https://cmake.org/cmake/help/latest/module/FindCUDA.html
link_directories(
    ${CUDA_TOOLKIT_ROOT_DIR}/lib64
    ${TensorRT_ROOT}/lib
)

# 将utils里写好的cu文件和cpp文件编译成so库，方便后面调用
file(GLOB_RECURSE cpp_cuda_srcs
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/application/yolov8_app/yolov8_det_app/*.cpp
    ${PROJECT_SOURCE_DIR}/src/application/yolov8_app/yolov8_seg_app/*.cpp
    ${PROJECT_SOURCE_DIR}/src/application/yolov8_app/yolov8_pose_app/*.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/common/*.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/backend/tensorrt/*.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/preprocess/*.cu
    ${PROJECT_SOURCE_DIR}/src/utils/preprocess/*.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/postprocess/*.cu
    ${PROJECT_SOURCE_DIR}/src/utils/postprocess/*.cpp
    ${TensorRT_ROOT}/samples/common/logger.cpp 
    ${TensorRT_ROOT}/samples/common/sampleOptions.cpp 
    ${TensorRT_ROOT}/samples/common/sampleUtils.cpp
)
cuda_add_library(utils_cu_cpp SHARED ${cpp_cuda_srcs})

add_executable(infer
     ${PROJECT_SOURCE_DIR}/src/application/app_yolov8_det.cpp
     ${PROJECT_SOURCE_DIR}/src/application/app_yolov8_seg.cpp
     ${PROJECT_SOURCE_DIR}/src/application/app_yolov8_pose.cpp

)
# 链接so库
target_link_libraries(infer
    utils_cu_cpp 
    cuda
    cudart
    cudnn
    pthread
    ${OpenCV_LIBS}
    nvinfer 
    nvinfer_plugin
    nvonnxparser    
)

# make install 用到
install(TARGETS infer  utils_cu_cpp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)

install(DIRECTORY
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/utils/backend
        ${PROJECT_SOURCE_DIR}/src/utils/backend/tensorrt
        ${PROJECT_SOURCE_DIR}/src/utils/common
        ${PROJECT_SOURCE_DIR}/src/utils/postprocess
        ${PROJECT_SOURCE_DIR}/src/utils/preprocess
        ${PROJECT_SOURCE_DIR}/src/application/
        ${PROJECT_SOURCE_DIR}/src/application/yolov8_app/yolov8_det_app
        ${PROJECT_SOURCE_DIR}/src/application/yolov8_app/yolov8_seg_app
        ${PROJECT_SOURCE_DIR}/src/application/yolov8_app/yolov8_pose_app
        DESTINATION include/
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h" PATTERN "*.cu")