#pragma once

#include "nanodetDemo.h"
#include <opencv2/imgproc/imgproc.hpp>
struct plamObject
{
	cv::Rect_<float> rect;
	std::vector<cv::Point2f> pts;
	int label;
	float prob;
};
class handpose
{
public:
	handpose(const char* param, const char* bin, bool useGPU);
	~handpose();
	static NanoDet* detector;
	ncnn::Net* Net;
public:
	std::string inputName = "input";//模型必填项 输入name
    float input_size = 224;    //模型必填项 input size
	std::string outputName_points = "points";  //必选项 net output name
	std::string outputName_score = "score";    //必选项 net output name
	const float mean_vals[3] = { 104.04f, 113.9f, 119.8f };             //必选项 inputdata预处理 104.04f, 113.9f, 119.8f
	const float norm_vals[3] = { 0.00392f, 0.00392f, 0.00392f };//必选项 inputdata预处理 0.013569f, 0.014312f, 0.014106f ,
public:
	bool detect(
		cv::Mat& image,						//原始图像(input)
		std::vector<cv::Rect>& rects,		//矩形框集合(input)
		std::vector<plamObject>& objects,	//包含21点手掌的集合(output)
		float score_threshold				//得分阈值(input)
	);

	bool draw(
		cv::Mat& image,
		const std::vector<plamObject>& objects
	);

private:
    bool preprocess(
		cv::Mat& image, //in 原始图像
		cv::Rect& rec,  //in 矩形
		ncnn::Mat& in,  //out,喂入推断网络的数据
		float& halfWpad,      //out,x项填充的列数/2
		float& halfHpad,      //out,y向填充的行数/2
		float& scale    //out,图像(宽或高)缩放的系数
	);
};

