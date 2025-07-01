#pragma once

#include <k4a/k4a.h>
#include <k4a/k4a.hpp>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <sl/Camera.hpp>
#include <sstream>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
//#include"matplot.h"


namespace mamun_util
{

	static inline cv::Mat kinectMat2cvMat(const k4a::image &im);
	static inline cv::Mat kinectDepth2cvDepth(const k4a::image &im);
	static inline cv::Mat kinectPC2cvPC(const k4a::image &im);

	cv::Mat get_mat(k4a::image& src, bool deep_copy = true);
	cv::Mat k4a_get_mat(k4a_image_t& src, bool deep_copy = true);
	bool im_show(cv::Mat src, cv::Mat &dst, std::string im_name = "ANONYMOUS", bool color_change = true, int color = 10, int waiting_time = 1, bool isImshow = true);
	bool getRoiMat(cv::Mat &src, cv::Rect roi2 = cv::Rect{}, int percentage = 0, cv::Mat maskMat = cv::Mat());	
	bool im_binary(cv::Mat inputIm, cv::Mat &img8uc1);
	bool im_mask(cv::Mat src, cv::Mat &dst, cv::Point p = cv::Point{ 0,0 }, bool imshow = false);
	double im_mean(cv::Mat inputIm, cv::Mat inputMask);
	bool pc_to_depth_Im(cv::Mat img32fc3orc4, cv::Mat &imag32fc1);
	bool csv2vector(std::string fileName, std::vector<std::vector<double>> &data);
	bool pointsCsv2pointsVector(std::string fileName, std::vector<cv::Point3d> &points);
	bool csv2dPoints2pointsVector(std::string fileName, std::vector<cv::Point2f> &points);
	bool csv2pointsVector(std::string fileName, std::vector<cv::Point3d>&pointvec, int c1=0, int c2=1, int c3=2);
	bool is_convertible_to_double(const std::string& str);
	bool perspectiveTransformIM(cv::Mat srcImage, cv::Mat &outputIm);
	bool median_Im(cv::Mat &depthmap, bool needFun = true, int karnelSize = 3);
	bool bilateral_filter(cv::Mat &depthmap, bool needFun = true, int diameter = 5, double sigmaColor = 80, double sigmaSpace = 80, int borderType = 4);
	cv::Mat slMat2cvMat(sl::Mat& input);
	double getZValue(const cv::Point3d& point, const std::vector<cv::Point3d>& points_forR);
	bool Points_vector(std::vector<cv::Point3d> &points, bool needFn=true);
	void mouse_click_event(int event, int x, int y, int flags, void* userdata);
	void mouse_click_event_2f(int event, int x, int y, int flags, void* userdata);
	bool ends_with(const std::string& fullString, const std::string& ending);
	bool panitIm_4CH(const cv::Mat & inputOutputIm, double radius=3.0);
	bool panitIm_1CH(const cv::Mat & inputOutputIm, double radius=3.0);
	std::string get_current_date_and_time(int chooseDateORtime=3, float hour=0);
	bool twoPointVectorInterchange(std::vector<cv::Point3d>&v1, std::vector<cv::Point3d>&v2);


}



