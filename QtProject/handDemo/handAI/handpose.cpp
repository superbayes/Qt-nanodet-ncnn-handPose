#include "handpose.h"

handpose::handpose(const char* param, const char* bin, bool useGPU)
{
	this->Net = new ncnn::Net();
    // opt 
    this->Net->opt.use_vulkan_compute =  useGPU;
    this->Net->opt.use_fp16_arithmetic = true;
    this->Net->load_param(param);
    this->Net->load_model(bin);
}

handpose::~handpose()
{
    delete this->Net;
}

bool handpose::detect(
    cv::Mat& image,                 //原始图像(input)
    std::vector<cv::Rect>& rects, //矩形框集合(input)
    std::vector<plamObject>& objects, //包含21点的手掌的集合(output)
    float score_threshold //得分阈值(input)
)
{
    if (rects.empty()||image.empty())return false;
    ncnn::Extractor ex = this->Net->create_extractor();
    ex.set_light_mode(false);
    ex.set_num_threads(4);

    for (int i = 0; i < rects.size(); i++)
    {
        plamObject plam_;
        cv::Rect rec = rects[i];
        ncnn::Mat input;
        float halfWpad, halfHpad,scale;
        //=======================================================
        bool flag_preprocess = preprocess(image, rec, input, halfWpad, halfHpad, scale);
        if(!flag_preprocess)continue;

        ex.input(this->inputName.c_str(), input);
        ncnn::Mat points, score;
        ex.extract("points", points);
        ex.extract("score", score);

        plam_.rect = rec;
        float* points_data = (float*)points.data;
        for (int j = 0; j < 21; j++)
        {
            cv::Point2f pt;
            pt.x = (points_data[j * 3] - ceil(halfWpad)) / scale + (float)rec.x;
            pt.y = (points_data[j * 3 + 1] - ceil(halfHpad)) / scale + (float)rec.y;
            //printf("x=%.5f;y=%.5f;\n", pt.x, pt.y);
            plam_.pts.push_back(pt);
        }
        objects.push_back(plam_);
        //ex.clear();
    }
    return true;
}

bool handpose::draw(cv::Mat& image, const std::vector<plamObject>& objects)
{
    if (image.empty()|| objects.empty())return false;
    int color_index = 0;

    for (int i = 0; i < objects.size(); i++) 
    {
        plamObject obj = objects[i];       
        {
            cv::Mat rgb = image;
            cv::Scalar color1(10, 215, 255);
            cv::Scalar color2(255, 115, 55);
            cv::Scalar color3(5, 255, 55);
            cv::Scalar color4(25, 15, 255);
            cv::Scalar color5(225, 15, 55);
            for (size_t j = 0; j < obj.pts.size(); j++)
            {
                cv::circle(rgb, obj.pts[j], 4, cv::Scalar(255, 0, 0), -1);
                if (j < 4)
                {
                    cv::line(rgb, obj.pts[j], obj.pts[j + 1], color1, 2, 8);
                }
                if (j < 8 && j > 4)
                {
                    cv::line(rgb, obj.pts[j], obj.pts[j + 1], color2, 2, 8);
                }
                if (j < 12 && j > 8)
                {
                    cv::line(rgb, obj.pts[j], obj.pts[j + 1], color3, 2, 8);
                }
                if (j < 16 && j > 12)
                {
                    cv::line(rgb, obj.pts[j], obj.pts[j + 1], color4, 2, 8);
                }
                if (j < 20 && j > 16)
                {
                    cv::line(rgb, obj.pts[j], obj.pts[j + 1], color5, 2, 8);
                }
            }
            cv::line(rgb, obj.pts[0], obj.pts[5], color2, 2, 8);
            cv::line(rgb, obj.pts[0], obj.pts[9], color3, 2, 8);
            cv::line(rgb, obj.pts[0], obj.pts[13], color4, 2, 8);
            cv::line(rgb, obj.pts[0], obj.pts[17], color5, 2, 8);
        }
    }
    return true;
}

bool handpose::preprocess(
    cv::Mat& image,
    cv::Rect& rec,
    ncnn::Mat& in,
    float& halfWpad,
    float& halfHpad,
    float& scale
)
{
//====================================
    cv::Point2i br = rec.br();
    if(br.x>=image.cols || br.y>=image.rows){return false;}
    if(rec.width<10 || rec.height<10){return false;}
    if(rec.x<0 || rec.y<0){return false;}
//====================================

    cv::Mat input = image(rec);
    float w = (float)input.cols;
    float h = (float)input.rows;

    if (w > h)
    {
        scale = this->input_size / w;
        w = this->input_size;
        h = h * scale;
    }
    else
    {
        scale = this->input_size / h;
        h = this->input_size;
        w = w * scale;
    }
    cv::resize(input, input, cv::Size(w,h));
    cv::Mat inputModel;
    halfWpad = abs((float)this->input_size - w) * 0.5;
    halfHpad = abs((float)this->input_size - h) * 0.5;
    cv::copyMakeBorder(input, inputModel, floor(halfHpad), ceil(halfHpad), floor(halfWpad), ceil(halfWpad), cv::BORDER_CONSTANT,cv::Scalar::all(0));
    in = ncnn::Mat::from_pixels(inputModel.data, ncnn::Mat::PIXEL_BGR, inputModel.cols, inputModel.rows);
    //ncnn::Mat in_src = ncnn::Mat::from_pixels_resize(input.data, ncnn::Mat::PIXEL_RGB, input.cols, input.rows, w, h);

    //ncnn::Mat in_pad;
    //ncnn::copy_make_border(in_src, in, hpad / 2, hpad - hpad / 2, wpad / 2, wpad - wpad / 2, ncnn::BORDER_CONSTANT, 0.f);
    in.substract_mean_normalize(0, this->norm_vals);

    //cv::Mat debugView = image(rec);
    //int w = debugView.cols;
    //int h = debugView.rows;
    //if (w > h)
    //{
    //    scale = this->input_size / w;
    //    w = this->input_size;
    //    h = h * scale;
    //}
    //else
    //{
    //    scale = this->input_size / h;
    //    h = this->input_size;
    //    w = w * scale;
    //}
    //cv::resize(debugView, debugView, cv::Size(w, h));
    //cv::Mat inputModel;
    //int halfWidth = abs(debugView.cols - this->input_size) / 2;
    //int halfHeight = abs(debugView.rows - this->input_size) / 2;
    //cv::copyMakeBorder(debugView, inputModel, halfHeight, halfHeight, halfWidth, halfWidth, cv::BORDER_CONSTANT);
    //input = ncnn::Mat::from_pixels(inputModel.data, ncnn::Mat::PIXEL_BGR, inputModel.cols, inputModel.rows);
    //input.substract_mean_normalize(0, this->norm_vals);
    //bool contiuns = inputModel.isContinuous();
    //if (contiuns)
    //{
    //    printf("============内存连续\n");
    //}
    //
    //=======================================================
}
