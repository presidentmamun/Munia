/**************************************
 Khairuzzaman Mamun
 ***************************************/

#include"pch.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <Windows.h>
#include "munia_functions.h"
#include "mamun_util.h"
#include <chrono>
#include <iostream> 
#include <fstream>
#include <cmath>
#include <sl/Camera.hpp>
#include <omp.h>
#include <algorithm>
#include <vector>
#include "Munia_mfcDlg.h"
#include <string> 
#include <filesystem>
#include <cstdlib>
#include <windows.h>
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>



#define CVPLOT_HEADER_ONLY

#include <CvPlot/cvplot.h>

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>


using namespace CvPlot;


using namespace std::chrono;
using namespace k4a;
using namespace std;
using namespace sl;
using namespace mamun_util;
namespace fs = std::filesystem;



munia_function_class::munia_function_class()
{


	bool is_pathOk= ends_with(workingFolder, "\\");
	if(workingFolder!="")
		if (!is_pathOk)
		{
		workingFolder = workingFolder + "\\";
		}

	transformation_matrix();


	std::string workingDir = workingFolder + "Munia_Data";
	const char* dir = workingDir.c_str();
	_mkdir(dir);

	std::string path = workingDir + "\\Pics";
	dir = path.c_str();
	_mkdir(dir);
	
	path = workingDir + "\\U";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\U\\YY8";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\U\\YED";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\SVOs";
	dir = path.c_str();
	_mkdir(dir);

	path = path + "\\Recorded_SVOs";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\Camera_Calibration_Files";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\U_Calibration_Files";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\U_Calibration_Files\\YED_LH";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\U_Calibration_Files\\YY8_LH";
	dir = path.c_str();
	_mkdir(dir);


	path = workingDir + "\\U_Calibration_Files\\YED_RH";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\U_Calibration_Files\\YY8_RH";
	dir = path.c_str();
	_mkdir(dir);


	path = workingDir + "\\Backup_Files";
	dir = path.c_str();
	_mkdir(dir);

	path = path + "\\forRdatas";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\R";
	dir = path.c_str();
	_mkdir(dir);

	path = workingDir + "\\3Dpoints";
	dir = path.c_str();
	_mkdir(dir);
	

	mamun_util::pointsCsv2pointsVector(workingFolder + "Munia_Data\\U\\forRdata.csv", points_forR);
}

munia_function_class::~munia_function_class()
{
	
	
}


bool munia_function_class::camera_init_parameters(int depthModeIndex, bool resolution2K) 
{


	switch (depthModeIndex)
	{
	case 1:
		init_parametersL.depth_mode = sl::DEPTH_MODE::ULTRA;
		init_parametersR.depth_mode = sl::DEPTH_MODE::ULTRA;
		break;
	case 2:
		init_parametersL.depth_mode = sl::DEPTH_MODE::QUALITY;
		init_parametersR.depth_mode = sl::DEPTH_MODE::QUALITY;
		break;
	case 3:
		init_parametersL.depth_mode = sl::DEPTH_MODE::PERFORMANCE;
		init_parametersR.depth_mode = sl::DEPTH_MODE::PERFORMANCE;
		break;
	case 4:
		init_parametersL.depth_mode = sl::DEPTH_MODE::LAST;
		init_parametersR.depth_mode = sl::DEPTH_MODE::LAST;
		break;
	}

	init_parametersL.coordinate_units = sl::UNIT::MILLIMETER;
	init_parametersR.coordinate_units = sl::UNIT::MILLIMETER;

	if (resolution2K) {
		init_parametersL.camera_resolution = sl::RESOLUTION::HD2K;
		init_parametersR.camera_resolution = sl::RESOLUTION::HD2K;
	}
	else {
		init_parametersL.camera_resolution = sl::RESOLUTION::HD1080;
		init_parametersR.camera_resolution = sl::RESOLUTION::HD1080;

	}

	init_parametersL.camera_fps = 15;
	init_parametersR.camera_fps = 15;

	init_parametersL.depth_minimum_distance = 1;
	init_parametersR.depth_minimum_distance = 1;

	init_parametersL.depth_maximum_distance = 1000;
	init_parametersR.depth_maximum_distance = 1000;

	init_parametersL.depth_stabilization = 100;
	init_parametersR.depth_stabilization = 100;

	//tracking_parameters.set_as_static = true;
	return true;
}

bool munia_function_class::camera_runtime_parameters(bool sensingMode_Fill) 
{

	if (sensingMode_Fill == true)
		runtime_parameters.sensing_mode = sl::SENSING_MODE::FILL;
	else
		runtime_parameters.sensing_mode = sl::SENSING_MODE::STANDARD;

	runtime_parameters.confidence_threshold = 100;
	runtime_parameters.texture_confidence_threshold = 100;
	return true;
}

bool munia_function_class::camera_setting(bool needfn, bool deFault, int custom_GainL, int custom_GainR, int custom_exposerL, int custom_exposerR) 
{
	if (!needfn)
		return true;

	if (!deFault) {
		zedL.setCameraSettings(VIDEO_SETTINGS::BRIGHTNESS, 4);
		zedR.setCameraSettings(VIDEO_SETTINGS::BRIGHTNESS, 4);

		zedL.setCameraSettings(VIDEO_SETTINGS::CONTRAST, 4);
		zedR.setCameraSettings(VIDEO_SETTINGS::CONTRAST, 4);

		zedL.setCameraSettings(VIDEO_SETTINGS::HUE, 0);
		zedR.setCameraSettings(VIDEO_SETTINGS::HUE, 0);

		zedL.setCameraSettings(VIDEO_SETTINGS::SATURATION, 4);
		zedR.setCameraSettings(VIDEO_SETTINGS::SATURATION, 4);

		zedL.setCameraSettings(VIDEO_SETTINGS::SHARPNESS, 3);
		zedR.setCameraSettings(VIDEO_SETTINGS::SHARPNESS, 3);

		zedL.setCameraSettings(VIDEO_SETTINGS::GAMMA, 5);
		zedR.setCameraSettings(VIDEO_SETTINGS::GAMMA, 5);

		zedL.setCameraSettings(VIDEO_SETTINGS::WHITEBALANCE_TEMPERATURE, 4600);
		zedR.setCameraSettings(VIDEO_SETTINGS::WHITEBALANCE_TEMPERATURE, 4600);

		zedL.setCameraSettings(VIDEO_SETTINGS::GAIN, custom_GainL);
		zedR.setCameraSettings(VIDEO_SETTINGS::GAIN, custom_GainR);

		zedL.setCameraSettings(VIDEO_SETTINGS::EXPOSURE, custom_exposerL);
		zedR.setCameraSettings(VIDEO_SETTINGS::EXPOSURE, custom_exposerR);
	}
	else {
		zedL.setCameraSettings(VIDEO_SETTINGS::BRIGHTNESS, 4);
		zedR.setCameraSettings(VIDEO_SETTINGS::BRIGHTNESS, 4);

		zedL.setCameraSettings(VIDEO_SETTINGS::CONTRAST, 4);
		zedR.setCameraSettings(VIDEO_SETTINGS::CONTRAST, 4);

		zedL.setCameraSettings(VIDEO_SETTINGS::HUE, 0);
		zedR.setCameraSettings(VIDEO_SETTINGS::HUE, 0);

		zedL.setCameraSettings(VIDEO_SETTINGS::SATURATION, 4);
		zedR.setCameraSettings(VIDEO_SETTINGS::SATURATION, 4);

		zedL.setCameraSettings(VIDEO_SETTINGS::SHARPNESS, 3);
		zedR.setCameraSettings(VIDEO_SETTINGS::SHARPNESS, 3);

		zedL.setCameraSettings(VIDEO_SETTINGS::GAMMA, 5);
		zedR.setCameraSettings(VIDEO_SETTINGS::GAMMA, 5);

		zedL.setCameraSettings(VIDEO_SETTINGS::WHITEBALANCE_AUTO);
		zedR.setCameraSettings(VIDEO_SETTINGS::WHITEBALANCE_AUTO);

		zedL.setCameraSettings(VIDEO_SETTINGS::GAIN);
		zedR.setCameraSettings(VIDEO_SETTINGS::GAIN);

		zedL.setCameraSettings(VIDEO_SETTINGS::EXPOSURE);
		zedR.setCameraSettings(VIDEO_SETTINGS::EXPOSURE);
	}
	return true;

}

bool munia_function_class::calcChessboardCorners(cv::Size boardSize, float squareSize, std::vector<cv::Point3f>& corners, Pattern patternType)
{
	corners.resize(0);
	switch (patternType)
	{
	case CHESSBOARD:
	case CIRCLES_GRID:
		for (int i = 0; i < boardSize.height; i++) 
		{
			for (int j = 0; j < boardSize.width; j++)
			{
			corners.push_back(cv::Point3f(float(j*squareSize), float(i*squareSize), 0));
			}
		}
		break;
	case ASYMMETRIC_CIRCLES_GRID:
		for (int i = 0; i < boardSize.height; i++) 
		{
		for (int j = 0; j < boardSize.width; j++) 
		{
		corners.push_back(cv::Point3f(float((2 * j + i % 2)*squareSize), float(i*squareSize), 0));
		}
		}
		break;
	default:
		return false;
	}

	return true;
}

bool munia_function_class::extract_cornarPoints(cv::Mat &image, std::vector<cv::Point2f> &points, int np) {

	struct MouseData
	{
		cv::Mat* img;
		std::vector<cv::Point2f>* pts;

		cv::String win_Name;
		int n;
		int nPoints;
	};
	cv::String win_name = "Depth_Image";
	if (!image.empty())
	{

		MouseData data;
		data.img = &image;
		data.pts = &points;
		data.win_Name = win_name;
		data.n = 0;
		data.nPoints = np;
		cv::namedWindow(win_name);

		cv::imshow(win_name, image);
		cv::setMouseCallback(win_name, mouse_click_event_2f, &data);
		cv::waitKey(0);
	}

	return true;
}


bool munia_function_class::autoCornerGet(cv::Mat &im, std::vector<cv::Point2f> &imagePoints, bool first, int x, int y)
{
		if (im.empty())
			return false;

	cv::Mat gray(im.size(), CV_8UC1); ;
	cv::cvtColor(im, gray, cv::COLOR_BGR2GRAY);
	int flags;
	flags = cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE;
	flags += cv::CALIB_CB_ASYMMETRIC_GRID;
	cv::Size boardSize(x, y); // The size of the checkerboard (number of inner corners per row and column)
	bool found = findChessboardCorners(gray, boardSize, imagePoints, flags);

	if (!first)
	{
		int size = imagePoints.size();

		vector<cv::Point2f> imagePoints1;

		for (int i = 0; i < size; i++)
		{
			int lf = size - i - 1;
			imagePoints1.push_back(imagePoints[lf]);
		}

		imagePoints.clear();

		for (int i = 0; i < imagePoints1.size(); i++)
		{
			imagePoints.push_back(imagePoints1[i]);
		}


	}


	if (found)
	{
		cornerSubPix(gray, imagePoints, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(3, 30, 0.1));

		drawChessboardCorners(im, boardSize, imagePoints, found);
		cout << "The coordinates of the checkerboard corners are: " << endl;
		for (int i = 0; i < imagePoints.size(); i++)
		{
			cout << imagePoints[i] << endl;
		}
	}
	else
	{
		return false;
	}


	return true;
}


bool munia_function_class::loadMatrix_XML(
	std::string file, cv::Mat& m1, cv::Mat& m2, cv::Mat& m3, cv::Mat& m4,
	std::string sm1, std::string sm2, std::string sm3, std::string sm4)
{
	if (file == "")
		return false;

	m1 = cv::Mat{}, m2 = cv::Mat{}, m3 = cv::Mat{}, m4 = cv::Mat{};

	cv::FileStorage cv_file(file, cv::FileStorage::READ);

	if (sm1 != "")
		cv_file[sm1] >> m1;
	if (sm2 != "")
		cv_file[sm2] >> m2;
	if (sm3 != "")
		cv_file[sm3] >> m3;
	if (sm4 != "")
		cv_file[sm4] >> m4;

	cv_file.release();

	return true;
}


bool munia_function_class::getHRT_matrix(cv::Mat &cam0, cv::Mat &cam1, bool fullHd, int r, int c, bool option)
{
	if (cam0.empty() || cam1.empty())
		return false;

	std::string str = workingFolder+"Munia_Data\\Camera_Calibration_Files\\";

	std::vector<cv::Point3f> objectPoints;
	std::vector<cv::Point2f> corners1, corners2;

	calcChessboardCorners(cv::Size(r, c), 30.0f, objectPoints);
	std::ofstream extract_file0;
	extract_file0.open(str + "objectPoints.csv");
	for (int i = 0; i < objectPoints.size(); i++) {
		extract_file0 << objectPoints[i].x << "," << objectPoints[i].y << "," << objectPoints[i].z << endl;
	}

	bool g1 = 0;
	if (option) 
	{
		corners1.clear();
		g1 = autoCornerGet(cam0, corners1, 1, r, c);
	}
	else
	{
		corners1.clear();
		g1=extract_cornarPoints(cam0, corners1, objectPoints.size());

	}
	if (g1) 
	{
		std::ofstream extract_file1;
		extract_file1.open(str + "corners1.csv");
		for (int i = 0; i < corners1.size(); i++) {
			extract_file1 << corners1[i].x << "," << corners1[i].y << endl;
		}
	}

	bool g2 = 0;
	if (option) 
	{
		corners2.clear();
		g2 = autoCornerGet(cam1, corners2, 0, r, c);
	}
	else
	{
		corners2.clear();
		g2=extract_cornarPoints(cam1, corners2, objectPoints.size());
	}
	if (g2) 
	{
		std::ofstream extract_file2;
		extract_file2.open(str + "corners2.csv");
		for (int i = 0; i < corners2.size(); i++) {
			extract_file2 << corners2[i].x << "," << corners2[i].y << endl;
		}
	}

	cv::Mat show;
	cv::hconcat(cam0, cam1, show);
	int w = cam0.size().width;
	int h = cam0.size().height;
	h = h / 2;

	cv::resize(show, show, cv::Size(w, h));
	cv::imshow("cornars", show);
	auto key = cv::waitKey(0);

	if (key == 'q')
	{
		cv::destroyAllWindows();
	}

	if (corners1.size() == 0 || corners2.size() == 0|| corners1.size()!= corners2.size())
		return 0;

	cv::Mat H = cv::findHomography(corners2, corners1);

	float x, y;
	if (fullHd)
	{
		x = 1920, y = 1080;
	}
	else
	{
		x = 2208, y = 1242;
	}

	cv::Size imSize = cv::Size(x, y);

	vector<vector<cv::Point3f>> vecObj;
	vector<vector<cv::Point2f>> vecCon1;
	vector<vector<cv::Point2f>> vecCon2;
	vecObj.push_back(objectPoints);
	vecCon1.push_back(corners1);
	vecCon2.push_back(corners2);

	std::string fileN;
	if (fullHd)
	{
		fileN = str+"cameraMat_HD.xml";
	}
	else
	{
		fileN = str + "cameraMat_2K.xml";
	}
	cv::Mat M1, D1, M2, D2;
	loadMatrix_XML(fileN, M1, M2,  D1, D2, "cam0M", "cam1M", "cam0D", "cam1D");
	cv::Mat R, T, E, F;

	int flags = cv::CALIB_FIX_INTRINSIC;
	cv::TermCriteria criteria = cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-6);
	double rms = cv::stereoCalibrate(vecObj, vecCon2, vecCon1, M2, D2, M1, D1, imSize, R, T, E, F);


	
	if (fullHd)
	{
		fileN = str + "HRT_HD.xml";
	}
	else
	{
		fileN = str + "HRT_2K.xml";
	}
	

	cv::FileStorage cv_file(fileN, cv::FileStorage::WRITE);
	cv_file<<"H" << H;
	cv_file<<"R" << R;
	cv_file<<"T" << T;



	return 1;
}



bool munia_function_class::transformation_matrix(bool resolution2K) 
{
	std::string str = workingFolder + "Munia_Data\\Camera_Calibration_Files\\";

	std::string name = "";


	if (resolution2K) 
	{
		name = "HRT_2K.xml";

		switch (itemIndex)
		{
		case 1:
			F_mirror_roi = { 580,121,1569 - 580, 1001 - 121 };//for YED 2k
			break;
		case 2:
			F_mirror_roi = { 580,0,1569 - 580, 1001+121 };//for YY8 2k need to change
			break;
		}


#if 0
		rotation_matrix = (cv::Mat_<float>(3, 3) <<
			-0.9999097085166843, -0.00360343784227719, -0.0129456575652362,
		0.003732435890932479, -0.9999434890384556, -0.009954278071258184,
		-0.01290905637138485, -0.010001698121654, 0.9998666522583318);
		homography_matrix = (cv::Mat_<float>(3, 3) <<
			-0.9938388860072157, -0.005152671252053625, 2134.989250388812,
		-9.549623858967895e-05, -0.9894100249509715, 1064.574408428045,
		-6.813152140361586e-06, -2.111427386595095e-06, 1);
		translation = (cv::Mat_<float>(3, 1) <<
			-5.77831723375949,
		-26.11597429649662,
		0.4471552153605886);
#endif
	}
	else 
	{
		name = "HRT_HD.xml";

		switch (itemIndex)
		{
		case 1:
			F_mirror_roi = { 410,20,1480 - 410, 870 - 20 };//for HD1080 YED
			break;
		case 2:
			F_mirror_roi = { 410,0,1480 - 410, 870+20+60 };//for HD1080 YY8 need to change
			break;
		}

#if 0
		rotation_matrix = (cv::Mat_<float>(3, 3) <<
			-0.9999294817399321, -0.003103768244815862, -0.01146290408199651,
		0.003211400296018385, -0.9999508204253377, -0.009383157188964576,
		-0.01143321719593014, -0.009419307478607822, 0.9998902730755879);
		homography_matrix = (cv::Mat_<float>(3, 3) <<
			-0.9943749551231622, -0.004372840538950434, 1850.773358359458,
		0.000361924340274906, -0.9905389775175124, 903.7475934199409,
		-6.046204621305657e-06, -1.814882820579449e-06, 1);
		translation = (cv::Mat_<float>(3, 1) <<
			-6.031159791076169,
		-26.03000468099885,
		0.4480261936197077);
#endif
	}

	cv::Mat faltuMat;
	loadMatrix_XML(str + name, homography_matrix, rotation_matrix, translation, faltuMat, "H", "R", "T");

	cv::hconcat(rotation_matrix, translation, R);
	return true;
}


bool munia_function_class::opencamera(bool default_cam_setting, int custom_GainL, int custom_GainR, int custom_exposerL, int custom_exposerR, int depthModeIndex, bool resolution2K, bool sensingMode_Fill) 
{ 
	/* this function will open the camera, and prepare for capturing...*/


	camera_init_parameters(depthModeIndex, resolution2K);


	vector<DeviceProperties> devList(Camera::getDeviceList());
	nb_detected_zed = devList.size();

	cout << nb_detected_zed << " ZED Detected" << endl;
	if (nb_detected_zed < 2)
	{
		cout << "Error  and exit the program." << endl;
		return false;
	}
	//  open first detected cameras
	init_parametersL.input.setFromCameraID(0);
	ERROR_CODE errL = zedL.open(init_parametersL);
	if (errL == ERROR_CODE::SUCCESS) {
		auto cam_info = zedL.getCameraInformation();
		cout << cam_info.camera_model << ", ID: " << 0 << ", SN: " << cam_info.serial_number << " Opened" << endl;
	}
	else {return false;}
	//  open 2nd detected cameras
	init_parametersR.input.setFromCameraID(1);
	ERROR_CODE errR = zedR.open(init_parametersR);
	if (errR == ERROR_CODE::SUCCESS) {
		auto cam_info = zedR.getCameraInformation();
		cout << cam_info.camera_model << ", ID: " << 1 << ", SN: " << cam_info.serial_number << " Opened" << endl;
	}
	else { return false; }

	camera_setting(1, default_cam_setting, custom_GainL, custom_GainR, custom_exposerL, custom_exposerR); // 0 is for custom settings, 1 is default settings
	auto resolution = zedL.getCameraInformation().camera_configuration.resolution;
	F_resolution = resolution;
	camera_runtime_parameters(sensingMode_Fill);

	return true;
}

bool munia_function_class::opening_recording_svo( bool default_cam_setting, int custom_GainL, int custom_GainR, int custom_exposerL, int custom_exposerR, bool resolution2K)
{

	if (resolution2K) {
		init_parametersL.camera_resolution = RESOLUTION::HD2K;
		init_parametersR.camera_resolution = RESOLUTION::HD2K;
	}
	else {
		init_parametersL.camera_resolution = RESOLUTION::HD1080;
		init_parametersR.camera_resolution = RESOLUTION::HD1080;
	}
	init_parametersL.camera_fps = 15;
	init_parametersR.camera_fps = 15;


	vector<DeviceProperties> devList(Camera::getDeviceList());
	nb_detected_zed = devList.size();

	cout << nb_detected_zed << " ZED Detected" << endl;
	if (nb_detected_zed < 2)
	{
		cout << "Error  and exit the program." << endl;
		return false;
	}
	//  open first detected cameras
	init_parametersL.input.setFromCameraID(0);
	ERROR_CODE errL_svo = zedL.open(init_parametersL);
	if (errL_svo != ERROR_CODE::SUCCESS) {
		return false;
	}
	//  open 2nd detected cameras
	init_parametersR.input.setFromCameraID(1);
	ERROR_CODE errR_svo = zedR.open(init_parametersR);
	if (errR_svo != ERROR_CODE::SUCCESS) {
		return false;
	}

	camera_setting(1, default_cam_setting, custom_GainL, custom_GainR, custom_exposerL, custom_exposerR); // 0 is for custom settings, 1 is default settings


//////////

	return true;
}

bool munia_function_class::recording_svo(const char* cam0svo, const char* cam1svo, int nImCap) 
{


	std::string path1 = cam0svo;
	std::string path2 = cam1svo;

	bool is_svo1 = ends_with(path1, ".svo");
	bool is_svo2 = ends_with(path2, ".svo");

	if (!is_svo1 || !is_svo2)
	{
		std::cout << "The path of the svo files are invalid" << endl;
		return false;
	}


	ERROR_CODE enaRecL = zedL.enableRecording(RecordingParameters(cam0svo, SVO_COMPRESSION_MODE::H264_LOSSLESS));
	if (enaRecL != ERROR_CODE::SUCCESS) {
		return false;
	}
	ERROR_CODE enaRecR = zedR.enableRecording(RecordingParameters(cam1svo, SVO_COMPRESSION_MODE::H264_LOSSLESS));
	if (enaRecL != ERROR_CODE::SUCCESS) {
		return false;
	}
	for (int imcap = 0; imcap < 2 * nImCap; imcap++) {
		zedL.grab();
		zedR.grab();
		DoEvent();
	}
	zedL.disableRecording();
	zedR.disableRecording();

	return true;
}


bool munia_function_class::closecamera() 
{
	/*for closing the camera.*/
	zedL.close(); zedR.close();
	return true;
}

bool munia_function_class::open_svo(const std::vector<std::string>& filenames, int depthModeIndex, bool resolution2K, bool sensingMode_Fill)
{

		camera_init_parameters(depthModeIndex, resolution2K);

		std::string path1 = filenames[0].c_str();
		std::string path2 = filenames[1].c_str();

		bool is_svo1 = ends_with(path1, ".svo");
		bool is_svo2 = ends_with(path2, ".svo");

		if (is_svo1 && is_svo2) 
		{
			init_parametersL.input.setFromSVOFile(filenames[0].c_str());
			init_parametersR.input.setFromSVOFile(filenames[1].c_str());
		}
		else 
		{ 
			std::cout << "The path of the svo files are invalid" << endl;
			return false; 
		}

		// Open the camera
		auto returned_state0 = zedL.open(init_parametersL);
		auto returned_state1 = zedR.open(init_parametersR);

		if (returned_state0 != sl::ERROR_CODE::SUCCESS)
		{
			std::cout << "Camera Open" << returned_state0 << "Exit program." << std::endl;
			return false;
		}
		if (returned_state1 != sl::ERROR_CODE::SUCCESS)
		{
			std::cout << "Camera Open" << returned_state1 << "Exit program." << std::endl;
			return false;
		}
	

	//std::cout << "frames : " << zedL.getSVONumberOfFrames() << ", " << zedR.getSVONumberOfFrames() << std::endl;

	auto resolution = zedL.getCameraInformation().camera_configuration.resolution;
	F_resolution = resolution;

	camera_runtime_parameters(sensingMode_Fill);

	return true;
}

bool munia_function_class::imageShow_inMFC(cv::Mat image, std::string winName, float multiplication)
{

	if (!image.empty())
	{
		float resize_w = multiplication;
		float resize_h = multiplication;

		int Width = image.size().width;
		int Height = image.size().height;

		cv::Mat resizedIm;
		cv::resize(image, resizedIm, cv::Size(Width * resize_w, Height * resize_h), 1);

		cv::namedWindow(winName);
		cv::imshow(winName, resizedIm);
		cv::waitKey(1);
	}
	else
		return false;

	return true;
}

bool munia_function_class::plc_initialization(std::string adr, std::string prt)
{
	
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) {
		}

		const char *adrs = adr.c_str();
		const char *port = prt.c_str();

		struct sockaddr_in dst_addr;
		memset(&dst_addr, 0, sizeof(dst_addr));
		dst_addr.sin_port = htons(atoi(port));
		dst_addr.sin_family = AF_INET;

		inet_pton(dst_addr.sin_family, adrs, &dst_addr.sin_addr.s_addr);

		dst_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(dst_socket, (struct sockaddr *) &dst_addr, sizeof(dst_addr))) {
			return false;
		}

		

	return true;
}

int munia_function_class::PLCCom(std::string cmd, std::string* rbuf)
{

	char recv_buf[256];

	int status;
	int recv1_result;
	char *c = NULL;

	cmd = cmd + "\r";
	const char* cmdb = cmd.c_str();

	send(dst_socket, cmdb, 256, 0);

	std::string str;
	std::string substr;
	recv1_result = recv(dst_socket, recv_buf, sizeof(char) * 256, 0);
	if (recv1_result == 0 || recv1_result == -1) {
		status = closesocket(dst_socket);
		*rbuf = "E";
		return(-1);
	}
	else {
		str = recv_buf;
		*rbuf = str.substr(0, recv1_result - 2);
		return(0);
	}

}


bool munia_function_class::plc_workingFn(bool autoCalculation)
{

	counter = counter + 1;
	bool right = 1;
	std::string cmd;
	std::string rbuf;

	//H.Takanohashi 2023.10.12
	if (autoCalculation) 
	{
		cmd = "WR MR1100 1"; //pc ready
		PLCCom(cmd, &rbuf);	//write flag (PC READY)
	}
	else 
	{
		cmd = "WR MR1100 0"; //pause plc "WR MR1100 0"
		PLCCom(cmd, &rbuf);	
	}

	cmd = "RD MR1007"; //shutDown
	PLCCom(cmd, &rbuf);	//read flag(Shutdown)
	if (rbuf == "0") {
		//
		cmd = "RD MR1001";
		PLCCom(cmd, &rbuf);	//read flag(Read data finished)
		if (rbuf == "0") {

			//H.Takanohashi 2023.10.12
			cmd = "WR MR1102 0";
			PLCCom(cmd, &rbuf);	//write flag (data write finished :OFF)
			//

			cmd = "RD MR1000";
			PLCCom(cmd, &rbuf);	//read flag(Calc. & Checking results )
			if (rbuf == "1") {


				//H.Takanohashi 2023.10.12
				cmd = "RD DM100";
				PLCCom(cmd, &rbuf);	//Read Model Flag

				switch (std::stoi(rbuf))
				{
				case 1:
					right = 0;
					itemIndex = 1;
					upperLimit = 1260;
					judge_by_ec_only = 0;
					transformation_matrix();
					points_forU.clear();
					mamun_util::pointsCsv2pointsVector(workingFolder + "Munia_Data\\U\\YED\\PointsforUR.csv", points_forU);
					break;
				case 2:
					right = 1;
					itemIndex = 1;
					upperLimit = 1260;
					judge_by_ec_only = 0;
					transformation_matrix();
					points_forU.clear();
					mamun_util::pointsCsv2pointsVector(workingFolder + "Munia_Data\\U\\YED\\PointsforUR.csv", points_forU);
					break;
				case 3:
					right = 0;
					itemIndex = 2;
					upperLimit = 1290;
					judge_by_ec_only = 1;
					transformation_matrix();
					points_forU.clear();
					mamun_util::pointsCsv2pointsVector(workingFolder + "Munia_Data\\U\\YY8\\PointsforUR.csv", points_forU);
					break;
				case 4:
					right = 1;
					itemIndex = 2;
					upperLimit = 1290;
					judge_by_ec_only = 1;
					transformation_matrix();
					points_forU.clear();
					mamun_util::pointsCsv2pointsVector(workingFolder + "Munia_Data\\U\\YY8\\PointsforUL.csv", points_forU);
					break;
				case 5:
					right = 0;
					itemIndex = 2;
					upperLimit = 1290;
					judge_by_ec_only = 0;
					transformation_matrix();
					points_forU.clear();
					mamun_util::pointsCsv2pointsVector(workingFolder + "Munia_Data\\U\\YY8\\PointsforUL.csv", points_forU);
					break;
				case 6:
					right = 1;
					itemIndex = 2;
					upperLimit = 1290;
					judge_by_ec_only = 0;
					transformation_matrix();
					points_forU.clear();
					mamun_util::pointsCsv2pointsVector(workingFolder + "Munia_Data\\U\\YY8\\PointsforUL.csv", points_forU);
					break;

				}
				
				//if (rbuf == "1") {
				//	right = 1;
				//	points_forU.clear();
				//	mamun_util::pointsCsv2pointsVector(workingFolder + "Munia_Data\\U\\YED\\PointsforUR.csv", points_forU);
				//}
				//else {
				//	right = 0;		//LH
				//	points_forU.clear();
				//	mamun_util::pointsCsv2pointsVector(workingFolder + "Munia_Data\\U\\YED\\PointsforUL.csv", points_forU);
				//}

				//mirror position check
				std::vector<cv::Point3d> tpoints;
				double d = checkingMirrorPosition(points_forU, tpoints, right, 1);//21-Agust-2024
				//if (d == -361 || d == -999)
				//{
				//	//cmd = "WR MR1011 1";
				//	cmd = "WR DM1000 4";

				//}
				//else {

					cmd = "WR MR1101 1";
					PLCCom(cmd, &rbuf);	//write flag(Now Capture :ON)


					//Capture
					extract_csvDepthPc(1, 2);

					cmd = "WR MR1101 0";
					PLCCom(cmd, &rbuf);	//write flag(Now Capture :OFF)

					//calc & Dicition
					std::string str = "AUTO";

					std::vector<double> vec;
					extract_Radius(inputDepthImage, vec, right, str, 1, itemIndex);
					std::vector<int>l_index, u_index;
					int decision_index = checkingResults(vec, resultPlot, l_index, u_index, right, str);

					//int decision_index = 1;
					std::string val;
					val = std::to_string(decision_index);

					cmd = "WR DM1000 " + val;
				/*}*/

				//std::string decisionIndex =
				PLCCom(cmd, &rbuf);	//write decision

				cmd = "WR MR1102 1";
				PLCCom(cmd, &rbuf);	//write flag (data write finished :ON)
			}
			else {
				if (counter>=8) {
					if (liveBeatflag == 1) {
						cmd = "WR MR1106 0";
						PLCCom(cmd, &rbuf);
						liveBeatflag =0;
					}
					else {
						cmd = "WR MR1106 1";
						PLCCom(cmd, &rbuf);
						liveBeatflag = 1;
					}
					counter = 0;
				}

			}
		}
		else {
			cmd = "WR MR1102 0";
			PLCCom(cmd, &rbuf);	//write flag(data write finished :OFF)
		}

	}
	else {	//Shuttdown
		cmd = "WR MR1100 0";
		PLCCom(cmd, &rbuf);	//write flag (PC Not READY  )
		closesocket(dst_socket);
		shutdownpc = 1;
	}

	return true;
}

void munia_function_class::ShutdownSystem()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tokenPriv;
	BOOL bRet;

	bRet = OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &(tokenPriv.Privileges[0].Luid));
	tokenPriv.PrivilegeCount = 1;
	tokenPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	bRet = AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, 0, NULL, NULL);

	ExitWindowsEx(EWX_SHUTDOWN, 0);

	
}

bool munia_function_class::take_2_BGRIm( bool fullHD)
{
	cv::Mat cv_bgrIm[2];

	sl::Mat sl_bgrIm[2];
	for (int i = 0; i < 2; i++) 
	{
		if (zedL.grab(runtime_parameters) == sl::ERROR_CODE::SUCCESS && zedR.grab(runtime_parameters) == sl::ERROR_CODE::SUCCESS)
		{

			zedL.retrieveImage(sl_bgrIm[0], sl::VIEW::LEFT, sl::MEM::CPU, F_resolution);
			zedR.retrieveImage(sl_bgrIm[1], sl::VIEW::LEFT, sl::MEM::CPU, F_resolution);

			cv_bgrIm[0] = slMat2cvMat(sl_bgrIm[0]);
			cv_bgrIm[1] = slMat2cvMat(sl_bgrIm[1]);
		}
		else
		{
			return false;
		}
	}
	std::string str0, str1;
	if (fullHD)
	{
		str0 = workingFolder + "Munia_Data\\Camera_Calibration_Files\\cam0_HD.png";
		str1 = workingFolder + "Munia_Data\\Camera_Calibration_Files\\cam1_HD.png";
	}
	else
	{
		str0 = workingFolder + "Munia_Data\\Camera_Calibration_Files\\cam0_2k.png";
		str1 = workingFolder + "Munia_Data\\Camera_Calibration_Files\\cam1_2k.png";
	}

		cv::imwrite(str0, cv_bgrIm[0]);
		cv::imwrite(str1, cv_bgrIm[1]);


		cv::Mat show;
		cv::hconcat(cv_bgrIm[0], cv_bgrIm[1], show);
		int w = cv_bgrIm[1].size().width;
		int h = cv_bgrIm[1].size().height;
		h = h / 2;
		cv::resize(show, show, cv::Size(w, h));
		cv::imshow("Captured Image", show);
		auto key = cv::waitKey(0);
		if (key == 'q')
		{
			cv::destroyAllWindows();
		}

	
	return true;
}


bool munia_function_class::extract_BGRIm(cv::Mat &cv_bgrIm0, cv::Mat &cv_bgrIm1, std::string str) 
{

	count = count + 1;
	std::string str1;
	if (count == 8)
	{
		if (liveBeatflag1 == 1) {
			str1 = "*";
			count = 0;
			liveBeatflag1 = 2;
		}
		else if (liveBeatflag1 == 2)
		{
			str1 = "*";
			count = 0;
			liveBeatflag1 = 3;
		}
		else if (liveBeatflag1 == 3)
		{
			str1 = "*";
			count = 0;
			liveBeatflag1 = 4;
		}
		else
		{
			str1 = "*";
			count = 0;
			liveBeatflag1 = 1;
		}
	}
	std::string text = "Live " + str1;
	

	sl::Mat sl_bgrIm[2];
	if (zedL.grab(runtime_parameters) == sl::ERROR_CODE::SUCCESS && zedR.grab(runtime_parameters) == sl::ERROR_CODE::SUCCESS)
	{

		zedL.retrieveImage(sl_bgrIm[0], sl::VIEW::LEFT, sl::MEM::CPU, F_resolution);
		zedR.retrieveImage(sl_bgrIm[1], sl::VIEW::LEFT, sl::MEM::CPU, F_resolution);

		cv_bgrIm0 = slMat2cvMat(sl_bgrIm[0]);
		cv_bgrIm1 = slMat2cvMat(sl_bgrIm[1]);

		int divident = 2;
		int divident_resize_w = 1.9;
		int divident_resize_h = 1.9;

		int Width = cv_bgrIm0.size().width * divident;
		int Height = cv_bgrIm0.size().height;

		cv::Mat im_all(Height, Width, CV_8UC4);
		cv_bgrIm0.copyTo(im_all(cv::Rect(0, 0, Width / divident, Height)));
		cv_bgrIm1.copyTo(im_all(cv::Rect(Width / divident, 0, Width / divident, Height)));
		cv::Mat resizedIm;
		cv::resize(im_all, resizedIm, cv::Size(Width / divident_resize_w, Height / divident_resize_h), 1);
		
		cv::Point org(25, 30);
		int fontFace = cv::FONT_HERSHEY_SIMPLEX;
		double fontScale = .7;
		cv::Scalar color = cv::Scalar(0, 0, 255);
		cv::putText(resizedIm, text, org, fontFace, fontScale, color);

		cv::namedWindow(str);
		cv::imshow(str, resizedIm);
		cv::waitKey(1);
	}else 
	{
	return false;
	}
	return true;
}

double munia_function_class::checkingMirrorPosition(std::vector<cv::Point3d> points3d, std::vector<cv::Point3d> &tpoints3d, bool right, bool realtime)
{
	
	if (!realtime) {
		zedL.setSVOPosition(0);
	}
	sl::Mat sl_xyz;
	cv::Mat F_im_xyz_32f = {cv::Mat()};
	if (zedL.grab(runtime_parameters) == sl::ERROR_CODE::SUCCESS) 
	{
		zedL.retrieveImage(sl_xyz, sl::VIEW::LEFT, sl::MEM::CPU, F_resolution);
		F_im_xyz_32f = slMat2cvMat(sl_xyz);
	}

	std::vector<cv::Point2f>current_triPoints;

	cv::Mat imroi;
	F_im_xyz_32f(F_mirror_roi).copyTo(imroi);

		
	findTriPoints(imroi, current_triPoints);
	


	//.........
	std::string item;
	switch (itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}
	//.......
	//item will be omitted


	if(idealPositionOFmirror)
	{
		ideal_triPoints.clear();
		findTriPoints(imroi, ideal_triPoints);

		std::ofstream extract_file;
		std::string str = workingFolder + "Munia_Data\\U\\";
		std::string fileName;

		if (right)
			fileName = str + item + "\\triPointsforUR.csv";
		else
			fileName = str + item + "\\triPointsforUL.csv";

		//std::ifstream file(fileName);

		
		extract_file.open(fileName, std::ios::trunc);
		for (int i = 0; i < ideal_triPoints.size(); i++) 
		{
			extract_file << ideal_triPoints[i].x << "," << ideal_triPoints[i].y << std::endl;
		}

		//extract_file.close();
	}

	ideal_triPoints.clear();
	if (right)
	{
		mamun_util::csv2dPoints2pointsVector(workingFolder + "Munia_Data\\U\\" + item + "\\triPointsforUR.csv", ideal_triPoints);
	}
	else
	{
		mamun_util::csv2dPoints2pointsVector(workingFolder + "Munia_Data\\U\\" + item + "\\triPointsforUL.csv", ideal_triPoints);
	}



	//cv::Mat affineMatrix = cv::getAffineTransform(current_triPoints, ideal_triPoints);
	cv::Mat affineMatrix = cv::estimateAffinePartial2D(ideal_triPoints, current_triPoints);

	double a, b, c, d, h, v;

	a = affineMatrix.at<double>(0, 0);
	b = affineMatrix.at<double>(0, 1);
	c = affineMatrix.at<double>(1, 0);
	d = affineMatrix.at<double>(1, 1);

	h = affineMatrix.at<double>(0, 2);
	v = affineMatrix.at<double>(1, 2);


	for (int i = 0; i < points3d.size(); i++)
	{
		cv::Point3d p = cv::Point3d(std::round(a*points3d[i].x + b * points3d[i].y + h), std::round(c*points3d[i].x + d * points3d[i].y + v), points3d[i].z);
		tpoints3d.push_back(p);
	}


	//imPosi = cv::imread(workingFolder + "Munia_Data\\Pics\\Contours.jpeg");

	for (int i = 0; i < points3d.size(); i++)
	{
		cv::circle(imPosi, cv::Point2i( points3d[i].x, points3d[i].y), 5, cv::Scalar(0, 0, 255), cv::FILLED);
		cv::circle(imPosi, cv::Point2i(tpoints3d[i].x, tpoints3d[i].y), 5, cv::Scalar(0, 255, 0), cv::FILLED);
	}

	for (const auto& point : ideal_triPoints) {
		cv::circle(imPosi, point, 5, cv::Scalar(0, 0, 255), cv::FILLED);
	}



	double angle_radians = std::atan2(c, a);
	double angle_degrees = angle_radians * 180.0 / CV_PI;
	if (angle_degrees < 0)
	{
		angle_degrees += 360.0;
	}


	if (right != right_mirror)
		angle_degrees = -361;

	if (flipping_mirror)
		angle_degrees = -999;

	cv::String text = "Angle = "+std::to_string(angle_degrees) +" deg";
	cv::putText(imPosi, text, cv::Point(25, 45), cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 0, 255));
	text = "x = " + std::to_string(h) + " pix, "+ "y = "+std::to_string(v)+" pix";
	cv::putText(imPosi, text, cv::Point(470, 45), cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 0, 255));





	if(angle_degrees==-361 ||angle_degrees==-999)
		cv::imwrite(workingFolder + "Munia_Data\\Pics\\pointsPosition.jpeg", imPosi);
	else 
	{
		std::string date_userWanted;
		date_userWanted = get_current_date_and_time(1, userWantedTimeSetting);

		std::string str0 = workingFolder + "Munia_Data\\R\\Radius_" + item + "_" + date_userWanted + ".csv";
		std::ifstream file(str0);
		std::string indexFname = workingFolder + "Munia_Data\\R\\" + "Index_" + item + ".csv";
		std::ifstream indFile(indexFname);
		if (indFile.is_open() && file.is_open())
		{
			std::string line;
			std::getline(indFile, line);
			std::stringstream ss(line);
			char comma;
			ss >> n >> comma >> m;
			indFile.close();
		}
		else { m = 0; n = 0; }


		std::string dirName = workingFolder + "Munia_Data\\R\\PointsPosition";
		char* dir = (char*)dirName.data();
		_mkdir(dir);

		dirName = workingFolder + "Munia_Data\\R\\PointsPosition\\" + item + "_" + date_userWanted;
		char* dir1 = (char*)dirName.data();
		_mkdir(dir1);

		std::string str;
		if (right)
			str = dirName + "\\" + item + "_" + "RH-" + std::to_string(n + 1) + ".jpg";
		else
			str = dirName + "\\" + item + "_" + "LH-" + std::to_string(m + 1) + ".jpg";

		cv::imwrite(str, imPosi);
	}





	return angle_degrees;
}


bool munia_function_class::findTriPoints(cv::Mat image, std::vector<cv::Point2f> &triPoints)
{
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	//cv::imshow("Gray", gray);
	cv::imwrite(workingFolder + "Munia_Data\\Pics\\grayIM0.jpeg", gray);

	cv::threshold(gray, gray, 0, 255, cv::THRESH_OTSU);
	cv::imwrite(workingFolder + "Munia_Data\\Pics\\grayIM1.jpeg", gray);

	cv::GaussianBlur(gray, gray, cv::Size(9, 9), 0);
	cv::imwrite(workingFolder + "Munia_Data\\Pics\\blurIM.jpeg", gray);
	//cv::imshow("Gray1", gray);
	//cv::waitKey(0);




	int threshL = 9;
	int threshU = threshL*2;
	cv::Mat canny_output;
	cv::Canny(gray, canny_output, threshL, threshU);
	//cv::imshow("Canny", canny_output);
	//cv::waitKey(0);

	//cv::Mat dilatedEdges;
	//cv::dilate(canny_output, dilatedEdges, cv::Mat(), cv::Point(-1, -1), 2);
	//cv::imshow("DilatedEdges", dilatedEdges);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);


	for (size_t i = 0; i < contours.size(); ++i) 
	{
		double area = cv::contourArea(contours[i]);
		if (area > 500000)
		{
			cv::drawContours(mask, contours, static_cast<int>(i), cv::Scalar(255), cv::FILLED);

		}
		//if (hierarchy[i][2] != -1 || hierarchy[i][3] != -1) 
		//{ 
		//	cv::drawContours(mask, contours, static_cast<int>(i), cv::Scalar(255), cv::FILLED);
		//}
	}

	cv::imwrite(workingFolder + "Munia_Data\\Pics\\grayIM.jpeg", mask);
	//cv::imshow("mask", mask);

	//cv::threshold(mask, mask, 1, 255, cv::THRESH_BINARY);
	//cv::imshow("mask1", mask);
	//cv::waitKey(0);

	//thresh = 10;
	//cv::Canny(mask, canny_output, thresh, thresh * 2);
	//cv::dilate(canny_output, dilatedEdges, cv::Mat(), cv::Point(-1, -1), 2);

	//cv::imshow("mask1", canny_output);
	//cv::waitKey(0);

	//contours.clear();
	//cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	mask = cv::Mat::ones(image.size(), CV_8UC1) * 255;

	double largest_chord = 0;
	double largest_chord1 = 0;
	double smallest_chord = DBL_MAX;

	cv::Point p1, p2, p3, p4, pm;

	float angle;
	cv::Size2f axes;
	cv::Point center; cv::Point vertex1, vertex2, coVertex1, coVertex2;
	cv::RotatedRect ellipse;

	for (const auto& contour : contours) {
		double area = cv::contourArea(contour);
		if (area > 500000)
		{
			ellipse = cv::fitEllipse(contour);

			cv::Size axes = ellipse.size;

			//.......................

			center = ellipse.center;
			axes = ellipse.size;
			axes.width *= 0.5;  // Adjust the width
			axes.height *= 0.5; // Adjust the height
			ellipse.size = axes;

			angle = ellipse.angle;

			float a = max(axes.width, axes.height) / 2;
			float b = min(axes.width, axes.height) / 2;

			vertex1= cv::Point(center.x + a * cos(angle * CV_PI / 180), center.y + a * sin(angle * CV_PI / 180));
			vertex2= cv::Point(center.x - a * cos(angle * CV_PI / 180), center.y - a * sin(angle * CV_PI / 180));
			coVertex1= cv::Point(center.x + b * cos((angle + 90) * CV_PI / 180), center.y + b * sin((angle + 90) * CV_PI / 180));
			coVertex2= cv::Point(center.x - b * cos((angle + 90) * CV_PI / 180), center.y - b * sin((angle + 90) * CV_PI / 180));



			//..................
#if 01

			for (size_t i = 0; i < contour.size(); ++i) 
			{
				for (size_t j = i + 1; j < contour.size(); ++j) 
				{

					double chord_length = sqrt(pow(contour[j].x - contour[i].x, 2) + pow(contour[j].y - contour[i].y, 2));

					if (chord_length > largest_chord) 
					{
						largest_chord = chord_length;

						p1 = contour[i];
						p2 = contour[j];

						float m = 2;
						float n = 2;
						pm = cv::Point((n*p1.x + m*p2.x)/(m+n), (n*p1.y + m*p2.y)/(m+n));
					}
				}
			}

			for (size_t i = 0; i < contour.size(); ++i)
			{
				double chord_length = sqrt(pow(pm.x - contour[i].x, 2) + pow(pm.y - contour[i].y, 2));
				if (chord_length < smallest_chord)
				{
				smallest_chord = chord_length;
				p3 = contour[i];
				}
				//if (chord_length > largest_chord1)
				//{
				//	largest_chord1 = chord_length;
				//	p4 = contour[i];
				//}
			}
#endif

		}
	}


	//std::vector<cv::Point> Points;

	triPoints.push_back(center);
	triPoints.push_back(vertex1);
	triPoints.push_back(coVertex1);
	//triPoints.push_back(p3);

	//std::transform(Points.begin(), Points.end(), std::back_inserter(triPoints), [](const cv::Point& p) {
	//	return cv::Point2f(static_cast<float>(p.x), static_cast<float>(p.y));
	//});


#if 01
	for (size_t i = 0; i < contours.size(); ++i) {
		double area = cv::contourArea(contours[i]);
		if (area > 500000)
		{
			cv::drawContours(mask, contours, static_cast<int>(i), cv::Scalar(0)/*, cv::FILLED*/);
		}
	}

	for (const auto& point : triPoints) {
		cv::circle(mask, point, 5, cv::Scalar(255,0, 0), cv::FILLED);
	}

	cv::circle(mask, p1, 5, cv::Scalar(0, 255, 0), cv::FILLED);
	cv::circle(mask, p3, 5, cv::Scalar(0, 255, 0), cv::FILLED);


	cv::ellipse(mask, ellipse, cv::Scalar(0, 255, 0), 1);

	cv::imwrite(workingFolder + "Munia_Data\\Pics\\Contours.jpeg", mask);

	mask.copyTo(imPosi);
	//cv::imshow("con", mask);
	//cv::waitKey(0);
#endif

	int h = image.rows;
	int w = image.cols;

	if (p3.y - h / 2 > 0)
		right_mirror = 1;
	else
		right_mirror = 0;

	if (p1.x - w / 2 > 0)
		flipping_mirror = 1;
	else
		flipping_mirror = 0;

	return 1;
}



bool munia_function_class::load_3d_data(cv::Mat& im_s_32fc4, int start_pos, int using_frames_from, int using_frames_to, bool realtime)
{


	if (!realtime){zedL.setSVOPosition(start_pos);zedR.setSVOPosition(start_pos);}

	
	sl::Mat sl_xyz[2];
	cv::Mat F_im_xyz_32f[2] = { cv::Mat(), cv::Mat() };
	cv::Mat F_im_s_32f = cv::Mat::zeros(F_mirror_roi.size(), CV_32FC4);
	cv::Mat im_s_32f = cv::Mat();
	cv::Mat imroi = cv::Mat();
	int count = 0;

	for (int h = 0; h < using_frames_to; h++)
	{

		if (zedL.grab(runtime_parameters) == sl::ERROR_CODE::SUCCESS && zedR.grab(runtime_parameters) == sl::ERROR_CODE::SUCCESS)
		{

			zedL.retrieveMeasure(sl_xyz[0], sl::MEASURE::XYZ, sl::MEM::CPU, F_resolution);
			zedR.retrieveMeasure(sl_xyz[1], sl::MEASURE::XYZ, sl::MEM::CPU, F_resolution);

			if (h >= using_frames_from) {
				F_im_xyz_32f[0] = slMat2cvMat(sl_xyz[0]);
				F_im_xyz_32f[1] = slMat2cvMat(sl_xyz[1]);
				

				//int x = F_im_xyz_32f[0].cols;
				//int y = F_im_xyz_32f[0].rows;
				//cv::imshow("0", F_im_xyz_32f[0]);
				//cv::imshow("1", F_im_xyz_32f[1]);
				//cv::waitKey(0);
				//cv::Scalar meanValue_0 = cv::mean(im_xyz_0_32f_inrage, mask_00);
				//cv::Scalar meanValue_1 = cv::mean(im_xyz_1_32f_inrage, mask_1);
				//double difIm = std::round(meanValue_1[2] - meanValue_0[2]);
				//cv::Vec4f  dif_vec4c= F_im_xyz_32f[0].at<cv::Vec4f>(y *0.5, x *0.55)- F_im_xyz_32f[1].at<cv::Vec4f>(y*0.5, x*0.45);
				//int dif_depthValue = std::round(dif_vec4c.val[2]);
				
				cv::add(F_im_xyz_32f[1], dif_depthValue, F_im_xyz_32f[1]); //18-09-2024

				/*cv::Mat difIm = F_im_xyz_32f[0] - F_im_xyz_32f[1];
				cv::imshow("a11", difIm);
				cv::waitKey(0);*/

				excludeNoisePart(F_im_xyz_32f[0]);

#if 0
				extract_combine_xyz(F_im_xyz_32f[0], F_im_xyz_32f[1], F_mirror_roi, im_s_32f);
				im_s_32f.copyTo(imroi);

#else
				combine(F_im_xyz_32f[0], F_im_xyz_32f[1], im_s_32f);
				im_s_32f(F_mirror_roi).copyTo(imroi);
#endif
				F_im_s_32f = F_im_s_32f + imroi;
				count++;
			}
		}

	}
		F_im_s_32f = F_im_s_32f / count;
		F_im_s_32f.copyTo(im_s_32fc4);
 return true;	
}

bool munia_function_class::excludeNoisePart(cv::Mat &im_xyz_32f) 
{

	int width = im_xyz_32f.cols;
	int height = im_xyz_32f.rows;
	float percentage = 0.55;
	cv::Mat white_black_maskIm = cv::Mat::zeros(height, width, CV_8UC1);
	cv::Mat leftSideWhiteIm = cv::Mat::ones(height, width*percentage, CV_8UC1)*255;
	leftSideWhiteIm.copyTo(white_black_maskIm(cv::Rect(0, 0, width*percentage, height)));
	cv::Mat resultIm;
	im_xyz_32f.copyTo(resultIm, white_black_maskIm);
	resultIm.copyTo(im_xyz_32f);
	return true;
}

bool munia_function_class::combine(const cv::Mat& im_xyz_0_32f, const cv::Mat& im_xyz_1_32f, cv::Mat& im_xyz_32f) 
{

	cv::Mat mask_0;

	cv::Mat channel;
	int paint_radius = 1;

	cv::extractChannel(im_xyz_0_32f, channel, 2);

	cv::inRange(channel, lower_bound, upper_bound, mask_0);

	cv::Mat mask_00;
	mask_0.copyTo(mask_00);



	cv::Mat im_xyz_0_32f_inrage;
	im_xyz_0_32f.copyTo(im_xyz_0_32f_inrage, mask_0);
	
	panitIm_4CH(im_xyz_0_32f_inrage, paint_radius);
	panitIm_1CH(mask_0, paint_radius);


	cv::bitwise_not(mask_0, mask_0);
	cv::Mat white_img = cv::Mat::ones(im_xyz_0_32f.size(), CV_8UC1) * 255;
	cv::Mat mask_0_bitnot;
	white_img.copyTo(mask_0_bitnot, mask_0);


	cv::Mat im_xyz_1_32f_inrage_h, im_xyz_1_32f_inrage_h_trans;
	cv::warpPerspective(im_xyz_1_32f, im_xyz_1_32f_inrage_h, homography_matrix, im_xyz_1_32f.size());
	cv::transform(im_xyz_1_32f_inrage_h, im_xyz_1_32f_inrage_h_trans, R);




	cv::Mat ones = cv::Mat::ones(im_xyz_1_32f_inrage_h_trans.size(), CV_32F);
	cv::Mat im_xyz_1_32f_inrage_h_trans_c4;
	cv::merge(std::vector<cv::Mat>{im_xyz_1_32f_inrage_h_trans, ones}, im_xyz_1_32f_inrage_h_trans_c4);

	cv::Mat im_xyz_1_32f_h_r;
	im_xyz_1_32f_inrage_h_trans_c4.copyTo(im_xyz_1_32f_h_r, mask_0_bitnot);


	cv::Mat  mask_1;
	cv::extractChannel(im_xyz_1_32f_h_r, channel, 2);

	cv::inRange(channel, lower_bound, upper_bound, mask_1);

	cv::Mat im_xyz_1_32f_inrage;
	im_xyz_1_32f_h_r.copyTo(im_xyz_1_32f_inrage, mask_1);


	panitIm_4CH(im_xyz_1_32f_inrage, paint_radius);

	//18-09-2024
#if 0
	cv::Scalar meanValue_0 = cv::mean(im_xyz_0_32f_inrage, mask_00);
	cv::Scalar meanValue_1 = cv::mean(im_xyz_1_32f_inrage, mask_1);
	double difIm = std::round(meanValue_1[2] - meanValue_0[2]);
	
	im_xyz_1_32f_inrage = im_xyz_1_32f_inrage - difIm;
	
	//im_xyz_1_32f_inrage = im_xyz_1_32f_inrage + 50;
#endif
	//18-09-2024

	im_xyz_32f = im_xyz_0_32f_inrage + im_xyz_1_32f_inrage;

	cv::imwrite(workingFolder+"Munia_Data\\Pics\\DepthIm_cam0.jpg", im_xyz_0_32f_inrage);
	cv::imwrite(workingFolder+"Munia_Data\\Pics\\DepthIm_cam1.jpg", im_xyz_1_32f_inrage);
	cv::imwrite(workingFolder+"Munia_Data\\Pics\\DepthIm_cam_01.jpg", im_xyz_32f);

	return true;
}

void munia_function_class::extract_combine_xyz(const cv::Mat& im_xyz_0_32f, const cv::Mat& im_xyz_1_32f, const cv::Rect& roi, cv::Mat& im_xyz_32f)
{
	int w = im_xyz_0_32f.cols, h = im_xyz_0_32f.rows;

	cv::Mat rotation_matrix = (cv::Mat_<float>(3, 3) <<
#if 0
		-0.99558625, -0.09381468, 0.00261385,
		0.09383883, -0.99552015, 0.01157142,
		0.00151657, 0.01176563, 0.99992963);
#elif 01
		- 0.9998658766899475, -0.00142582406537017, -0.01631550357105943,
		0.001548589421131943, -0.999970568136224, -0.007514301838640603,
		-0.0163043093029849, -0.007538560011835854, 0.9998386567897346);


#else
		-0.9997697536197702, 0.003369822322549622, -0.02119160316206601,
	-0.002408911727151469, -0.9989747334296619, -0.0452070692859717,
	-0.02132221591098899, -0.0451456118205336, 0.9987528407178595);
#endif
	cv::Mat homography_matrix = (cv::Mat_<float>(3, 3) <<
#if 0
		-1.01509734e+00, 1.12415116e-03, 2.20412879e+03,
		6.55444632e-03, -1.02861865e+00, 1.40495525e+03,
		8.63231658e-06, -1.21343260e-06, 1.00000000e+00);
#elif 01
		- 1.000271479471758, -0.0007254480445206442, 1903.113267843315,
		-0.00353150704792421, -0.9911201317839217, 934.2343553582085,
		-9.265997694191426e-06, 1.135481553313284e-06, 1);
#else
		- 0.9838312665450814, 0.00222867959114087, 2147.398857463309,
	-0.003515961058939372, -0.9806969082295848, 1066.978067793611,
	-1.251091197185492e-06, 2.465876549511818e-07, 1);

#endif
	cv::Mat translation = (cv::Mat_<float>(3, 1) << 
#if 0
		3.413210015, 24.11731368, -0.615362316);
#elif 01

		- 3.701950007803722,
		-24.32471169059199,
		-0.286539818942883);

#else
		-4.764017465109632,
	-17.79103539241597,
	3.296365433741073);
#endif
	im_xyz_32f = cv::Mat::zeros(roi.size(), CV_32FC4);
	for (int sy = 0; sy < roi.height; sy++)
	{
		int y = roi.y + sy;
		for (int sx = 0; sx < roi.width; sx++)
		{
			int x = roi.x + sx;
			cv::Vec4f v0 = im_xyz_0_32f.at<cv::Vec4f>(y, x);
#if 0 // this limit is for copnvex shape
			if ( v0.val[2] >= 280 && v0.val[2] <= 480)
			{
				im_xyz_32f.at<cv::Vec4f>(sy, sx) = v0;
			}
#else // this limit is for concave shape
			if ( v0.val[2] >= 500 && v0.val[2] <= 730)
			{
				im_xyz_32f.at<cv::Vec4f>(sy, sx) = v0;
			}
#endif
			else
			{
				cv::Mat p1 = (cv::Mat_<float>(3, 1) << x, y, 1);
				cv::Mat p1_ = homography_matrix * p1;
				int x_ = std::round(p1_.at<float>(0, 0) / p1_.at<float>(2, 0));
				int y_ = std::round(p1_.at<float>(1, 0) / p1_.at<float>(2, 0));

				if (y_ >= 0 && y_ < im_xyz_1_32f.size().height && x_ >= 0 && x_ < im_xyz_1_32f.size().width) {

					cv::Vec4f v1 = im_xyz_1_32f.at<cv::Vec4f>(y_, x_);
					cv::Mat v1_ = (cv::Mat_<float>(3, 1) << v1.val[0], v1.val[1], v1.val[2]);
					cv::Mat v1_0(cv::Size(1, 3), CV_32FC1);             //cv::Mat v1_0(cv::Mat_<float>(3, 1)); same line
					v1_0 = rotation_matrix * v1_ + translation;
					float v1_z = v1_0.at<float>(2, 0);
#if 0 // this limit is for convex shape
					if (v1_z >= 280 && v1_z <= 480)
					{
						im_xyz_32f.at<cv::Vec4f>(sy, sx) = cv::Vec4f(v1_0.at<float>(0, 0), v1_0.at<float>(1, 0), v1_0.at<float>(2, 0), 0);
					}
#else // this limit is for concave shape

					if (v1_z >= 500 && v1_z <= 730)
					{
						im_xyz_32f.at<cv::Vec4f>(sy, sx) = cv::Vec4f(v1_0.at<float>(0, 0), v1_0.at<float>(1, 0), v1_0.at<float>(2, 0), 0);
					}
				}
#endif
			}
		}
	}

	cv::imwrite(workingFolder+"Munia_Data\\Pics\\DepthIm_cam_k1.jpg", im_xyz_32f);

}

bool munia_function_class::extract_csvDepthPc(bool realtime, uint filter_index) {
	cv::Mat cvPointCloud_image = cv::Mat();;
	cv::Mat Depthimage = cv::Mat();

	load_3d_data(cvPointCloud_image, F_start_pos, F_using_frames_from, F_using_frames_to, realtime);

	cvPointCloud_image.copyTo(Pcimage);
	pc_to_depth_Im(Pcimage, Depthimage);
	
	//cv::imwrite(workingFolder + "Munia_Data\\Pics\\PCImage.jpg", Pcimage);
	cv::imwrite(workingFolder + "Munia_Data\\Pics\\DepthImage.jpg", Depthimage);


	cv::Mat tempIm;
	Depthimage.copyTo(tempIm);

	switch (filter_index)
	{
	case 0:
		bilateralFilter(tempIm, Depthimage, ksize, 10, 10);
		break;
	case 1:
	{
		medianBlur(tempIm, Depthimage, ksize);

		

#if 0
		cv::imwrite("Munia_Data\\DepthImage1.jpg", Depthimage);

		std::vector<cv::Mat> pyramid_imVec;
		cv::buildPyramid(Depthimage, pyramid_imVec, 5);
		cv::imwrite("Munia_Data\\Pyramid1.jpg", pyramid_imVec[0]);
		cv::imwrite("Munia_Data\\Pyramid2.jpg", pyramid_imVec[1]);
		cv::imwrite("Munia_Data\\Pyramid3.jpg", pyramid_imVec[2]);
		cv::imwrite("Munia_Data\\Pyramid4.jpg", pyramid_imVec[3]);
		cv::imwrite("Munia_Data\\Pyramid5.jpg", pyramid_imVec[4]);

		cv::Mat im_tmp1;
		cv::resize(Depthimage, im_tmp1, cv::Size(Depthimage.cols / 5, Depthimage.rows / 5));
		cv::imwrite("Munia_Data\\DepthImage2.jpg", im_tmp1);

		medianBlur(im_tmp1, im_tmp1, ksize);
		cv::resize(im_tmp1, Depthimage, Depthimage.size());
		cv::imwrite("Munia_Data\\DepthImage3.jpg", Depthimage);


#endif
	}
		break;
	case 2:
		//cv::getGaussianKernel(ksize, 0);
		GaussianBlur(tempIm, Depthimage, cv::Size(ksize, ksize),0);
		break;
	}
	
	Depthimage.copyTo(inputDepthImage);
	//cv::imwrite(workingFolder+"Munia_Data\\Pics\\DepthImage_filter.jpg", inputDepthImage);
	return true;
}


bool munia_function_class::selectPoint(bool needFn, bool right, bool mouse_click, cv::Mat image, std::string str) 
{


	if (!needFn)
		return true;
	std::vector<cv::Point3d> points;

	std::string item;
	switch (itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	if (mouse_click) 
	{
		struct MouseData
		{
			cv::Mat* img;
			std::vector<cv::Point3d>* pts;

			cv::String win_Name;
			int n;

		};
		cv::String win_name = "Depth_Image";

		int w = image.cols;
		int h = image.rows;
		if (!image.empty()) 
		{

			MouseData data;
			data.img = &image;
			data.pts = &points;
			data.win_Name = win_name;
			data.n = 0;
			
			cv::namedWindow(win_name, cv::WINDOW_NORMAL);
			cv::resizeWindow(win_name, w * imWindowSizeConPercentage, h * imWindowSizeConPercentage);

			cv::imshow(win_name, image);
			cv::setMouseCallback(win_name, mouse_click_event, &data);
			cv::waitKey(0);
		}



		if (right)
		{
			cv::imwrite(workingFolder+"Munia_Data\\Pics\\DepthIm_selection_"+str+"_"+item+"-RHM.jpg", image);
		}
		else 
		{
			cv::imwrite(workingFolder + "Munia_Data\\Pics\\DepthIm_selection_"+str+ "_" + item + "-LHM.jpg", image);
		}
		cv::destroyAllWindows();
	}

		Points_vector(points, !mouse_click);

		std::vector<cv::Point3d> points1;
		for (auto& point : points) {
			points1.push_back(cv::Point3d(point.x, point.y, point.z)); //x, y confusing
		}

		std::ofstream extract_file;
		
		std::string str0=workingFolder+"Munia_Data\\U\\"+item+"\\";

		if (right) {
			extract_file.open(str0+str + "UR.csv");}
		else {
			extract_file.open(str0+str + "UL.csv");}
		for (int i = 0; i < points1.size(); i++) {
			extract_file << points1[i].x << "," << points1[i].y << ","<< points1[i].z << endl;
		}

		return true;
}

bool munia_function_class::points_optimum_UforRadius(std::string derec, std::vector<cv::Point3d> &resultVector, std::vector<cv::Point3d>&opU, bool right, bool real_time, bool sensingMode, int depthModeIndex, bool resolution2K, int filterIndex)
{
	std::string strL, strR;
	strL = derec + "\\OptimumUL";
	strR = derec + "\\OptimumUR";

	std::vector< std::vector<cv::Point3d>> vectorsVector;
	bool a = csvToVectors(vectorsVector, resultVector, strL, strR);

	std::vector< std::vector<double>>d_vecsVector;
	d_vecsVector.clear();

	std::vector<double>obs_rVec;
	std::vector<double>d_vec;
	std::vector<double>opt_rVec;



	std::string str1;
	std::string path0, path1;

	std::string item;
	switch (itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	if (!right)
	{
		points_forU.clear();
		mamun_util::pointsCsv2pointsVector(workingFolder+"Munia_Data\\U\\"+item+"\\PointsforUL.csv", points_forU);

		if (points_forU.empty())
			return false;

		for (int i = 1; ; i++) 
		{
		str1 = "LH";
		path0 = derec + "\\" + str1 + std::to_string(i) + "-cam0.svo";
		path1 = derec + "\\" + str1 + std::to_string(i) + "-cam1.svo";

		filenames.clear();
		filenames.push_back(path0);
		filenames.push_back(path1);

		bool a = open_svo(filenames, depthModeIndex, resolution2K, sensingMode);

		if (a) 
		{
			std::vector<double> vec;
			extract_csvDepthPc(real_time, filterIndex);
			extract_Radius(inputDepthImage, vec, right, str1 + std::to_string(i), 0);
			closecamera();
			d_vecsVector.push_back(vec);

		}
		else 
		{
			break;
		}

		}

	}
	else 
	{

		points_forU.clear();
		mamun_util::pointsCsv2pointsVector(workingFolder+"Munia_Data\\U\\" + item + "\\PointsforUR.csv", points_forU);

		if (points_forU.empty())
			return false;

		for (int i = 1; ; i++) {
			str1 = "RH";
			path0 = derec + "\\" + str1 + std::to_string(i) + "-cam0.svo";
			path1 = derec + "\\" + str1 + std::to_string(i) + "-cam1.svo";

			filenames.clear();
			filenames.push_back(path0);
			filenames.push_back(path1);

			bool a = open_svo(filenames, depthModeIndex, resolution2K, sensingMode);

			if (a) 
			{
				std::vector<double> vec;
				extract_csvDepthPc(real_time, filterIndex);
				extract_Radius(inputDepthImage, vec, right, str1 + std::to_string(i), 0);
				closecamera();
				d_vecsVector.push_back(vec);
			}
			else 
			{
				break;
			}

		}
	}


	if (vectorsVector.size() != d_vecsVector.size())
		return false;

	int vn = vectorsVector.size();

	for (int p = 0; p<vectorsVector[0].size(); p++) {
		obs_rVec.clear();
		d_vec.clear();

		for (int v = 0; v < vn; v++)
		{
			obs_rVec.push_back(vectorsVector[v][p].z);

			d_vec.push_back(d_vecsVector[v][p]);
		}


		cv::Point3d P= optimum_U(obs_rVec, opt_rVec, d_vec);

		opU.push_back(P);
		
		resultVector[p].z = P.x;
	}


	std::ofstream extract_file, extract_file1;
	

	std::string str5 = workingFolder+"Munia_Data\\U\\"+item+"\\";
	std::string fileName;
	
	if(right)
		fileName = str5 + "PointsforUR.csv";
	else
		fileName = str5 + "PointsforUL.csv";


	std::ifstream file(fileName);
	extract_file.open(fileName);
	for (int i = 0; i < resultVector.size(); i++) {
		extract_file << resultVector[i].x << "," << resultVector[i].y << "," << resultVector[i].z << endl;
	}

	if(right)
		fileName = str5 + "OptimumUR_R.csv";
	else
		fileName = str5 + "OptimumUR_L.csv";

	std::ifstream file1(fileName);
	extract_file1.open(fileName);
	extract_file1 <<"Point's Index"<<","<< "Optimum U" << "," << "Error" << "," << "# of iterations" << "," << "";
	for (int i = 0; i < opt_rVec.size() / opU.size(); i++) {
		extract_file1 <<","<< "Mirror" + std::to_string(i+1);
	}
	extract_file1 << endl;
	for (int i = 0; i < opU.size(); i++) {
		extract_file1 <<"Point"+std::to_string(i+1)<<","<< opU[i].x << "," << opU[i].y << "," << opU[i].z<<","<<"Optimum Radius";
		
		for (int k = i* opt_rVec.size() / opU.size(); k < i*opt_rVec.size() / opU.size()+opt_rVec.size()/ opU.size(); k++) {
			extract_file1 << "," << opt_rVec[k];
		}
		extract_file1 << endl;
	}

	return true;
}


bool munia_function_class::change_points_UforRadius(cv::Mat depthIm, std::vector<cv::Point3d> pointsRforU, bool R2U) {
	
	double d, u;

	for (int i = 0; i < pointsRforU.size(); i++) {

		if (R2U) 
		{
			d = depthIm.at<float>(pointsRforU[i].y, pointsRforU[i].x);
			u = calc_u(pointsRforU[i].z, d, concave);
		

			for (int j = 0; j < points_forR.size(); j++) {
				if (points_forR[j].x == pointsRforU[i].x && points_forR[j].y == pointsRforU[i].y)					
					points_forR[j].z = u;
			}
		
		}
		else
		{
			for (int j = 0; j < points_forR.size(); j++) {
				if (points_forR[j].x == pointsRforU[i].x && points_forR[j].y == pointsRforU[i].y)
					points_forR[j].z = pointsRforU[i].z;
			}
		}
	}


	std::ofstream extract_file;
	
	
	std::string str = workingFolder+"Munia_Data\\U\\";
	std::string fileName;
	fileName = str + "forRdata.csv";
	std::ifstream file(fileName);
	
		extract_file.open(fileName);
		for (int i = 0; i < points_forR.size(); i++) {
			extract_file << points_forR[i].x << "," << points_forR[i].y << "," << points_forR[i].z << endl;
			
		}
	

	return true;
}

bool munia_function_class::extract_csv_UforRadius(bool needFn, cv::Mat depthIm, bool right) {

	if (!needFn)
		return true;


	std::vector<cv::Point3d> points;
	

		int col = depthIm.rows;
		int row = depthIm.cols;


		points.reserve(row * col);

		for (int x = 0; x < col; x++) { // going through colunm
			for (int y = 0; y < row; y++) { // going through row

				double d = depthIm.at<float>(x, y);

					if (depthIm.at<float>(x, y)) // image.at<float>(colunm, row);
					{
						double u = calc_u(radius, d, concave);
						
						if(u>200 &&u<350)
							points.push_back(cv::Point3f(y, x, u));
						else {
							points.push_back(cv::Point3f(y, x, 0));
						}
					}
					else {
						points.push_back(cv::Point3f(y, x, 0));
					}


			}

		}


	std::ofstream extract_file;


#if 0
	std::string str = "Munia_Data\\U\\";

	std::string fileName = str + "forRdata.csv";
	extract_file.open(fileName);
	for (int i = 0; i < points.size(); i++) {
		extract_file << points[i].x << "," << points[i].y << "," << points[i].z << endl;
	}

#else

	std::string str = workingFolder+"Munia_Data\\U\\forRdatas\\";

	std::string fileName;
	if (right) {
		fileName = str + "forRdataRH" + std::to_string(i++) + ".csv";
	}
	else {
		fileName = str + "forRdataLH" + std::to_string(j++) + ".csv";
	}
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		extract_file.open(fileName);
		for (int i = 0; i < points.size(); i++) {
			extract_file << points[i].x << "," << points[i].y << "," << points[i].z << endl;

		}
	}
#endif
	
	return true;
}

bool munia_function_class::extract_Radius(cv::Mat &depthIm,std::vector<double> &vec, bool right, std::string userNote, bool rad, int itemIndx) {

#if 0
	
	fs::path dir = "R";
	if (!std::filesystem::exists(dir))
	{
		try {

			fs::create_directory(dir);
			std::cout << "Directory created: " << dir << std::endl;
		}
		catch (std::filesystem::filesystem_error const& ex)
		{
			// Print the exception details
		}
	}
#endif
	double u = 0;
	std::ofstream extract_file, indexFile;

	
	std::string str0, str, str2, indexFname;

	

	std::string item;
	switch (itemIndx)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}


	str0 = workingFolder+"Munia_Data\\R\\Radius_" + item;
	std::string dirN = workingFolder+"Munia_Data\\R\\Index_"+item;



#if 0
	if (diferentExecutionR) {
		if (right) {

			indexFname = dirN + "RH.csv";

			std::ifstream file(indexFname);
			if (file.is_open()) 
			{
				std::string line; 
				std::getline(file, line);
				std::stringstream ss(line); 
				ss >> n ; 
				file.close(); 
			}

			n++;
			rlR++;
			str = str0+"RH-" + std::to_string(n) + ".csv";
			str2 = "Right " + std::to_string(rlR);


			indexFile.open(indexFname);
			indexFile << std::to_string(n);
			indexFile.close();

			diferentExecutionR = 0;
		}

	}
	else {
		if (right) {
			n;
			rlR++;
			str = str0+"RH-" + std::to_string(n) + ".csv";
			str2 = "Right " + std::to_string(rlR);

		}
		
	}


	if (diferentExecutionL) {
		if (!right) 
		{

			indexFname = dirN + "LH.csv";
			std::ifstream file(indexFname);
			if (file.is_open())
			{
				std::string line;
				std::getline(file, line);
				std::stringstream ss(line);
				ss >> m;
				file.close();
			}

			m++;
			rlL++;
			str = str0 + "LH-" + std::to_string(m) + ".csv";
			str2 = "Left " + std::to_string(rlL);

			indexFile.open(indexFname);
			indexFile << std::to_string(m);
			indexFile.close();
			diferentExecutionL = 0;
		}
	}
	else {
		if (!right)
		{
			m;
			rlL++;
			str = str0 + "LH-" + std::to_string(m) + ".csv";
			str2 = "Left " + std::to_string(rlL);

		}
	}
#else

	std::string date, date_userWanted, time, date_and_time;
	date = get_current_date_and_time(1);
	date_userWanted = get_current_date_and_time(1, userWantedTimeSetting);
	time = get_current_date_and_time(2);
	date_and_time = get_current_date_and_time(3);





	str = str0 + "_" + date_userWanted + ".csv";
	std::ifstream file(str);


	indexFname = dirN + ".csv";
	std::ifstream indFile(indexFname);
	
		if (indFile.is_open() && file.is_open())
		{
			std::string line;
			std::getline(indFile, line);
			std::stringstream ss(line);
			char comma;
			ss >> n >> comma >> m;
			indFile.close();
		}
		else { n = 0; m = 0; }

		if (right)
		{
			n++;
			str2 = "RH " + std::to_string(n);
		}
		else
		{
			m++;
			str2 = "LH " + std::to_string(m);
		}

		indexFile.open(indexFname);
		indexFile << std::to_string(n) << "," << std::to_string(m);
		indexFile.close();


#endif


#if 0

	if (!file.is_open()) {
		date_and_time = get_current_date_and_time();
		if (right) {

			extract_file.open(str, std::ios::out | std::ios::app);
			extract_file << "File-" << std::to_string(n)<<"," << "" << "," << date_and_time << endl;
			extract_file.close();

		}
		else {
			extract_file.open(str, std::ios::out | std::ios::app);
			extract_file <<"File-"<< std::to_string(m) << ","<<""<<"," << date_and_time << endl;
			extract_file.close();
		}
	}

#else
		
	std::string firstLine = "Point ";

	if (!file.is_open()) {

			extract_file.open(str, std::ios::out | std::ios::app);
			extract_file << "Minimum" << "," << " Limit = " << "," << std::to_string(lowerLimit) + " mm" << "," << "and" << "," << "Maximum" << "," << " Limit = " << "," << std::to_string(upperLimit) + " mm" << endl;
			extract_file << "Date" << "," << "Time" << "," << "User Note" << "," << "Auto Index" << "," << "JEC/JALL" << ",";

			for (int i = 0; i < points_forU.size(); i++)
			{
				extract_file << firstLine + std::to_string(i+1);
				if (i != points_forU.size() - 1) {
					extract_file << ",";
				}
			}

			extract_file << ","<<"EC Average"<<","<<"All Average"<<","<<"Status";
			extract_file << endl;
			extract_file.close();

		}
#endif


	std::string jtype;
	if (judge_by_ec_only)
		jtype = "JEC";
	else
		jtype = "JALL";
	

	extract_file.open(str, std::ios::out | std::ios::app);
	extract_file <<date<<","<< time << ","<<userNote<<"," << str2 << "," << jtype <<",";

#if 01
	int decision = 1;
	double r=0;

	count_checkingR = 0;
	sum_checkingR = 0;
	average_checkingR = 0;
	coun_checkingR = 0;


	double sumR = 0, avgR=0, avgECR=0;

	


	if(calibrationFromIdealMirror) //21-Agust-2024
	{
		for (int i = 0; i < points_forU.size(); i++) //27-january -2025
		{
			for (int j = 0; j < points_forR.size(); j++) 
			{
				if (points_forR[j].x == points_forU[i].x && points_forR[j].y == points_forU[i].y)
					u = points_forR[j].z;

			}

			double d = depthIm.at<float>(points_forU[i].y, points_forU[i].x);



			if (rad) {
				r = calc_radius(d, u, concave);
			}
			else {
				r = d;
			}

			double rr = std::round(r);
			std::cout << rr << endl;


			if (rr < lowerLimit && rr >= lowerLimit - graceValue)
				rr = std::round(lowerLimit + 2 * (lowerLimit - rr) / graceValue);
			if (rr > upperLimit && rr <= upperLimit + graceValue)
				rr = std::round(upperLimit - 2 * (rr - upperLimit) / graceValue);



			sumR = sumR + rr;


				
			vec.push_back(rr);
			extract_file << rr;


			//if (judgeByEC_points != points_forU.size()) //27 january 2025
			//{
			//	if (i <= (points_forU.size() - judgeByEC_points) - 1)
			//	{
			//		int aboltabol = 0;
			//	}
			//	else
			//	{
			//		extract_file << r;
			//		decision = checking_R(r, decision, lowerLimit, upperLimit);
			//	}
			//}
			//else
			//{
			//	extract_file << r;
			//	decision = checking_R(r, decision, lowerLimit, upperLimit);
			//}
			


			if (i != points_forU.size() - 1) 
			{
				extract_file << ",";
			}

			decision = checking_R(rr, decision, lowerLimit, upperLimit);

		}
	}
	else //21-Agust-2024
	{
		for (int i = 0; i < points_forU.size(); i++)//27-january-2025
		{
			u = points_forU[i].z;

			double d = depthIm.at<float>(points_forU[i].y, points_forU[i].x);

			if (rad) {
				r = calc_radius(d, u, concave);
			}
			else {
				r = d;
			}

			double rr = std::round(r);
			std::cout << rr << endl;

			//std::srand(std::time(0));
			//int random_number = std::rand() % 10;
			//random_number = 2 + random_number * 3;

			if (rr < lowerLimit && rr >= lowerLimit - graceValue)
				rr = std::round(lowerLimit + 2 * (lowerLimit - rr) / graceValue);
			if (rr > upperLimit && rr <= upperLimit + graceValue)
				rr = std::round(upperLimit - 2 * (rr - upperLimit) / graceValue);


			sumR = sumR + rr;


			vec.push_back(rr);

			//if (judgeByEC_points != points_forU.size()) //27 january 2025
			//{
			//	if (i <= (points_forU.size() - points_er_number) - 1)
			//	{
			//		int aboltabol = 0;
			//	}
			//	else
			//	{
			//		extract_file << r;
			//		//decision = checking_R(r, decision, lowerLimit, upperLimit)
			//	}
			//}
			//else
			//{
			//	extract_file << r;
			//	//decision = checking_R(r, decision, lowerLimit, upperLimit);
			//}

			extract_file << rr;

			if (i != points_forU.size() - 1) 
			{
				extract_file << ",";
			}

			decision = checking_R(rr, decision, lowerLimit, upperLimit);

		}
	}


	avgR = sumR / vec.size();

	avgECR = average_checkingR;

	extract_file << "," << avgECR<< "," <<avgR;

	switch(decision)
	{
	case 1:
		extract_file << "," <<"SP";
		break;
	case 2:
		extract_file << "," << "NOR";
		break;
	case 3:
		extract_file << "," <<"NG";
		break;
	}

	extract_file << endl;
	extract_file.close();

#elif 0
	for (const auto& point : points) {
		double px = point.x;
		double py = point.y;
		auto it = std::lower_bound(points_forR.begin(), points_forR.end(), point, [](const cv::Point3d& left, const cv::Point3d& right) { return left.x < right.x || left.y < right.y; });
		if (it != points_forR.end())
		{
			const auto& point_forR = points_forR[std::distance(points_forR.begin(), it)];
			//std::cout << px << "," << py << "," << point_forR.x << "," << point_forR.y << std::endl;
			if (point_forR.x == point.x && point_forR.y == point.y)
			{
				u = point_forR.z;
			}
		}
		double r = calc_radius(average_near_point(inputImage, MASKIM, point.x, point.y, pointRadius, need_avg), u);
		//int rr = std::round(r);
		std::cout << r << endl;
		vec.push_back(r);

		extract_file << point.x << "," << point.y << "," << r << endl;
	}
#else

	for (const auto& point : points) {
		double u = getZValue(point, points_forR);
		if (u != -1) {
			double r = calc_radius(average_near_point(inputImage, MASKIM, point.x, point.y, pointRadius, need_avg), u);
			std::cout << r << std::endl;
			vec.push_back(r);
			extract_file << point.x << "," << point.y << "," << r << std::endl;
		}
	}
#endif
	

	return true;
}

bool munia_function_class::getVecFromMat(cv::Mat &pcIm, cv::Mat &depthIm, vector<cv::Point3f> &data, bool radius) {
	int rows = depthIm.rows;
	int cols = depthIm.cols;
	cv::Mat dst = cv::Mat::zeros(depthIm.size(), CV_64FC1);

	double u = 0;
	int r = 0;

	data.reserve(rows * cols);

	if (radius) {
		for (int x = 0; x < rows; x++) {
			for (int y = 0; y < cols; y++) {
				double d = depthIm.at<float>(x, y);
				if (d)
				{
					for (int j = 0; j < points_forR.size(); j++) {
						if (points_forR[j].x == y && points_forR[j].y == x)
							u = points_forR[j].z;
					}
					r = std::round(calc_radius(d, u, concave));
					cv::Vec4f v = pcIm.at<cv::Vec4f>(x, y);
					//data.push_back(cv::Point3f(v.val[0], v.val[1], r));
					data.push_back(cv::Point3f(x, y, r));

					dst.at<double>(x, y) = r;

				}

			}


		}

	}
	else {
		for (int x = 0; x < rows; x++) {
			for (int y = 0; y < cols; y++) {

				if (depthIm.at<float>(x, y))
				{
					cv::Vec4f v = pcIm.at<cv::Vec4f>(x, y);
					if (v.val[2] >600 && v.val[2] < 700) {
						//data.push_back(cv::Point3f(v.val[0], v.val[1], v.val[2]));
						data.push_back(cv::Point3f(x, y, v.val[2]));
					}

				}

			}

		}
	}


	std::ofstream extract_file;


	std::string str = workingFolder+"Munia_Data\\3Dpoints\\";


	if (radius) {
		extract_file.open(str+"3dRadius.csv");
		for (int i = 0; i < data.size(); i++) {
			extract_file << data[i].x << "," << data[i].y << "," << data[i].z << endl;
			
		}
		cv::imwrite(str + "RadiusIm.jpg", dst);

	}
	else {
		extract_file.open(str+"3dDepth.csv");
		for (int i = 0; i < data.size(); i++) {
			extract_file << data[i].x << "," << data[i].y << "," << data[i].z << endl;
			
		}

	}


	
	return true;
}




double munia_function_class::average_near_point(cv::Mat inputIm,  cv::Mat inputMask, int x, int y, int circle_radius, bool needAverage)
{
	double a=0;

	if(!needAverage)
	{
		a = inputIm.at<float>(x, y);
		return a;
	}

	//inputMask.convertTo(inputMask, CV_8U);

	float value = inputMask.at<float>(x, y);
	if (value) {
		int channel = inputIm.channels();
		cv::Mat maskIm = cv::Mat::zeros(inputIm.rows, inputIm.cols, CV_8UC1);
		cv::circle(maskIm, cv::Point(x, y), circle_radius, cv::Scalar(255), cv::FILLED);
		//	cv::imshow("a", maskIm);
		//cv::waitKey(1);
		cv::Mat resultIm, resultIm32fc1;
		inputIm.copyTo(resultIm, maskIm);

		if (resultIm.type() == CV_32FC3)
			pc_to_depth_Im(resultIm, resultIm32fc1);
		else
			resultIm.copyTo(resultIm32fc1);


		cv::Mat resultIm8uc1;
		resultIm32fc1.convertTo(resultIm8uc1, CV_8UC1);
		cv::Mat resultMaskIm8uc1;
		cv::threshold(resultIm8uc1, resultMaskIm8uc1, 0.01, 1, cv::THRESH_BINARY);



		//im_binary(resultIm8uc1, resultMaskIm8uc1);
		//cv::Mat im;
		//resultMaskIm8uc1.copyTo(im);
		//im_show(im);
		//double b = im_mean(inputIm, resultMaskIm8uc1);



		cv::Scalar m = cv::mean(inputIm, resultMaskIm8uc1);
		if (channel > 1) {
			a = m.val[2];
			return a;
		}
		a = m.val[0];

		return a;
	}
	return a;
}

bool munia_function_class::make_U_Im(cv::Mat depthIm)
{

	cv::Mat dst=cv::Mat::zeros(depthIm.size(), CV_64FC1);
	for (int dx = 0; dx < depthIm.rows; dx++)
	{
		for (int dy = 0; dy < depthIm.cols; dy++)
		{
			double d = depthIm.at<float>(dx, dy);

				if (d) 
				{
					double u = calc_u(radius, d, concave);
					if (u != 0) 
					{
						dst.at<double>(dx, dy) = u;
					}
				}
			
		}
	}
	cv::imshow("U_Im", dst);
	cv::waitKey(-1);
	std::string imWritingPath = workingFolder + "Munia_data\\U\\Pics\\U_image2.jpg";
	cv::imwrite(imWritingPath, dst);
	return true;
}

double munia_function_class::calc_u(double radius, double d, int concave)
{


	double u=0;
	double R_d=0;
	double B=0;

	switch (concave)
	{
	case 1:
		R_d = radius + d;
		B = sqrt(4 * R_d * R_d - 8 * radius * d);
		u = ((2 * R_d) - B) / 4;
		break;
	case 2:
		R_d = radius - d;
		B = sqrt(4 * R_d * R_d + 8 * radius * d);
		u = ((-2 * R_d) + B) / 4;
		break;
	case 3:
		u = d / 2;
		break;
	}
	return u;
}

double munia_function_class::calc_radius(double d, double u, int concave)
{


	double radius=0;
	double v=0;

	switch (concave)
	{
	case 1:
		v = d - u;
		radius = (2 * u * v) / (-u + v);
		break;
	case 2:
		v = d - u;
		radius = (2 * u * v) / (u - v);
		break;
	case 3:
		radius = d;
		break;
	}
	return radius;
}

int munia_function_class::checkingResults(std::vector<double>res, cv::Mat &resultPlot, std::vector<int>&l_indx, std::vector<int>&u_indx, bool right, std::string strUser)
{

	int decision=1;

	int ist=0, ien = 0;

	if (judge_by_ec_only) 
	{
		ist = ec_start - 1;
		ien = ec_end;
	}
	else
	{
		ist = 0;
		ien = res.size();
	}
	
	if (itemIndex == 2)
	{
		for (int i = ist; i < ien; i++)
		{
			if (lowerLimit > res[i])
			{
				l_indx.push_back(i + 1 - ist);
				decision = 3;
			}
			if (upperLimit < res[i])
			{
				u_indx.push_back(i + 1 - ist);
				//if(decision !=3)
				decision = 3;
			}
			if (lowerLimit <= res[i] && upperLimit >= res[i])
			{
				if (decision != 3)
					decision = 1;
			}
		}

		goto A;
	}


	for(int i=ist; i<ien;i++)
	{
		if (lowerLimit > res[i])
		{
			l_indx.push_back(i+1-ist);
			decision = 0;
		}
		if(upperLimit <res[i])
		{
			u_indx.push_back(i+1-ist);
			//if(decision !=3)
				decision = 0;
		}
		if(lowerLimit <=res[i] && upperLimit >=res[i])
		{
			if(decision!=0)
				decision = 1;
		}
	}

	float sum = 0;
	float average = 0;
	int coun = 0;
	for (int i = ec_start-1; i < ec_end; i++) {
		sum = sum + res[i];
		coun++;

		average = sum / coun;
	}
	
	if (decision != 1) {

		if (lowerLimit <= average && upperLimit2 >= average)
		{

			decision = 2;
		}
	}

	if (decision != 1)
	{
		if (decision != 2)
		{
			decision = 3;
		}
	}


A:

	std::string date_userWanted, time, date_and_time;
	date_userWanted = get_current_date_and_time(1, userWantedTimeSetting);
	time = get_current_date_and_time(2);
	date_and_time = get_current_date_and_time(3);



	std::string item;
	switch (itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}



	std::string dirName = workingFolder+"Munia_Data\\R\\Plots"/*+item+"_" + date_userWanted*/;
	char* dir = (char*)dirName.data();
	_mkdir(dir);

	dirName = workingFolder + "Munia_Data\\R\\Plots\\" + item + "_" + date_userWanted;
	char* dir1 = (char*)dirName.data();
	_mkdir(dir1);

	cv::Mat cvplot=plotGraph(lowerLimit, upperLimit, res, l_indx, u_indx, right, decision, strUser);

	std::string str;
	if (right)
		str = dirName + "\\" +item+"_"+"RH-" + std::to_string(n)+".jpg";
	else
		str = dirName + "\\" + item + "_" + "LH-" + std::to_string(m) + ".jpg";

	cv::imwrite(str, cvplot);

	cvplot.copyTo(resultPlot);
	

	return decision;
}


int munia_function_class::checking_R(double res, int decision, int lower_Limit, int upper_Limit)
{
	
	count_checkingR++;

	if (count_checkingR >= ec_start && count_checkingR <= ec_end)
	{
		sum_checkingR = sum_checkingR + res;
		coun_checkingR++;

		average_checkingR = sum_checkingR / coun_checkingR;


	}

	if (judge_by_ec_only)
	{
		if (count_checkingR >= ec_start && count_checkingR <= ec_end)
		{
			goto A;
		}
		else
			goto B;
	}

A:

	if(itemIndex==2)
	{

		if (lower_Limit > res)
		{
			decision = 3;
		}
		else if (upper_Limit < res)
		{
			decision = 3;
		}
		else
		{
			if (decision != 3)
				decision = 1;
		}

		goto B;
	}


	if (lower_Limit > res)
	{
		decision = 0;
	}
	else if (upper_Limit < res)
	{
			decision = 0;
	}
	else 
	{
		if (decision != 0)
			decision = 1;
	}
	
	/*if (lower_Limit <= res && upper_Limit >= res)
	{
		if (decision != 4)
			decision = 1;
	}*/

	

	

	if(coun_checkingR==ec_end-ec_start+1)
		if (lowerLimit <= average_checkingR && upperLimit2 >= average_checkingR)
		{
			if(decision!=1)
				decision = 2;
		}
		else { decision = 3; }

B:


	return decision;
}


cv::Mat munia_function_class::plotGraph(int ll, int ul, std::vector<double>resy, std::vector<int>l_index, std::vector<int>u_index, bool right, int decision, std::string strUser)
{
	cv::Mat cvplot;

	std::vector<int>x, resY;
	std::vector<double>llY, ulY, ulY2, mY;

	std::vector<cv::Point2d>l_points, u_points;
	cv::Point2d l_p(0.0, 0.0), u_p(0.0, 0.0);

	float ul2 = upperLimit2;

	if (judge_by_ec_only)
	{
		for (int i = 6; i < resy.size(); i++)
		{
			resY.push_back(resy[i]);
		}
	}
	else
	{
		for (int i = 0; i < resy.size(); i++)
		{
			resY.push_back(resy[i]);
		}
	}

	for (int i = 0; i < resY.size(); i++)
	{

		x.push_back(i + 1);
		llY.push_back(ll);
		ulY.push_back(ul);
		ulY2.push_back(ul2);
		mY.push_back((ll + ul) / 2);

		l_p.x = i + 1;
		u_p.x = i + 1;


		for (int j = 0; j < l_index.size(); j++) {
			if (l_index[j] == i + 1)
			{
				l_p.y = resY[i];
				break;
			}
			else
				l_p.y = 0;
		}
		for (int j = 0; j < u_index.size(); j++) {
			if (u_index[j] == i + 1)
			{
				u_p.y = resY[i];
				break;
			}
			else
				u_p.y = 0;
		}

		l_points.push_back(l_p);
		u_points.push_back(u_p);

	}


	std::string item;
	switch (itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}


	std::string str;

	if (right)
	{
		if (strUser == "")
			str = item+"_RH-" + std::to_string(n);
		else
			str = "(" + strUser + ")"+ item+"_RH-" + std::to_string(n);
	}
	else
	{
		if (strUser == "")
			str = item+"_LH-" + std::to_string(m);
		else
			str = "(" + strUser + ")"+item+ "_LH-" + std::to_string(m);
	}


	auto axes = CvPlot::makePlotAxes();
	axes.create<Title>(str);
	axes.xLabel("Points");
	axes.yLabel("Radius");


	float dif_llul = (ul2 - ll)*0.35;
	axes.setXLim({ 0, resY.size() + 1 });
	axes.setYLim({ ll - dif_llul, ul2 + dif_llul });

	axes.create<CvPlot::Series>(x, llY, "-r");

	axes.create<CvPlot::Series>(x, ulY)
		.setColor(cv::Scalar(0,0, 255));

	axes.create<CvPlot::Series>(x, ulY2)
		.setColor(cv::Scalar(0, 0, 255));


	axes.create<CvPlot::Series>(x, mY)
		.setLineType(LineType::Solid)
		.setColor(cv::Scalar(0, 210, 0));

	axes.create<CvPlot::Series>(x, resY)
		.setLineType(LineType::Solid)
		.setMarkerType(MarkerType::Circle)
		.setMarkerSize(3)
		.setColor(cv::Scalar(255, 0, 0));

	axes.create<Series>(l_points, "-ok")
		.setLineType(LineType::None)
		.setMarkerType(MarkerType::Circle)
		.setMarkerSize(10)
		.setColor(cv::Scalar(0, 0, 255));

	axes.create<Series>(u_points, "-ok")
		.setLineType(LineType::None)
		.setMarkerType(MarkerType::Circle)
		.setMarkerSize(10)
		.setColor(cv::Scalar(0, 0, 255));

	cvplot = axes.render(300, 500);
	std::string text;
	cv::Scalar color; 
	switch (decision)
	{
	case 1:
		text = "SP";
		color= cv::Scalar(0, 255, 0);
		break;
	case 2:
		text = "NOR";
		color = cv::Scalar(255, 255, 0);
		break;
	case 3:
		text = "NG";
		color = cv::Scalar(0, 0, 255);
		break;
	}

	cv::Point org(85, 78);
	int fontFace = cv::FONT_HERSHEY_SIMPLEX;
	double fontScale = 1.4;
	cv::putText(cvplot, text, org, fontFace, fontScale, color,5);

	std::string text1 = "This graph is made by MUNIA";
	cv::Point org1(335, 290);
	int fontFace1 = 0;
	double fontScale1 = 0.3;
	cv::Scalar color1(0, 0, 0);
	cv::putText(cvplot, text1, org1, fontFace1, fontScale1, color1);

	std::string text2 = "Invented by ISS Inc., Japan";
	cv::Point org2(12, 290);
	int fontFace2 = 0;
	double fontScale2 = 0.3;
	cv::Scalar color2(0, 0, 0);
	cv::putText(cvplot, text2, org2, fontFace2, fontScale2, color2);

	return cvplot;
}


double munia_function_class::derivative_R(double d, double u, int concave)
{


	double derivative_R=0;

	switch (concave)
	{
	case 1:
		derivative_R = 2*(d*d-2*u*d+2*u*u )/ ((d - 2 * u)*(d - 2 * u));
		break;
	case 2:
		derivative_R = 1;
		break;
	case 3:
		derivative_R = 2;
		break;
	}
	return derivative_R;
}

cv::Point3d munia_function_class::optimum_U(std::vector<double>&obs_r, std::vector<double>&opt_r, std::vector<double>&d)
{

	srand(time(NULL));
	double u = (rand() % 1000) / 100.0;
	u = 235;

	double alpha = 0.001;
	double epsilon = 0.00006;
	double new_r=0;

	int numIteration = 50000;
	int d_size = obs_r.size();
	double r=0;
	double ue = 0;
	int it;
	for( it=0; numIteration;it++){
		double d_R = 0;
		ue = 0;
		for (int i = 0; i < d_size; i++) {
			double ue0=(u - calc_u(obs_r[i], d[i], concave));
			
			d_R =d_R + derivative_R(d[i], u)*ue0 ;
			ue = ue + abs(ue0);
		
		

		}
		ue = ue / d_size;
		u = u-alpha * d_R;

		new_r = 0;
		for (int i = 0; i < d_size; i++) {
			new_r = new_r+pow(obs_r[i] - calc_radius(d[i], u, concave), 2);
		}


		if (abs(new_r-r) < epsilon) {
			break;
		}
		r = new_r;
	

	}



	for (int i = 0; i < d_size; i++) {
		opt_r.push_back(calc_radius(d[i], u, concave));
	

	}


	cv::Point3d point = {u, new_r, (double)it};



	return point;
}


bool munia_function_class::avg_csvTo_ponitVec(std::vector<cv::Point3d> &for_R_data, std::string str){
	
int i = 0;
std:string str1 = str + std::to_string(i) + ".csv";

	while (1) {
		std::ifstream file(str1);
		if (file.is_open())
		{
			i++;
			str1 = str + std::to_string(i) + ".csv";
		}
		else {
			break;
		}

	}

	vector<vector<cv::Point3d>> forRdata;
	for (int j = 0; j < i; j++) {
		vector<cv::Point3d>point_vec;
		mamun_util::pointsCsv2pointsVector(str + std::to_string(j) + ".csv", point_vec);
		forRdata.push_back(point_vec);

	}

	//for_R_data.resize(forRdata[0].size());

	for (int i = 0; i < forRdata[0].size(); i++) {
		for_R_data.push_back(forRdata[0][i]);
	

	}


	for (int m = 0; m < forRdata[0].size(); m++) {
		double sum = 0;
		for (int n = 0; n < i; n++) {
			cv::Point3d p = forRdata[n][m];
			sum = sum + p.z;
		}
		cv::Point3d p1;
		p1.z = sum / i;

		for_R_data[m].z = p1.z;

	

	}

	return true;
}


bool munia_function_class::combined_Ufactor(std::string strL, std::string strR) {
	std::vector<cv::Point3d> for_R_dataL;
	std::vector<cv::Point3d> for_R_dataR;
	std::vector<cv::Point3d> for_R_data;

	avg_csvTo_ponitVec(for_R_dataL, strL);
	avg_csvTo_ponitVec(for_R_dataR, strR);

	//for_R_data.resize(for_R_dataR.size());
	int lowerLimit = 200, upperLimit = 300;

	int size = for_R_dataR.size();

	for (int i = 0; i < size; i++) {
		for_R_data.push_back(for_R_dataR[i]);


	}

	for (int j = 0; j < size; j++) {
		if (for_R_dataR[j].z > lowerLimit && for_R_dataR[j].z<upperLimit && for_R_dataL[j].z>lowerLimit && for_R_dataL[j].z < upperLimit)
		{
			for_R_data[j].z = (for_R_dataL[j].z + for_R_dataR[j].z) / 2;
		}
		else if(for_R_dataL[j].z>lowerLimit && for_R_dataL[j].z < upperLimit)
		{
			for_R_data[j].z = for_R_dataL[j].z;

		}
		else {
			for_R_data[j].z = 0;
		}


	}

	

	/*bool addNew = 1;
	for (int i = 0; i < for_R_dataR.size(); i++) {

		int count = 0;

		for (int j = 0; j < for_R_dataL.size(); j++) {
			if (for_R_dataR[i].z> lowerLimit && for_R_dataR[i].z<upperLimit && for_R_dataL[i].z>lowerLimit && for_R_dataL[i].z < upperLimit)
			{

				for_R_data[i].z = (for_R_dataL[j].z + for_R_dataR[i].z) / 2;
				addNew = 0;
				break;
			}
			count = j;
		}
		if (addNew) {
			for_R_data.push_back(for_R_dataL[count]);
			addNew = 1;
		}
	}*/



	for (auto it = for_R_data.begin(); it != for_R_data.end();)
	{
		if (it->z == 0)
			it = for_R_data.erase(it);
		else
			++it;
		

	}
	std::ofstream extract_file;

	std::string str = workingFolder+"Munia_Data\\U\\";

	std::string fileName = str + "forRdata.csv";
	extract_file.open(fileName);
	for (int i = 0; i < for_R_data.size(); i++) {
		extract_file << for_R_data[i].x << "," << for_R_data[i].y << "," << for_R_data[i].z << endl;
		

	}

	return true;
}


bool munia_function_class::csvToVectors(std::vector< std::vector<cv::Point3d>> &vectorsVector, std::vector<cv::Point3d> &pointsVector, std::string str_1, std::string str_2) 
{



int ic = 0, jc=0;
std::string str1 = str_1 + std::to_string(ic) + ".csv";
std::string str2 = str_2 + std::to_string(jc) + ".csv";

	while (1) {
		std::ifstream file(str1);
		if (file.is_open())
		{
			ic++;
			str1 = str_1 + std::to_string(ic) + ".csv";
		}
		else {
			break;
		}

		DoEvent();

	}

	while (1) {
		std::ifstream file(str2);
		if (file.is_open())
		{
			jc++;
			str2 = str_2 + std::to_string(jc) + ".csv";
		}
		else {
			break;
		}
	
		DoEvent();

	}

	for (int i = 0; i < ic; i++) {
		vector<cv::Point3d>point_vec;
		mamun_util::pointsCsv2pointsVector(str_1 + std::to_string(i) + ".csv", point_vec);
		vectorsVector.push_back(point_vec);

		DoEvent();

	}

	for (int i = 0; i < jc; i++) {
		vector<cv::Point3d>point_vec;
		mamun_util::pointsCsv2pointsVector(str_2 + std::to_string(i) + ".csv", point_vec);
		vectorsVector.push_back(point_vec);


		DoEvent();


	}


	for (int i = 0; i < vectorsVector[0].size(); i++) {
		pointsVector.push_back(vectorsVector[0][i]);

		DoEvent();

	}


	

	return true;
}

bool munia_function_class::vectorOperations(std::vector<cv::Point3d> &resultVector, std::vector<cv::Point3d> changingVector) 
{

	
	
	std::vector<cv::Point3d> helpingVector;

	int lowerLimit = 200, upperLimit = 300;

	int size = resultVector.size();

	helpingVector.resize(resultVector.size());


	for (int i = 0; i < size; i++) {
		helpingVector[i].z = resultVector[i].z;

		DoEvent();

	}

	for (int j = 0; j < size; j++) {
		if (resultVector[j].z > lowerLimit && resultVector[j].z<upperLimit && changingVector[j].z>lowerLimit && changingVector[j].z < upperLimit)
		{
			helpingVector[j].z = (changingVector[j].z + resultVector[j].z) / 2;
		}
		else if (changingVector[j].z > lowerLimit && changingVector[j].z < upperLimit)
		{
			helpingVector[j].z = changingVector[j].z;

		}
		else {
			helpingVector[j].z = 0;
		}

		DoEvent();

	}

	for (int i = 0; i < size; i++) {
		resultVector[i].z = helpingVector[i].z;

		DoEvent();

	}


	return true;
}

void munia_function_class::DoEvent()
{
	MSG msg;

	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

bool munia_function_class::extract_U_csv(std::vector<cv::Point3d> pointsVector)
{
	for (auto it = pointsVector.begin(); it != pointsVector.end();)
	{
		if (it->z == 0)
			it = pointsVector.erase(it);
		else
			++it;
	
		DoEvent();
	}
	std::ofstream extract_file;

	std::string str = workingFolder+"Munia_Data\\U\\";

	std::string fileName = str + "forRdata.csv";
	extract_file.open(fileName);
	for (int i = 0; i < pointsVector.size(); i++) {
		extract_file << pointsVector[i].x << "," << pointsVector[i].y << "," << pointsVector[i].z << endl;
		
		DoEvent();

	}

	return true;
}