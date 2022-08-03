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
	std::string inputName = "input";//ģ�ͱ����� ����name
    float input_size = 224;    //ģ�ͱ����� input size
	std::string outputName_points = "points";  //��ѡ�� net output name
	std::string outputName_score = "score";    //��ѡ�� net output name
	const float mean_vals[3] = { 104.04f, 113.9f, 119.8f };             //��ѡ�� inputdataԤ���� 104.04f, 113.9f, 119.8f
	const float norm_vals[3] = { 0.00392f, 0.00392f, 0.00392f };//��ѡ�� inputdataԤ���� 0.013569f, 0.014312f, 0.014106f ,
public:
	bool detect(
		cv::Mat& image,						//ԭʼͼ��(input)
		std::vector<cv::Rect>& rects,		//���ο򼯺�(input)
		std::vector<plamObject>& objects,	//����21�����Ƶļ���(output)
		float score_threshold				//�÷���ֵ(input)
	);

	bool draw(
		cv::Mat& image,
		const std::vector<plamObject>& objects
	);

private:
    bool preprocess(
		cv::Mat& image, //in ԭʼͼ��
		cv::Rect& rec,  //in ����
		ncnn::Mat& in,  //out,ι���ƶ����������
		float& halfWpad,      //out,x����������/2
		float& halfHpad,      //out,y����������/2
		float& scale    //out,ͼ��(����)���ŵ�ϵ��
	);
};

