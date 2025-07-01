/**************************************
 khairuzzaman Mamun
 ***************************************/

#pragma once
#include <opencv2/opencv.hpp>
#include <sl/Camera.hpp>
#include <vector>
#include <mutex>
#include <Windows.h>
#include <cmath>
//#include <MatPlot.h>


using namespace sl;


class munia_function_class
{
public:
	munia_function_class();
	~munia_function_class();

	
	bool opencamera(bool default_cam_setting=1, int custom_GainL = 1, int custom_GainR = 1, int custom_exposerL = 18, int custom_exposerR = 18, int depthModeIndex=1, bool resolution2K=false, bool sensingMode_Fill=false);
	bool opening_recording_svo( bool default_cam_setting = 1, int custom_GainL = 1, int custom_GainR = 1, int custom_exposerL = 18, int custom_exposerR = 18, bool resolution2K = false);
	bool recording_svo(const char* cam0svo, const char* cam1svo, int nImCap=45);
	bool open_svo(const std::vector<std::string>& filenames, int depthModeIndex = 1, bool resolution2K = false, bool sensingMode_Fill = true);
	bool extract_BGRIm(cv::Mat &cv_bgrIm0, cv::Mat &cv_bgrIm1, std::string str="Preview");
	bool take_2_BGRIm(bool fullHD=1);
	//21-Agust-2024
	double checkingMirrorPosition(std::vector<cv::Point3d> points3d, std::vector<cv::Point3d> &tpoints3d, bool right = 1, bool realtime=0);
	//21-Agust-2024
	bool extract_csvDepthPc(bool realtime = false, uint filter_index=2);
	bool getVecFromMat(cv::Mat &pcIm, cv::Mat &depthIm, std::vector<cv::Point3f> &data, bool radius=1);
	bool closecamera();

	bool extract_Radius(cv::Mat &depthIm, std::vector<double> &vec, bool right=true, std::string userNote="Manual", bool rad=1, int itemIndx=1);
	int checkingResults(std::vector<double>res, cv::Mat &resultPlot, std::vector<int>&l_indx, std::vector<int>&u_indx, bool right, std::string strUser="");
	int checking_R(double res, int decision, int lower_Limit, int upper_Limit);
	
	cv::Mat plotGraph(int ll, int ul, std::vector<double>resy, std::vector<int>l_index, std::vector<int>u_index, bool right, int decision = 1, std::string strUser = "");
	bool points_optimum_UforRadius(std::string derec, std::vector<cv::Point3d> &resultVector, std::vector<cv::Point3d>&opU, bool right=1, bool real_time=1, bool sensingMode=0, int depthModeIndex=1, bool resolution2K=0, int filterIndex=1);
	bool change_points_UforRadius(cv::Mat depthIm, std::vector<cv::Point3d> pointsRforU, bool R2U=1);
	bool extract_csv_UforRadius(bool needFn = false, cv::Mat depthIm = cv::Mat{}, bool right=true);
	bool selectPoint(bool needFn = false, bool right = true, bool mouse_click = false, cv::Mat image = cv::Mat(), std::string str="");
	bool combined_Ufactor(std::string strL, std::string strR);
	


	bool csvToVectors(std::vector< std::vector<cv::Point3d>> &vectorsVector, std::vector<cv::Point3d> &pointsVector, std::string str_1, std::string str_2);
	bool vectorOperations(std::vector<cv::Point3d> &resultVector, std::vector<cv::Point3d> changingVector);
	bool extract_U_csv(std::vector<cv::Point3d> pointsVector);

	bool getHRT_matrix(cv::Mat &cam0, cv::Mat &cam1, bool fullHd = 1, int r = 7, int c = 5, bool option = 1);
	bool transformation_matrix(bool resolution2K = false);

	bool imageShow_inMFC(cv::Mat image, std::string winName, float multiplication=1);
	int PLCCom(std::string cmd, std::string* rbuf);
	bool plc_initialization(std::string adr = "192.168.1.20", std::string prt = "8501");
	bool plc_workingFn(bool autoCalculation=1);
	void ShutdownSystem();
	
	void DoEvent();


	

	bool shutdownpc = 0;
	cv::Mat inputDepthImage;
	cv::Mat Pcimage;

	cv::Mat resultPlot;
	cv::Mat imPosi;


	std::vector<cv::Point3d> points_forR;
	std::vector<std::string> filenames;
	char* svo_path[2];
	std::vector<cv::Point3d> points_forU;

	std::vector<cv::Point2f>ideal_triPoints;


	int F_start_pos = 0;
	int F_using_frames_from = 0;
	int F_using_frames_to = 10;
	int ksize = 375;
	float imWindowSizeConPercentage = 1;

	int graceValue = 5; ////////////
	float lowerLimit = 1200, upperLimit = 1260, upperLimit2=1400;

	int itemIndex = 1;
	bool idealPositionOFmirror = 0; //21-Agust-2024

	bool calibrationFromIdealMirror = 0; //21-Agust-2024
	int dif_depthValue = 00;

	bool judge_by_ec_only = 0;   ////////////

	bool right_mirror = 1;
	bool flipping_mirror = 0;

	//std::string workingFolder = "C:\\Users\\kial\\Desktop\\";//C:\\Users\\mamun\\Desktop\\

	std::string workingFolder = "E:\\vs_projects\\munia_versions\\Munia_postIndian_YY8_version\\Munia\\";



private:

	enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };


	bool load_3d_data(cv::Mat& im_s_32fc4, int start_pos = 0, int using_frames_from = 1, int using_frames_to = 10, bool realtime = 0);
	bool combine(const cv::Mat& im_xyz_0_32f, const cv::Mat& im_xyz_1_32f, cv::Mat& im_xyz_32f);
	void extract_combine_xyz(const cv::Mat& im_xyz_0_32f, const cv::Mat& im_xyz_1_32f, const cv::Rect& roi, cv::Mat& im_xyz_32f/*, cv::Mat& im_area_8u*/);
	bool excludeNoisePart(cv::Mat &im_xyz_32f);
	bool avg_csvTo_ponitVec(std::vector<cv::Point3d> &for_R_data, std::string str);
	//.....
	bool findTriPoints(cv::Mat image, std::vector<cv::Point2f> &triPoints);
	//.........

	double average_near_point(cv::Mat inputIm, cv::Mat inputMask, int x, int y, int circle_radius, bool needAverage = true);
	bool make_U_Im(cv::Mat depthIm);
	double calc_u(double radius, double d, int concave=1);
	double calc_radius(double d, double u, int concave=1);
	double derivative_R(double d, double u, int concave = 1);
	cv::Point3d optimum_U(std::vector<double>&obs_r, std::vector<double>&opt_r, std::vector<double>&d);

	bool camera_init_parameters(int depthModeIndex = 1, bool resolution2K = false);
	bool camera_runtime_parameters(bool sensingMode_Fill = true);
	bool camera_setting(bool needfn = 0, bool deFault = 1, int custom_GainL = 1, int custom_GainR = 1, int custom_exposerL = 6, int custom_exposerR = 6);


	bool loadMatrix_XML(std::string file, cv::Mat& m1, cv::Mat& m2, cv::Mat& m3, cv::Mat& m4, std::string sm1 = "", std::string sm2 = "", std::string sm3 = "", std::string sm4 = "");
	bool autoCornerGet(cv::Mat &im, std::vector<cv::Point2f> &imagePoints, bool first = 1, int x = 7, int y = 5);
	bool calcChessboardCorners(cv::Size boardSize, float squareSize, std::vector<cv::Point3f>& corners, Pattern patternType = CHESSBOARD);
	bool extract_cornarPoints(cv::Mat &image, std::vector<cv::Point2f> &points, int np);


	sl::Camera zedL, zedR;
	int nb_detected_zed;
	sl::RuntimeParameters runtime_parameters;
	sl::InitParameters init_parametersL, init_parametersR;
	sl::PositionalTrackingParameters tracking_parameters;
	sl::Resolution F_resolution;
	
	int counter = 0;
	int liveBeatflag = 0;
	int count = 0;
	int liveBeatflag1 = 1;
	int lower_bound = 550;
	int upper_bound = 750;
	int concave = 1;

	int pointRadius = 3;
	bool need_avg=0;
	double radius = 1230;

	int ec_start = 7;
	int ec_end = 9;

	int beg_point_indx = 0;
	int end_point_indx = 9;

	float userWantedTimeSetting = -7;

	int count_checkingR = 0;

	double sum_checkingR = 0;
	double average_checkingR = 0;
	int coun_checkingR = 0;


	cv::Rect F_mirror_roi = cv::Rect{};
	cv::Mat rotation_matrix = cv::Mat(3, 3, CV_32F, cv::Scalar(0));
	cv::Mat homography_matrix = cv::Mat(3, 3, CV_32F, cv::Scalar(0));
	cv::Mat translation = cv::Mat(3, 1, CV_32F, cv::Scalar(0));
	cv::Mat R = cv::Mat(3, 4, CV_32F, cv::Scalar(0));

	int n = 0, m=0, rlR=0, rlL=0;
	bool diferentExecution = true;
	int i = 0, j=0;

	int dst_socket;

};