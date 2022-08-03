#pragma once
//
// Create by RangiLyu
// 2020 / 10 / 2
//

#ifndef NANODET_H
#define NAMODET_H

#include "opencv2/imgproc/imgproc.hpp"
#include "net.h"

typedef struct HeadInfo
{
    std::string cls_layer;
    std::string dis_layer;
    int stride;
};

typedef struct BoxInfo
{
    float x1;
    float y1;
    float x2;
    float y2;
    float score;
    int label;
} BoxInfo;

class NanoDet
{
public:
    NanoDet(const char* param, const char* bin, bool useGPU);

    ~NanoDet();

    static NanoDet* detector;
    ncnn::Net* Net;
    static bool hasGPU;

//*************************************************************************
public:
    //模型必填项 输出layer
    std::vector<HeadInfo> heads_info{
        // cls_pred|dis_pred|stride
            //{"792", "795",    8},
            //{"814", "817",   16},
            //{"836", "839",   32},
            {"cls_pred_stride_8", "dis_pred_stride_8",    8},
            {"cls_pred_stride_16", "dis_pred_stride_16",   16},
            {"cls_pred_stride_32", "dis_pred_stride_32",   32},
    };
    std::string inputName = "input.1";//模型必填项 输入name
    const float mean_vals[3] = { 104.04f, 113.9f, 119.8f };             //必选项 inputdata预处理
    const float norm_vals[3] = { 0.013569f, 0.014312f, 0.014106f };//必选项 inputdata预处理
    int input_size = 320;    //模型必填项 input size
    int num_class = 2;//80; //模型必填项 num class
    int reg_max = 7;//7;    //模型选填项,可选默认
    //float score_threshold = 0.5; //模型选填项 anchor置信度阈值
    //float nms_threshold = 0.5;   //模型选填项 NMS阈值
//*************************************************************************

public:
    std::vector<BoxInfo> detect(cv::Mat image, float score_threshold = 0.5, float nms_threshold=0.5);
    void draw(cv::Mat& image, std::vector<BoxInfo> boxes);//在图像上绘制矩形
    std::vector<std::string> labels{ "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
                                    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
                                    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
                                    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
                                    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
                                    "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
                                    "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
                                    "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
                                    "hair drier", "toothbrush" };
private:
    void preprocess(cv::Mat& image, ncnn::Mat& in);
    void preprocess(
        cv::Mat& image, //in 原始图像
        float dst_size,  //in 输出图像的宽(宽高均为此值)
        ncnn::Mat& in,  //out,喂入推断网络的数据
        float& wpad,      //out,x项填充的列数
        float& hpad,      //out,y向填充的行数
        float& scale    //out,图像(宽或高)缩放的系数
    );
    void decode_infer(ncnn::Mat& cls_pred, ncnn::Mat& dis_pred, int stride, float threshold, std::vector<std::vector<BoxInfo>>& results);
    BoxInfo disPred2Bbox(const float*& dfl_det, int label, float score, int x, int y, int stride);
    static void nms(std::vector<BoxInfo>& result, float nms_threshold);

};


#endif //NANODET_H


