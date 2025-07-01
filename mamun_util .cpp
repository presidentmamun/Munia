#include "pch.h"
#include "mamun_util.h"
#include <k4a/k4a.h>
#include <k4a/k4a.hpp>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <sstream>
#include <string>


#include <iostream>
#include <chrono>
#include <ctime>


namespace mamun_util
{

	static inline cv::Mat kinectMat2cvMat(const k4a::image &im) {
	
		cv::Mat cv_image_with_alpha(im.get_height_pixels(), im.get_width_pixels(), CV_8UC4, (void *)im.get_buffer());
		cv::Mat cv_image_no_alpha;
		cv::cvtColor(cv_image_with_alpha, cv_image_no_alpha, cv::COLOR_BGRA2BGR);
		return cv_image_no_alpha;
	}


	static inline cv::Mat kinectDepth2cvDepth(const k4a::image &im)
	{
		return cv::Mat(im.get_height_pixels(),
			im.get_width_pixels(),
			CV_16U,
			(void *)im.get_buffer(),
			static_cast<size_t>(im.get_stride_bytes()));
	}
	static inline cv::Mat kinectPC2cvPC(const k4a::image &im)
	{
		return cv::Mat(im.get_height_pixels(),
			im.get_width_pixels(),
			CV_8UC4,
			(void *)im.get_buffer(),
			static_cast<size_t>(im.get_stride_bytes()));
	}

	cv::Mat get_mat(k4a::image& src, bool deep_copy)
	{
		assert(src.get_size() != 0);

		cv::Mat mat;
		const int32_t width = src.get_width_pixels();
		const int32_t height = src.get_height_pixels();

		const k4a_image_format_t format = src.get_format();
		switch (format)
		{
		case k4a_image_format_t::K4A_IMAGE_FORMAT_COLOR_MJPG:
		{
			// NOTE: this is slower than other formats.
			std::vector<uint8_t> buffer(src.get_buffer(), src.get_buffer() + src.get_size());
			mat = cv::imdecode(buffer, cv::IMREAD_ANYCOLOR);
			cv::cvtColor(mat, mat, cv::COLOR_BGR2BGRA);
			break;
		}
		case k4a_image_format_t::K4A_IMAGE_FORMAT_COLOR_NV12:
		{
			cv::Mat nv12 = cv::Mat(height + height / 2, width, CV_8UC1, src.get_buffer()).clone();
			cv::cvtColor(nv12, mat, cv::COLOR_YUV2BGRA_NV12);
			break;
		}
		case k4a_image_format_t::K4A_IMAGE_FORMAT_COLOR_YUY2:
		{
			cv::Mat yuy2 = cv::Mat(height, width, CV_8UC2, src.get_buffer()).clone();
			cv::cvtColor(yuy2, mat, cv::COLOR_YUV2BGRA_YUY2);
			break;
		}
		case k4a_image_format_t::K4A_IMAGE_FORMAT_COLOR_BGRA32:
		{
			mat = deep_copy ? cv::Mat(height, width, CV_8UC4, src.get_buffer()).clone()
				: cv::Mat(height, width, CV_8UC4, src.get_buffer());
			break;
		}
		case k4a_image_format_t::K4A_IMAGE_FORMAT_DEPTH16:
		case k4a_image_format_t::K4A_IMAGE_FORMAT_IR16:
		{
			mat = deep_copy ? cv::Mat(height, width, CV_16UC1, reinterpret_cast<uint16_t*>(src.get_buffer())).clone()
				: cv::Mat(height, width, CV_16UC1, reinterpret_cast<uint16_t*>(src.get_buffer()));
			break;
		}
		case k4a_image_format_t::K4A_IMAGE_FORMAT_CUSTOM8:
		{
			mat = cv::Mat(height, width, CV_8UC1, src.get_buffer()).clone();
			break;
		}
		case k4a_image_format_t::K4A_IMAGE_FORMAT_CUSTOM:
		{
			// NOTE: This is opencv_viz module format (cv::viz::WCloud).
			const int16_t* buffer = reinterpret_cast<int16_t*>(src.get_buffer());
			mat = cv::Mat(height, width, CV_32FC3, cv::Vec3f::all(std::numeric_limits<float>::quiet_NaN()));
			mat.forEach<cv::Vec3f>(
				[&](cv::Vec3f& point, const int32_t* position) {
				const int32_t index = (position[0] * width + position[1]) * 3;
				point = cv::Vec3f(buffer[index + 0], buffer[index + 1], buffer[index + 2]);
			}
			);
			break;
		}
		default:
			throw k4a::error("Failed to convert this format!");
			break;
		}

		return mat;
	}

	cv::Mat k4a_get_mat(k4a_image_t& src, bool deep_copy)
	{
		k4a_image_reference(src);
		k4a::image img = k4a::image(src);
		return get_mat(img, deep_copy);
	}
	bool im_show(cv::Mat src, cv::Mat &dst, std::string im_name, bool color_change, int color, int waiting_time, bool isImshow)
	{
	if (color_change) {
		double min;
		double max;
		cv::minMaxIdx(src, &min, &max);
		cv::Mat adjMap;
		src.convertTo(adjMap, CV_8U, 255 / (max - min), -min * 255 / (max - min));
		//src.convertTo(adjMap, CV_8U, -255.0 / 5000.0, 255.0);

		cv::Mat falseColorsMap;
		applyColorMap(adjMap, falseColorsMap, color);
		falseColorsMap.copyTo(dst);
	}
	
	if (isImshow) { imshow(im_name, dst); cv::waitKey(waiting_time); }

#ifdef HAVE_OPENCV_VIZ
	// Create Point Cloud Widget
	cv::viz::WCloud cloud = cv::viz::WCloud(xyz, color);

	// Show Widget
	viewer.showWidget("cloud", cloud);
	viewer.spinOnce();
#endif

	return true;
	}


	bool getRoiMat(cv::Mat &src, cv::Rect roi2, int percentage, cv::Mat maskMat) {
		int rows = src.rows;
		int cols = src.cols;

		int row_per = rows * percentage / 100;
		int col_per = cols * percentage / 100;

		cv::Rect roi = { col_per, row_per, cols - col_per, rows - row_per };

		cv::Rect roi1(0, 0, maskMat.cols, maskMat.rows);
		cv::Mat roiMat;
		


		if (!maskMat.empty())
		{
			roiMat = src(roi1);
			roiMat.copyTo(src);

			return true;
		}
		if (!roi2.empty()) {
			roiMat = src(roi2);
			roiMat.copyTo(src);

			return true;
		}

		roiMat = src(roi);


		roiMat.copyTo(src);

		return true;
	}

	bool im_binary(cv::Mat inputIm, cv::Mat &img8uc1) {
		int rows = inputIm.rows;
		int cols = inputIm.cols;
		int channels = inputIm.channels();
		cv::Mat outputIm = cv::Mat::zeros(inputIm.rows, inputIm.cols, CV_8UC1);


		for (int x = 0; x < cols; x++) {
			for (int y = 0; y < rows; y++) {


					if (channels == 1) {
						if (inputIm.type() == CV_16UC1) {
							ushort a = inputIm.at<ushort>(y, x);
							if (a != 0)
							{
								outputIm.at<uchar>(y, x) = 1;
							}
						}
						if (inputIm.type() == CV_8UC1) {
							int a = inputIm.at<uchar>(y, x);
							if (a != 0)
							{
								outputIm.at<uchar>(y, x) = 1;
							}
						}
						if (inputIm.type() == CV_32FC1) {
							float a = inputIm.at<float>(y, x);
							if (a != 0)
							{
								outputIm.at<uchar>(y, x) = 1;
							}
						}
					}
					if(channels==3){
						cv::Vec3f a = inputIm.at<cv::Vec3f>(y, x);
						
						if (a.val[2] != 0)
						{
							outputIm.at<uchar>(y, x) = 1;
						}
					}
					
					
				

			}
		}
		outputIm.copyTo(img8uc1);
		return true;
	}
	

	bool im_mask(cv::Mat src, cv::Mat &dst,  cv::Point p, bool imshow) {
		
		
		cv::Mat mask = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
		cv::Mat colorIm;
		im_show(src, colorIm, "a", 1, 10, -1, 0);
		cv::Mat garayIm;
		cv::cvtColor(colorIm, garayIm, cv::COLOR_BGR2GRAY);
		int threshold_value = colorIm.at<uchar>(p);
		cv::threshold(garayIm, mask, threshold_value /*134*/, 255, cv::THRESH_BINARY);
		
		cv::Mat binaryIm;
		src.copyTo(dst, mask);
		if (imshow)
		{
			im_binary(dst, binaryIm);
			cv::imshow("maskIM", binaryIm);
			cv::waitKey(0);
		}
		return true;
	}

	double im_mean(cv::Mat inputIm, cv::Mat inputMask) {
		int rows = inputMask.rows;
		int cols = inputMask.cols;
		int channels = inputIm.channels();


		int n = 0;
		double sum = 0;
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {

					if (channels > 1) {
						int a=inputMask.at<uchar>(y, x);
						if (a!=0)
						{
							cv::Vec3f a = inputIm.at<cv::Vec3f>(y, x);
							//std::cout<<a<<std::endl;
							sum = sum + a[2];
							n++;
						}
					}
					else
					{
						if (inputMask.at<uchar>(y, x))
						{
							float a = inputIm.at<float>(y, x);
							//std::cout<<a<<std::endl;
							sum = sum + a;
							n++;
						}
					}
				

			}
		}
		double average;
		if (n != 0)
			average = sum / n;
		else
			average = 0;

		return average;
	}

	
	bool pc_to_depth_Im(cv::Mat img32fc3orc4, cv::Mat &imag32fc1)
	{
		if (img32fc3orc4.type() == CV_32FC1)
			return true;
		cv::Mat dst = cv::Mat::zeros(img32fc3orc4.size(), CV_32FC1);

		/*if (img32fc3orc4.type() == CV_32FC3) {
			for (int dy = 0; dy < img32fc3orc4.cols; dy++)
			{
				for (int dx = 0; dx < img32fc3orc4.rows; dx++)
				{
					if (dy >= 0 && dy < img32fc3orc4.size().height && dx >= 0 && dx < img32fc3orc4.size().width) {
						cv::Vec3f v = img32fc3orc4.at<cv::Vec3f>(dy, dx);

						dst.at<float>(dy, dx) = v.val[2];

					}
				}
			}
		}
		if (img32fc3orc4.type() == CV_32FC4) {
			for (int dy = 0; dy < img32fc3orc4.cols; dy++)
			{
				for (int dx = 0; dx < img32fc3orc4.rows; dx++)
				{
					if (dy >= 0 && dy < img32fc3orc4.size().height && dx >= 0 && dx < img32fc3orc4.size().width) {
						cv::Vec4f v = img32fc3orc4.at<cv::Vec4f>(dy, dx);

						dst.at<float>(dy, dx) = v.val[2];

					}
				}
			}
		}*/

		std::vector<cv::Mat> bgr;
		cv::split(img32fc3orc4, bgr);
		bgr[2].copyTo(imag32fc1);
		//cv::cvtColor(inputIm, inputIm, cv::COLOR_BGR2GRAY);

		//im_show(dst, "average_depth_image", 1, 10, 0, 1);

		return true;
	}
	
	


	bool csv2vector(std::string fileName, std::vector<std::vector<double>> &data) {
		std::ifstream file(fileName);
		std::string line;
		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::vector<double> row;

			std::string cell;
			while (std::getline(ss, cell, ',')) {
				row.push_back(std::stod(cell));
			}

			data.push_back(row);
		}

		//cv::Mat mat(data, true); // true means copy the data
		//mat.convertTo(mat, CV_32F); // convert to float type
		//mat.copyTo(outputMat);
		return true;
	}

	bool csv2dPoints2pointsVector(std::string fileName, std::vector<cv::Point2f> &points) {

		std::ifstream fin(fileName); // open the csv file 
		if (fin.is_open()) // check if the file is opened successfully
		{
			std::string line; // create a string variable to store each line
			while (std::getline(fin, line)) // loop over all the lines in the file
			{
				std::stringstream ss(line); // create a stringstream object from the line
				float x, y; // create three float variables to store the point coordinates
				char comma; // create a char variable to store the comma delimiter
				ss >> x >> comma >> y ; // read the point coordinates and comma from the stringstream
				points.push_back(cv::Point2f(x, y)); // create a cv point from x, y and z and push it to the vector
			}
			fin.close(); // close the file
		}
		else // if the file is not opened successfully
		{
			std::cout << "Error: cannot open the file." << std::endl; // print an error message
			return false; // exit with a negative value
		}

		std::cout << std::endl; // print a newline
		return true;
	}


	bool pointsCsv2pointsVector(std::string fileName, std::vector<cv::Point3d> &points){
		
		std::ifstream fin(fileName); // open the csv file 
		if (fin.is_open()) // check if the file is opened successfully
		{
			std::string line; // create a string variable to store each line
			while (std::getline(fin, line)) // loop over all the lines in the file
			{
				std::stringstream ss(line); // create a stringstream object from the line
				double x, y, z; // create three float variables to store the point coordinates
				char comma; // create a char variable to store the comma delimiter
				ss >> x >> comma >> y >> comma >> z; // read the point coordinates and comma from the stringstream
				points.push_back(cv::Point3d(x, y, z)); // create a cv point from x, y and z and push it to the vector
			}
			fin.close(); // close the file
		}
		else // if the file is not opened successfully
		{
			std::cout << "Error: cannot open the file." << std::endl; // print an error message
			return false; // exit with a negative value
		}
		
		std::cout << std::endl; // print a newline
		return true;
	}

	bool is_convertible_to_double(const std::string& str) {
		char* end = nullptr;
		double val = strtod(str.c_str(), &end);
		return end != str.c_str() && *end == '\0';
	}

	bool csv2pointsVector(std::string fileName, std::vector<cv::Point3d>&pointvec, int c1, int c2, int c3)
	{
		

		std::ifstream fin(fileName); // open the csv file 
		if (fin.is_open()) // check if the file is opened successfully
		{
			std::string line;// create a string variable to store each line


			while (std::getline(fin, line)) // loop over all the lines in the file
			{
				std::vector<std::string>temp;
				std::string word;
				std::stringstream ss(line); // create a stringstream object from the line

				
				while(std::getline(ss, word, ',')) {
					
					temp.push_back(word);
				}

				if (temp.size() >= 3) {
					double x, y, z;

					bool a = is_convertible_to_double(temp[c1]);
					bool b = is_convertible_to_double(temp[c2]);
					bool c = is_convertible_to_double(temp[c3]);

					if (a)
						x = stod(temp[c1]);
					else
						x = 0;

					if (b)
						y = stod(temp[c2]);
					else
						y = 0;

					if (c)
						z = stod(temp[c3]);
					else
						z = 0;

					pointvec.push_back(cv::Point3d(x, y, z));
				}
				else if (temp.size() == 2) {

					double x, y;
					
					bool a = is_convertible_to_double(temp[c1]);
					bool b = is_convertible_to_double(temp[c2]);

					if (a)
						x = stod(temp[c1]);
					else
						x = 0;

					if (b)
						y = stod(temp[c2]);
					else
						y = 0;


					pointvec.push_back(cv::Point3d(x, y, 0));
				}
				else {
					double x;

					bool a = is_convertible_to_double(temp[c1]);

					if (a)
						x = stod(temp[c1]);
					else
						x = 0;

					pointvec.push_back(cv::Point3d(x, 0, 0));
				}

			}
			fin.close(); // close the file
		}
		else // if the file is not opened successfully
		{
			std::cout << "Error: cannot open the file." << std::endl; // print an error message
			return false; // exit with a negative value
		}

		std::cout << std::endl; // print a newline
		return true;
	}


	bool perspectiveTransformIM(cv::Mat srcImage, cv::Mat &outputIm) {
		
		std::vector<cv::Point2f> srcPoints;
		std::vector<cv::Point2f> dstPoints;

		srcPoints.push_back(cv::Point2f(0, 0));
		srcPoints.push_back(cv::Point2f(300, 0));
		srcPoints.push_back(cv::Point2f(300, 300));
		srcPoints.push_back(cv::Point2f(0, 300));
		srcPoints.push_back(cv::Point2f(0, 301));

		dstPoints.push_back(cv::Point2f(100, 100));
		dstPoints.push_back(cv::Point2f(200, 50));
		dstPoints.push_back(cv::Point2f(200, 250));
		dstPoints.push_back(cv::Point2f(100, 200));
		dstPoints.push_back(cv::Point2f(100, 201));

		cv::Mat dstImage = cv::Mat::zeros(srcImage.size(), srcImage.type());
		cv::Mat H;
		if (srcPoints.size() == 4) {
			H = cv::findHomography(srcPoints, dstPoints);
		}
		if (srcPoints.size() > 4) {
			H = cv::findHomography(srcPoints, dstPoints, cv::RANSAC);
		}

		cv::warpPerspective(srcImage, dstImage, H, dstImage.size());

		//cv::imwrite("destination.jpg", dstImage);

		dstImage.copyTo(outputIm);

		return true;
	}

	bool median_Im(cv::Mat &depthmap, bool needFun, int karnelSize) {
		if (!needFun)
			return true;
		
		cv::Mat temp;
		depthmap.convertTo(temp, CV_8U); 
		cv::medianBlur(temp, temp, karnelSize); 
		temp.convertTo(depthmap, CV_32F); 

		return true;
	}

	bool bilateral_filter(cv::Mat &depthmap, bool needFun, int diameter, double sigmaColor, double sigmaSpace, int borderType)
	{
		if (!needFun)
			return true;

		cv::Mat temp, dst;
		depthmap.convertTo(temp, CV_8U);

	bilateralFilter(temp, dst, diameter, sigmaColor, sigmaSpace, borderType);
	dst.convertTo(depthmap, CV_32F); 

	return true;
	}

	cv::Mat slMat2cvMat(sl::Mat& input) {
		// Mapping between MAT_TYPE and CV_TYPE
		int cv_type = -1;
		switch (input.getDataType()) {
		case sl::MAT_TYPE::F32_C1: cv_type = CV_32FC1; break;
		case sl::MAT_TYPE::F32_C2: cv_type = CV_32FC2; break;
		case sl::MAT_TYPE::F32_C3: cv_type = CV_32FC3; break;
		case sl::MAT_TYPE::F32_C4: cv_type = CV_32FC4; break;
		case sl::MAT_TYPE::U8_C1: cv_type = CV_8UC1; break;
		case sl::MAT_TYPE::U8_C2: cv_type = CV_8UC2; break;
		case sl::MAT_TYPE::U8_C3: cv_type = CV_8UC3; break;
		case sl::MAT_TYPE::U8_C4: cv_type = CV_8UC4; break;
		default: break;
		}

		return cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(sl::MEM::CPU));
	}

	double getZValue(const cv::Point3d& point, const std::vector<cv::Point3d>& points_forR) {
		int low = 0;
		int high = points_forR.size();
		while (low <= high) {
			int mid = (low + high) / 2;
			const auto& point_forR = points_forR[mid];
			if (point_forR.x < point.x || (point_forR.x == point.x && point_forR.y < point.y)) {
				low = mid + 1;
			}
			else if (point_forR.x > point.x || (point_forR.x == point.x && point_forR.y > point.y)) {
				high = mid - 1;
			}
			else {
				return point_forR.z;
			}
		}
		return -1;
	}


	bool Points_vector(std::vector<cv::Point3d> &points, bool needFn) {
		if (!needFn)
			return true;

		int n = 0;
		std::cout << "Enter the number of points: ";
		std::cin >> n;
		std::cout << "Enter the points as x y z: ";
		for (int i = 0; i < n; i++)
		{
			double x, y, z;
			std::cin >> x >> y >> z;
			points.push_back(cv::Point3d(x, y, z));
		}
		return true;
	}

	void mouse_click_event(int event, int x, int y, int flags, void* userdata) {

		//std::vector<cv::Point3d>* v = static_cast<std::vector<cv::Point3d>*>(userdata);
		//if (event == cv::EVENT_LBUTTONDOWN) {
		//	v->push_back(cv::Point3d(x, y, 0));
		//}

		struct MouseData
		{
			cv::Mat* img;
			std::vector<cv::Point3d>* pts;
			cv::String win_Name;
			int n;
		};

		MouseData* data = (MouseData*)userdata;
		cv::Mat* img = data->img;
		cv::String win_Name = data->win_Name;
		 
		std::vector<cv::Point3d>* pts = data->pts;
		if (event == cv::EVENT_LBUTTONDOWN)
		{
			data->n++; 
			std::string strN = std::to_string(data->n);
			cv::circle(*img, cv::Point(x, y), 3, cv::Scalar(255, 0, 0), 1);
			cv::putText(*img, strN, cv::Point(x, y), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 200, 0), 1);
			pts->push_back(cv::Point3d(x, y,0));
			cv::imshow(win_Name, *img);
		}
	}
	
	void mouse_click_event_2f(int event, int x, int y, int flags, void* userdata) {

		//std::vector<cv::Point3d>* v = static_cast<std::vector<cv::Point3d>*>(userdata);
		//if (event == cv::EVENT_LBUTTONDOWN) {
		//	v->push_back(cv::Point3d(x, y, 0));
		//}

		struct MouseData
		{
			cv::Mat* img;
			std::vector<cv::Point2f>* pts;
			cv::String win_Name;
			int n;
			int nPoints;
		};

		MouseData* data = (MouseData*)userdata;
		cv::Mat* img = data->img;
		cv::String win_Name = data->win_Name;

		std::vector<cv::Point2f>* pts = data->pts;
		if (event == cv::EVENT_LBUTTONDOWN)
		{
			data->n++;
			std::string strN = std::to_string(data->n);
			cv::circle(*img, cv::Point(x, y), 2, cv::Scalar(255, 255, 255), 1);
			cv::putText(*img, strN, cv::Point(x, y), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 200, 0), 1);
			pts->push_back(cv::Point2f(x, y));
			cv::imshow(win_Name, *img);

			if (data->n == data->nPoints)
				cv::destroyAllWindows();

		}
	}


	bool ends_with(const std::string& fullString, const std::string& ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		}
		else {
			return false;
		}
	}

	std::string get_current_date_and_time(int chooseDateORtime, float hour) {
		std::time_t now = std::time(nullptr); // get current time as seconds since epoch
		std::tm local_time; // create a tm structure to hold the local time
		localtime_s(&local_time, &now); // convert to local time and store in local_time
		char buffer[80]; // create a buffer to hold the formatted date and time
		

		std::time_t userwanted_time = now+hour*60*60; // get current time as seconds since epoch
		std::tm userWanted_local_time; // create a tm structure to hold the local time
		localtime_s(&userWanted_local_time, &userwanted_time); // convert to local time and store in local_time




		switch (chooseDateORtime) {
		case 1:
			std::strftime(buffer, 80, "%Y-%m-%d", &userWanted_local_time); // format the date
			break;
		case 2:
			std::strftime(buffer, 80, "%H:%M:%S", &local_time); // format the time
			break;
		case 3:
			std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &local_time); // format the date and time
			break;
		default:
			std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &local_time); // format the date and time
			break;
		}
		return std::string(buffer); // return as a string
	}

	bool panitIm_4CH(const cv::Mat & inputOutputIm, double radius)
	{
	std::vector<cv::Mat> channels;
	cv::split(inputOutputIm, channels);
	cv::Mat binary, binaryU8;
	threshold(channels[2], binary, 1, 255, cv::THRESH_BINARY);
	binary.convertTo(binaryU8, CV_8U);
	cv::floodFill(binaryU8, cv::Point(0, 0), 255);
	cv::Mat mask = 255 - binaryU8;


	

	cv::Mat paintIm;

	cv::inpaint(channels[2], mask, paintIm, radius, cv::INPAINT_TELEA);
	paintIm.copyTo(channels[2]);



	cv::merge(channels, inputOutputIm); 
	return true;
	}

	bool panitIm_1CH(const cv::Mat & inputOutputIm, double radius)
	{
		cv::Mat binary, binaryU8;
		threshold(inputOutputIm, binary, 1, 255, cv::THRESH_BINARY);
		binary.convertTo(binaryU8, CV_8U);
		cv::floodFill(binaryU8, cv::Point(0, 0), 255);
		cv::Mat mask = 255 - binaryU8;

		cv::Mat paintIm;

		cv::inpaint(inputOutputIm, mask, paintIm, radius, cv::INPAINT_TELEA);
		paintIm.copyTo(inputOutputIm);

		return true;
	}

	bool twoPointVectorInterchange(std::vector<cv::Point3d>&v1, std::vector<cv::Point3d>&v2)
	{
		std::vector<cv::Point3d>v3;

		for (int i = 0; i < v1.size(); i++)
		{
			cv::Point3d p = cv::Point3d(v1[i].x, v1[i].y, v1[i].z);
			v3.push_back(p);
		}
		v1.clear();

		for (int i = 0; i < v2.size(); i++)
		{
			cv::Point3d p = cv::Point3d(v2[i].x, v2[i].y, v2[i].z);
			v1.push_back(p);
		}
		v2.clear();

		for (int i = 0; i < v3.size(); i++)
		{
			cv::Point3d p = cv::Point3d(v3[i].x, v3[i].y, v3[i].z);
			v2.push_back(p);
		}
		v3.clear();

	
		return 1;
	}
};