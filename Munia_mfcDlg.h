
// Munia_mfcDlg.h : header file
//

#pragma once
//#define WM_UPDATE_PROGRESS (WM_USER + 1)
#include<iostream>

#include <windows.h>



static bool flag = false;
static bool right = true;
static bool realtime=true;
static bool filename=false;
static bool checksvorecord=false;
static bool check_preview=false;
static bool viewResultsPlot = false;
static bool check_defaultCamera=false;
static bool resolution2K=false;
static bool sensingMode_Fill=false;
static bool check_3dData=false;
static int filterIndex=2;
static int depthModeIndex=1;
static bool checkUserNote = 0;
static bool calibration = 0;
static bool manualPointSelect = 0;



static std::string inputVal= "";
static bool autoCalculation = 1;
static CString working_folder;

static bool isCheckedSettings = 0;

//bool ini_success1=0;

// progressBar dialog
UINT threadFunc(LPVOID pParam);

class progressBar : public CDialog
{
	DECLARE_DYNAMIC(progressBar)

public:
	progressBar(CWnd* pParent = nullptr);   // standard constructor
	virtual ~progressBar();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_progressBar };
#endif
public:
	CProgressCtrl m_progBarCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	bool setProgPos(int stepsize = 10, int pauseTime=100);
	bool setProgPos1(int stepsize = 10, int pauseTime = 100);
	bool setProgPos2(int stepsize = 10, int pauseTime = 100);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

class Ucalibraton : public CDialog
{
	DECLARE_DYNAMIC(Ucalibraton)

private:
	progressBar mProgressBarDlg;
	CString m_str;

public:
	Ucalibraton(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Ucalibraton();

	CEdit *pFilename0;

	CEdit* mEditNcol;
	CEdit* mEditNrow;

	CString edit_ncol;
	CString edit_nrow;


	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Ucalibration };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtontakeU();
	afx_msg void OnBnClickedButtoncombileU();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonchangeUcsv();
	afx_msg void OnBnClickedBoptimumu();
	afx_msg void OnBnClickedButtoncheckoptimizationresults();
	afx_msg void OnBnClickedButtonselectfolder();
	afx_msg void OnBnClickedButtoncameracalibration();
	afx_msg void OnBnClickedCheckmanual();
	afx_msg void OnBnClickedRHd1080();
	afx_msg void OnBnClickedRHd2k();
	afx_msg void OnBnClickedRadioYed();
	afx_msg void OnBnClickedRadioYy8();
};


// inputValue dialog

class inputValue : public CDialog
{
	DECLARE_DYNAMIC(inputValue)


private:
	Ucalibraton mUcalibrationDlg;
public:
	inputValue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~inputValue();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_input };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
public:



	CString input_val;
	afx_msg void OnBnClickedOk();
};


// m_settings dialog

class m_settings : public CDialog
{
	DECLARE_DYNAMIC(m_settings)

public:
	m_settings(CWnd* pParent = nullptr);   // standard constructor
	virtual ~m_settings();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_M_setting };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBilateralFilter();
	afx_msg void OnBnClickedMedianBlur();
	afx_msg void OnBnClickedGaussianBlur();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedOk();
	CString Edit_ksize;
	CString Edit_frame_starting_no;
	CString Edid_ending_frame_no;

	CString lowerlimitradius;
	CString upperlimitradius;
	afx_msg void OnBnClickedRadioYed();
	afx_msg void OnBnClickedRadioYy8();
	afx_msg void OnBnClickedidealpositionmirror();
	afx_msg void OnBnClickedRadio9points();
	afx_msg void OnBnClickedRadio3points();
};




// measurements dialog

class measurements : public CDialog
{
	DECLARE_DYNAMIC(measurements)

public:
	measurements(CWnd* pParent = nullptr);   // standard constructor
	virtual ~measurements();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Measurements };
#endif

private:
	m_settings mSetting_dlg;
	progressBar mProgressBarDlg;
	Ucalibraton mUcalibrationDlg;
	inputValue mInputValueDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonmsettings();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBcalibration();
	afx_msg void OnBnClickedBSelectPoints();
	afx_msg void OnBnClickedButton3ddata();

	CEdit* userNote;
	CString UserNote;
	afx_msg void OnBnClickedCheckusernote();
	afx_msg void OnBnClickedButtoncancel();


	afx_msg void OnBnClickedCheckViewresultsplot();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBCalculationflexipoints();
	afx_msg void OnBnClickedButtonCalculation();
};



// realtimeCameraOpening dialog 

class realtimeCameraOpening : public CDialog
{
	DECLARE_DYNAMIC(realtimeCameraOpening)

public:
	realtimeCameraOpening(CWnd* pParent = nullptr);   // standard constructor
	virtual ~realtimeCameraOpening();



	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Realtime };
#endif

private:
	measurements measurements_dlg;
	progressBar mProgressBarDlg;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	CEdit* mEditGainL;
	CEdit* mEditGainR;
	CEdit* mEditExposureL;
	CEdit* mEditExposureR;

	CStatic *mStCamL, *mStCamR;
	CStatic *mStGain, *mStExposure;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRHd1080();
	afx_msg void OnBnClickedRHd2k();
	afx_msg void OnBnClickedRUltra();
	afx_msg void OnBnClickedRQuality();
	afx_msg void OnBnClickedRPerformance();
	afx_msg void OnBnClickedRLast();
	afx_msg void OnBnClickedRStandard();
	afx_msg void OnBnClickedRFill();
	afx_msg void OnBnClickedCheckCamerasettings();
	CString edit_gainL;
	CString edit_gainR;
	CString edit_exposureL;
	CString edit_exposureR;
	afx_msg void OnBnClickedBCameraOpen();
	afx_msg void OnBnClickedCheckpreview();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBCloseCamera();
	afx_msg void OnBnClickedButtongomeasementdialog();
};



// svoReading dialog

class svoReading : public CDialog
{
	DECLARE_DYNAMIC(svoReading)

public:
	svoReading(CWnd* pParent = nullptr);   // standard constructor
	virtual ~svoReading();

	CEdit *pFilename0;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SVO_reading };
#endif

private:
	measurements measurements_dlg;
	progressBar mProgressBarDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();


	afx_msg void OnBnClickedBenter();
	CString m_str;
	CString m_str1;
	afx_msg void OnBnClickedRStandard();
	afx_msg void OnBnClickedRFill();
	afx_msg void OnBnClickedRUltra();
	afx_msg void OnBnClickedRQuality();
	afx_msg void OnBnClickedRPerformance();
	afx_msg void OnBnClickedRLast();
	afx_msg void OnBnClickedCheckpreview();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtongomeasementdialog();
	afx_msg void OnBnClickedBallsvocalculation();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButtonselectfolder();
};


// svo_Recording dialog

class svo_Recording : public CDialog
{
	DECLARE_DYNAMIC(svo_Recording)

public:
	svo_Recording(CWnd* pParent = nullptr);   // standard constructor
	virtual ~svo_Recording();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SVO_Record };
#endif

	progressBar mProgressBarDlg;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

public:
	CEdit* mEditGainL;
	CEdit* mEditGainR;
	CEdit* mEditExposureL;
	CEdit* mEditExposureR;

	CStatic *mStCamL, *mStCamR;
	CStatic *mStGain, *mStExposure;

	CButton* mSvoRecord;
	CEdit* mSvoPath0;
	CEdit* mSvoPath1;
	CStatic* mStCapno;
	CEdit* mEditCapno;
	CStatic* mStPress_Record_svo;

public:

	int n = 0;

	afx_msg void OnBnClickedRHd1080();
	afx_msg void OnBnClickedRHd2k();
	afx_msg void OnBnClickedCheckCamerasettings();
	CString edit_gainL;
	CString edit_gainR;
	CString edit_exposureL;
	CString edit_exposureR;
	afx_msg void OnBnClickedBRecordsvo();
	CString m_str_svo0;
	CString m_str_svo1;
	CString edit_cap_no;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBopeningrecording();
	afx_msg void OnBnClickedBclosingrecording();
	afx_msg void OnBnClickedButtonselectfolder();
};


// CMuniamfcDlg dialog 

class CMuniamfcDlg : public CDialogEx
{
// Construction
public:
	CMuniamfcDlg(CWnd* pParent = nullptr);	// standard constructor

	realtimeCameraOpening realtime_dlg;
	svoReading svoRead_dlg;
	svo_Recording svoRecord_dlg;


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUNIA_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CStatic *pPmainDlgBG;
	CButton* pBtn;
// Implementation
protected:
	HICON m_hIcon;
	int count = 0;
	int liveBeatflag = 1;
	int countFortimeKilling = 0;
	bool plcFlag = 0;


	

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


	DECLARE_MESSAGE_MAP()
public:
	CRect rect;
	int startingOptionIndex = 1;
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedBrefresh();
	afx_msg void OnBnClickedRealtime();
	afx_msg void OnBnClickedRecorded();
	afx_msg void OnBnClickedRadiosvorecording();
	afx_msg void OnBnClickedButtonstart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckautomatic();
	afx_msg void OnClose();
};

