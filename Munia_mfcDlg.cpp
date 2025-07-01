
// Munia_mfcDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Munia_mfc.h"
#include "Munia_mfcDlg.h"
#include "afxdialogex.h"
#include <afxwin.h>
#include <sstream>
#include "mamun_util.h"
#include <string>
#include <opencv2/opencv.hpp>
#include <afxdlgs.h>
#include <windows.h>
#include <direct.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDT_TIMER 100
#define WM_MY_MESSAGE WM_USER + 100

//#define _T(x) L ## x
//#define UNICODE
using namespace mamun_util;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMuniamfcDlg dialog



CMuniamfcDlg::CMuniamfcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MUNIA_MFC_DIALOG, pParent)
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMuniamfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


}

BEGIN_MESSAGE_MAP(CMuniamfcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_Exit, &CMuniamfcDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_B_refresh, &CMuniamfcDlg::OnBnClickedBrefresh)
	ON_BN_CLICKED(IDC_Realtime, &CMuniamfcDlg::OnBnClickedRealtime)
	ON_BN_CLICKED(IDC_Recorded, &CMuniamfcDlg::OnBnClickedRecorded)
	ON_BN_CLICKED(IDC_RADIO_svo_recording, &CMuniamfcDlg::OnBnClickedRadiosvorecording)
	ON_BN_CLICKED(IDC_BUTTON_start, &CMuniamfcDlg::OnBnClickedButtonstart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_automatic, &CMuniamfcDlg::OnBnClickedCheckautomatic)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMuniamfcDlg message handlers

BOOL CMuniamfcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	working_folder = CString(theApp.m().workingFolder.c_str());

	// Add "About..." menu item to system menu.

	pPmainDlgBG = (CStatic*)GetDlgItem(IDC_STATIC_mainDlgBG);
	HBITMAP entryIm = (HBITMAP)LoadImage(NULL, L"res\\BackgroundIm.bmp", IMAGE_BITMAP, 360, 320, LR_LOADFROMFILE);
	pPmainDlgBG->SetBitmap(entryIm);

	CheckRadioButton(IDC_Realtime, IDC_Recorded, IDC_Realtime);

	pBtn = (CButton*)GetDlgItem(IDC_CHECK_automatic);


	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	
	
	bool cameraOpen=theApp.m().opencamera(0,1,1,15,15, 1,false,1);

	if(cameraOpen)
		flag = 1;

	bool ini_success = theApp.m().plc_initialization();

	if (cameraOpen && ini_success)
	{
		pBtn->SetCheck(1);
		SetTimer(4, 100, 0);
		plcFlag = 1;
	}
	else
	{
		if (!cameraOpen && !ini_success) 
		{
			while (1) {
				int result = MessageBoxW(_T("Retry camera openning and PLC initialization?"), _T("Notice"), MB_YESNO);

				if (result == IDYES)
				{
					cameraOpen = theApp.m().opencamera(0, 1, 1, 15, 15, 1, false, 1);
					ini_success = theApp.m().plc_initialization();

					if (cameraOpen && ini_success)
					{
						pBtn->SetCheck(1);
						flag = 1;
						SetTimer(4, 100, 0);
						plcFlag = 1;
						break;
					}

				}
				else {
					MessageBox(_T("PLC initialization and openning cameras are failed. \n You can work with recording svo file only."), _T("Info"), MB_ICONINFORMATION);
					break;
				}
			}
		}
		else
		{
			
			if (!cameraOpen)
			{
				flag = 0;

				while (1) {
				int result = MessageBoxW(_T("Retry camera openning?"), _T("Notice"), MB_YESNO);

				if (result == IDYES)
				{
					cameraOpen = theApp.m().opencamera(0, 1, 1, 15, 15, 1, false, 1);
					if (cameraOpen)
					{
						pBtn->SetCheck(1);
						flag = 1;
						SetTimer(4, 100, 0);
						plcFlag = 1;
						break;
					}
				}
				else {
					MessageBox(_T("Openning cameras are failed. \n You can work with recording svo file only."), _T("Info"), MB_ICONINFORMATION);
					break;
				}
			}
		}

			
			if(!ini_success)
			{
				while (1)
				{
				int result = MessageBoxW(_T("Retry PLC initialization?"), _T("Notice"), MB_YESNO);

				if (result == IDYES)
				{
					ini_success = theApp.m().plc_initialization();
					if (ini_success)
					{
						pBtn->SetCheck(1);
						SetTimer(4, 100, 0);
						plcFlag = 1;
						break;
					}
				}
				else {
					MessageBox(_T("PLC initialization is failed. \n You can work manually. \n For automatic working please connect PLC first."), _T("Info"), MB_ICONINFORMATION);
					break;
				}
			}
		}
		}

	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CMuniamfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMuniamfcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMuniamfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CMuniamfcDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Create a brush with the background color
	static CBrush brush(RGB(102, 153, 204)); // white color
	// Return the brush

	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		// Set the background mode to transparent
		pDC->SetBkMode(TRANSPARENT);
	}


	////buttons

	if (pWnd->GetDlgCtrlID() == IDC_BUTTON_start) {
		static CBrush brush(RGB(0, 255, 0)); // white color
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == ID_Exit) {
		static CBrush brush(RGB(255, 0, 0)); // white color
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_B_refresh) {
		static CBrush brush(RGB(10, 0, 255)); // white color
		return (HBRUSH)brush.GetSafeHandle();
	}


	return (HBRUSH)brush.GetSafeHandle();
}


/////////////////////..............................................................................................................................................................

void CMuniamfcDlg::OnBnClickedExit()
{

	int result = MessageBoxW(_T("Are you sure to exit?"),_T("Notice"), MB_YESNO);

	if (result == IDYES)
	{
		autoCalculation = 0;

		if (flag) {
			theApp.m().closecamera();
			flag = 0;
		}
		

		if (plcFlag)
		{
			countFortimeKilling = 0;

			while (1) {

				MSG msg;
				if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}

				if (countFortimeKilling ==1) {
					break;
				}
			}
		}

		EndDialog(ID_Exit);/*OnCancel();*//*OnOK()*/

	}
}

void CMuniamfcDlg::OnBnClickedBrefresh()
{
	autoCalculation = 0;


	if (flag) {
		theApp.m().closecamera();
		flag = 0;
	}
	
	if (plcFlag)
	{
		countFortimeKilling = 0;

		while (1) {
			MSG msg;
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			if (countFortimeKilling == 1) {break;}
		}
	}
	
	//WinExec("x64\\Release\\Munia_mfc.exe", SW_SHOW);
	WinExec("Munia_mfc.exe", SW_SHOW);
	
	PostQuitMessage(0);
}

void CMuniamfcDlg::OnBnClickedRealtime()
{
	if (autoCalculation) {
		autoCalculation = 0;
		pBtn->SetCheck(0);
	}

	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
	realtime = true;
	startingOptionIndex = 1;

	if (svoRead_dlg.m_hWnd != NULL)
		svoRead_dlg.DestroyWindow();

	if (svoRecord_dlg.m_hWnd != NULL)
		svoRecord_dlg.DestroyWindow();
}


void CMuniamfcDlg::OnBnClickedRecorded()
{


	if (autoCalculation) {
		autoCalculation = 0;
		pBtn->SetCheck(0);
	}

	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
	realtime = false;

	startingOptionIndex = 2;

	if (realtime_dlg.m_hWnd != NULL)
		realtime_dlg.DestroyWindow();

	if (svoRecord_dlg.m_hWnd != NULL)
		svoRecord_dlg.DestroyWindow();

}


void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}






// realtimeCameraOpening dialog

IMPLEMENT_DYNAMIC(realtimeCameraOpening, CDialog)

realtimeCameraOpening::realtimeCameraOpening(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_Realtime, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

realtimeCameraOpening::~realtimeCameraOpening()
{
}

void realtimeCameraOpening::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

#if 0
	DDX_Text(pDX, IDC_E_Gain0, edit_gainL);
	DDX_Text(pDX, IDC_E_Gain1, edit_gainR);
	DDX_Text(pDX, IDC_E_Exposure0, edit_exposureL);
	DDX_Text(pDX, IDC_E_Exposure1, edit_exposureR);
#else
	GetDlgItemText(IDC_E_Gain0, edit_gainL);
	GetDlgItemText(IDC_E_Gain1, edit_gainR);
	GetDlgItemText(IDC_E_Exposure0, edit_exposureL);
	GetDlgItemText(IDC_E_Exposure1, edit_exposureR);
#endif
}
BOOL realtimeCameraOpening::OnInitDialog()
{
	CDialog::OnInitDialog();




	mEditGainL = (CEdit*)GetDlgItem(IDC_E_Gain0);
	mEditGainR = (CEdit*)GetDlgItem(IDC_E_Gain1);
	mEditExposureL = (CEdit*)GetDlgItem(IDC_E_Exposure0);
	mEditExposureR = (CEdit*)GetDlgItem(IDC_E_Exposure1);

	mEditGainL->SetWindowTextW(_T("1"));
	mEditGainR->SetWindowTextW(_T("1"));
	mEditExposureL->SetWindowTextW(_T("18"));
	mEditExposureR->SetWindowTextW(_T("18"));


	mStCamL = (CStatic*)GetDlgItem(IDC_STATIC_camL);
	mStCamR = (CStatic*)GetDlgItem(IDC_STATIC_camR);
	mStGain = (CStatic*)GetDlgItem(IDC_STATIC_Gain);
	mStExposure = (CStatic*)GetDlgItem(IDC_STATIC_Exposure);




	CheckRadioButton(IDC_R_HD1080, IDC_R_HD2K, IDC_R_HD1080);
	CheckRadioButton(IDC_R_ULTRA, IDC_R_QUALITY, IDC_R_ULTRA);
	CheckRadioButton(IDC_R_STANDARD, IDC_R_FILL, IDC_R_STANDARD);

	return true;
}
HBRUSH realtimeCameraOpening::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Create a brush with the background color
	static CBrush brush(RGB(127, 130, 187)); // white color
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_Gain)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_Exposure)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_camL)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_camR)
		pDC->SetBkMode(TRANSPARENT);

	////
	if (pWnd->GetDlgCtrlID() == IDC_E_Gain0) {
		static CBrush brush(RGB(220, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_E_Gain1) {
		static CBrush brush(RGB(220, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_E_Exposure0) {
		static CBrush brush(RGB(220, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_E_Exposure1) {
		static CBrush brush(RGB(220, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
		pDC->SetBkMode(TRANSPARENT);

	//buttons
	if (pWnd->GetDlgCtrlID() == IDC_B_Camera_Open) {
		static CBrush brush(RGB(0, 255, 10)); // white color
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_BUTTON_goMeasementDialog) {
		static CBrush brush(RGB(0, 255, 100)); // white color
		return (HBRUSH)brush.GetSafeHandle();
	}


	// Return the brush
	return (HBRUSH)brush.GetSafeHandle();
}


BEGIN_MESSAGE_MAP(realtimeCameraOpening, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &realtimeCameraOpening::OnBnClickedOk)
	ON_BN_CLICKED(IDC_R_HD1080, &realtimeCameraOpening::OnBnClickedRHd1080)
	ON_BN_CLICKED(IDC_R_HD2K, &realtimeCameraOpening::OnBnClickedRHd2k)
	ON_BN_CLICKED(IDC_R_ULTRA, &realtimeCameraOpening::OnBnClickedRUltra)
	ON_BN_CLICKED(IDC_R_QUALITY, &realtimeCameraOpening::OnBnClickedRQuality)
	ON_BN_CLICKED(IDC_R_PERFORMANCE, &realtimeCameraOpening::OnBnClickedRPerformance)
	ON_BN_CLICKED(IDC_R_LAST, &realtimeCameraOpening::OnBnClickedRLast)
	ON_BN_CLICKED(IDC_R_STANDARD, &realtimeCameraOpening::OnBnClickedRStandard)
	ON_BN_CLICKED(IDC_R_FILL, &realtimeCameraOpening::OnBnClickedRFill)
	ON_BN_CLICKED(IDC_CHECK_CameraSettings, &realtimeCameraOpening::OnBnClickedCheckCamerasettings)
	ON_BN_CLICKED(IDC_B_Camera_Open, &realtimeCameraOpening::OnBnClickedBCameraOpen)
	ON_BN_CLICKED(IDC_CHECK_preview, &realtimeCameraOpening::OnBnClickedCheckpreview)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_B_Close_Camera, &realtimeCameraOpening::OnBnClickedBCloseCamera)
	ON_BN_CLICKED(IDC_BUTTON_goMeasementDialog, &realtimeCameraOpening::OnBnClickedButtongomeasementdialog)
END_MESSAGE_MAP()


// realtimeCameraOpening message handlers




// svoReading dialog

IMPLEMENT_DYNAMIC(svoReading, CDialog)

svoReading::svoReading(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SVO_reading, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

svoReading::~svoReading()
{
}

void svoReading::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

#if 0
	DDX_Text(pDX, IDC_Edit_filename, m_str);
	DDX_Text(pDX, IDC_Edit_Filename1, m_str1);
#else
	GetDlgItemTextW(IDC_Edit_filename, m_str);
	GetDlgItemTextW(IDC_Edit_Filename1, m_str1);
#endif
}

BOOL svoReading::OnInitDialog()
{
	CDialog::OnInitDialog();

	pFilename0 = (CEdit*)GetDlgItem(IDC_Edit_filename);

	//const char* l = (char*)localpath.data();
	pFilename0->SetWindowTextW(working_folder + _T("Munia_Data\\SVOs"));

	CEdit *pFilename1 = (CEdit*)GetDlgItem(IDC_Edit_Filename1);
	pFilename1->SetWindowTextW(_T("RH1"));


	CheckRadioButton(IDC_R_STANDARD, IDC_R_FILL, IDC_R_STANDARD);
	CheckRadioButton(IDC_R_ULTRA, IDC_R_QUALITY, IDC_R_ULTRA);

	return true;
}
HBRUSH svoReading::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Create a brush with the background color
	static CBrush brush(RGB(127, 130, 187)); // white color
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_press_enter)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_Edit_filename) {
		static CBrush brush(RGB(255, 205, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_Edit_Filename1) {
		static CBrush brush(RGB(255, 205, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
		pDC->SetBkMode(TRANSPARENT);


	//buttons

	if (pWnd->GetDlgCtrlID() == IDC_B_enter) {
		static CBrush brush(RGB(0, 255, 10)); // white color
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_BUTTON_goMeasementDialog) {
		static CBrush brush(RGB(0, 255, 100)); // white color
		return (HBRUSH)brush.GetSafeHandle();
	}


	// Return the brush
	return (HBRUSH)brush.GetSafeHandle();
}


BEGIN_MESSAGE_MAP(svoReading, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &svoReading::OnBnClickedOk)
	ON_BN_CLICKED(IDC_B_enter, &svoReading::OnBnClickedBenter)
	ON_BN_CLICKED(IDC_R_STANDARD, &svoReading::OnBnClickedRStandard)
	ON_BN_CLICKED(IDC_R_FILL, &svoReading::OnBnClickedRFill)
	ON_BN_CLICKED(IDC_R_ULTRA, &svoReading::OnBnClickedRUltra)
	ON_BN_CLICKED(IDC_R_QUALITY, &svoReading::OnBnClickedRQuality)
	ON_BN_CLICKED(IDC_R_PERFORMANCE, &svoReading::OnBnClickedRPerformance)
	ON_BN_CLICKED(IDC_R_LAST, &svoReading::OnBnClickedRLast)
	ON_BN_CLICKED(IDC_CHECK_preview, &svoReading::OnBnClickedCheckpreview)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_goMeasementDialog, &svoReading::OnBnClickedButtongomeasementdialog)
	ON_BN_CLICKED(IDC_B_all_svo_calculation, &svoReading::OnBnClickedBallsvocalculation)
	ON_BN_CLICKED(IDC_CHECK2, &svoReading::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON_selectFolder, &svoReading::OnBnClickedButtonselectfolder)
END_MESSAGE_MAP()


// svoReading message handlers


// measurements dialog

IMPLEMENT_DYNAMIC(measurements, CDialog)

measurements::measurements(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_Measurements, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

measurements::~measurements()
{
}

void measurements::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

#if 0
	DDX_Text(pDX, IDC_EDIT_userNote, UserNote);
#else
	GetDlgItemTextW(IDC_EDIT_userNote, UserNote);

#endif
}
BOOL measurements::OnInitDialog()
{
	CDialog::OnInitDialog();

	CStatic *pPmeasurementDlgBG = (CStatic*)GetDlgItem(IDC_STATIC_pMeasurementBG);
	HBITMAP entryIm = (HBITMAP)LoadImage(NULL, L"res\\BGmeasurement.bmp", IMAGE_BITMAP, 550, 210, LR_LOADFROMFILE);
	pPmeasurementDlgBG->SetBitmap(entryIm);

	userNote = (CEdit*)GetDlgItem(IDC_EDIT_userNote);
	userNote->SetWindowTextW(_T(""));

	return true;
}
HBRUSH measurements::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Create a brush with the background color
	static CBrush brush(RGB(80, 127, 128)); // white color
	// Return the brush
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_B_Calculation) {
		static CBrush brush(RGB(0, 2550, 0)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_userNote) {
		static CBrush brush(RGB(255, 255, 224)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}

	if (pWnd->GetDlgCtrlID() == IDC_BUTTON_cancel) {
		static CBrush brush(RGB(255, 0, 0)); // white color
		return (HBRUSH)brush.GetSafeHandle();
	}

	return (HBRUSH)brush.GetSafeHandle();
}


BEGIN_MESSAGE_MAP(measurements, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_mSettings, &measurements::OnBnClickedButtonmsettings)
	ON_BN_CLICKED(IDOK, &measurements::OnBnClickedOk)
	ON_BN_CLICKED(IDC_B_calibration, &measurements::OnBnClickedBcalibration)
	ON_BN_CLICKED(IDC_B_Select_Points, &measurements::OnBnClickedBSelectPoints)
	ON_BN_CLICKED(IDC_BUTTON_3dData, &measurements::OnBnClickedButton3ddata)
	ON_BN_CLICKED(IDC_CHECK_userNote, &measurements::OnBnClickedCheckusernote)
	ON_BN_CLICKED(IDC_BUTTON_cancel, &measurements::OnBnClickedButtoncancel)
	ON_BN_CLICKED(IDC_CHECK_ViewResultsPlot, &measurements::OnBnClickedCheckViewresultsplot)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_B_Calculation_flexiPoints, &measurements::OnBnClickedBCalculationflexipoints)
	ON_BN_CLICKED(IDC_Button_Calculation, &measurements::OnBnClickedButtonCalculation)
END_MESSAGE_MAP()


// measurements message handlers




void svoReading::OnBnClickedOk()
{
	check_preview = false;
	sensingMode_Fill = false;
	depthModeIndex = 1;
	right = 1;
	filterIndex = 2;
	
	calibration = 0;

	checkUserNote = 0;

	UpdateData(1);
	
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();
	
	if (this->m_hWnd != NULL)
		this->DestroyWindow();



	CDialog::OnOK();
}


void realtimeCameraOpening::OnBnClickedOk()
{
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}

	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();
	
	if (this->m_hWnd != NULL)
		this->DestroyWindow();

	resolution2K = false;
	depthModeIndex = 1;
	sensingMode_Fill = false;
	check_defaultCamera = 0;
	check_preview = 0;

	right = 1;
	filterIndex = 2;
	checkUserNote = 0;

	UpdateData(1);


	CDialog::OnOK();
}




void realtimeCameraOpening::OnBnClickedRHd1080()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(true);
	resolution2K = false;
	theApp.m().transformation_matrix(resolution2K);
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void realtimeCameraOpening::OnBnClickedRHd2k()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(true);
	resolution2K = true;
	theApp.m().transformation_matrix(resolution2K);
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void realtimeCameraOpening::OnBnClickedRUltra()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(true);
	depthModeIndex = 1;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void realtimeCameraOpening::OnBnClickedRQuality()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	depthModeIndex = 2;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void realtimeCameraOpening::OnBnClickedRPerformance()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	depthModeIndex = 3;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void realtimeCameraOpening::OnBnClickedRLast()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	depthModeIndex = 4;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void realtimeCameraOpening::OnBnClickedRStandard()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	sensingMode_Fill = false;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void realtimeCameraOpening::OnBnClickedRFill()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	sensingMode_Fill = true;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void realtimeCameraOpening::OnBnClickedCheckCamerasettings()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	check_defaultCamera = reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_CameraSettings))->GetCheck() == BST_CHECKED;

	//check_defaultCamera = !check_defaultCamera;

	if (check_defaultCamera) {
		mEditGainL->ShowWindow(SW_HIDE);
		mEditGainR->ShowWindow(SW_HIDE);
		mEditExposureL->ShowWindow(SW_HIDE);
		mEditExposureR->ShowWindow(SW_HIDE);

		mStCamL->ShowWindow(SW_HIDE);
		mStCamR->ShowWindow(SW_HIDE);
		mStGain->ShowWindow(SW_HIDE);
		mStExposure->ShowWindow(SW_HIDE);

	}
	else {
		mEditGainL->ShowWindow(SW_SHOW);
		mEditGainR->ShowWindow(SW_SHOW);
		mEditExposureL->ShowWindow(SW_SHOW);
		mEditExposureR->ShowWindow(SW_SHOW);

		mStCamL->ShowWindow(SW_SHOW);
		mStCamR->ShowWindow(SW_SHOW);
		mStGain->ShowWindow(SW_SHOW);
		mStExposure->ShowWindow(SW_SHOW);

	}
	UpdateData(true);



	int gainL = _ttoi(edit_gainL) == 0 ? 1 : _ttoi(edit_gainL);
	int gainR = _ttoi(edit_gainR) == 0 ? 1 : _ttoi(edit_gainR);
	int exposureL = _ttoi(edit_exposureL) == 0 ? 50 : _ttoi(edit_exposureL);
	int exposureR = _ttoi(edit_exposureR) == 0 ? 50 : _ttoi(edit_exposureR);

	if (!check_defaultCamera) {
		if (0 > gainL || gainL > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"));
			return;
		}
		if (0 > gainR || gainR > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"));
			return;
		}
		if (0 > exposureL || exposureL > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"));
			return;
		}
		if (0 > exposureR || exposureR > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"));
			return;
		}
	}

	if (realtime) {
		if (flag == true) {
			theApp.m().closecamera();
			//theApp.m().opencamera(check_defaultCamera, _ttoi(edit_gainL), _ttoi(edit_gainR), _ttoi(edit_exposureL), _ttoi(edit_exposureR), depthModeIndex, resolution2K, sensingMode_Fill);
			flag = false;
		}
	}
	else {
		if (flag == true) {
			theApp.m().closecamera();
			theApp.m().open_svo(theApp.m().filenames, depthModeIndex, resolution2K, sensingMode_Fill);
			flag = true;
		}
	}
}




void realtimeCameraOpening::OnBnClickedBCameraOpen()
{
	if (flag)
	{
		MessageBox(_T("The camera has already been opened"), _T("info"), MB_ICONEXCLAMATION);
		return;
	}
	

	
	
	CWinThread* thread1 = AfxBeginThread(threadFunc, this);
	CWnd *cwnd_B = (CWnd*)GetDlgItem(IDC_B_Camera_Open);
		cwnd_B->SetWindowTextW(_T("Opening..."));


	UpdateData(true);

	int gainL = _ttoi(edit_gainL) == 0 ? 1 : _ttoi(edit_gainL);
	int gainR = _ttoi(edit_gainR) == 0 ? 1 : _ttoi(edit_gainR);
	int exposureL = _ttoi(edit_exposureL) == 0 ? 18 : _ttoi(edit_exposureL);
	int exposureR = _ttoi(edit_exposureR) == 0 ? 18 : _ttoi(edit_exposureR);


	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	if (right) {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUR.csv", theApp.m().points_forU);
	}
	else {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUL.csv", theApp.m().points_forU);
	}

	
		if (realtime) {

			if (!check_defaultCamera) {
				if (0 > gainL || gainL > 100) {
					MessageBox(_T("Please enter value between 0 to 100"), _T("info"));
					return;
				}
				if (0 > gainR || gainR > 100) {
					MessageBox(_T("Please enter value between 0 to 100"), _T("info"));
					return;
				}
				if (0 > exposureL || exposureL > 100) {
					MessageBox(_T("Please enter value between 0 to 100"), _T("info"));
					return;
				}
				if (0 > exposureR || exposureR > 100) {
					MessageBox(_T("Please enter value between 0 to 100"), _T("info"));
					return;
				}
			}

			bool a = theApp.m().opencamera(check_defaultCamera, gainL, gainR, exposureL, exposureR, depthModeIndex, resolution2K, sensingMode_Fill);
			
			
			PostThreadMessage(thread1->m_nThreadID, WM_QUIT, 0, 0);
			if (mProgressBarDlg.m_hWnd != NULL) {
				Sleep(100);
				mProgressBarDlg.DestroyWindow();
			}


			if (a)
			{
				MessageBox(_T("The camera has been opened"), _T("info"));
				flag = true;
			}
			else
			{
				MessageBox(_T("The Cameras are disconnected. Please connect the camera properly."), _T("info"), MB_ICONEXCLAMATION);
			}
		
		
		}
	

	cwnd_B->SetWindowTextW(_T("Open Camera"));


}


void realtimeCameraOpening::OnBnClickedCheckpreview()
{
	UpdateData(true);

	check_preview = reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_preview))->GetCheck() == BST_CHECKED;

	//check_preview = !check_preview;
	if (check_preview) {

		SetTimer(2, 100, 0);
	}
	else {
		KillTimer(2);
		cv::destroyWindow("Preview");
	}
}


void realtimeCameraOpening::OnTimer(UINT_PTR nIDEvent)
{
	cv::Mat cv_bgrIm0, cv_bgrIm1;
	if (!theApp.m().extract_BGRIm(cv_bgrIm0, cv_bgrIm1)) {
		//KillTimer(1);
		cv::destroyWindow("Preview");
	}

	CDialog::OnTimer(nIDEvent);
}





void svoReading::OnBnClickedBenter()
{


	mProgressBarDlg.setProgPos(10, 100);
	CWnd *cwnd_B = (CWnd*)GetDlgItem(IDC_B_enter);
	cwnd_B->SetWindowTextW(_T("Reading..."));

	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
	mProgressBarDlg.setProgPos(10, 100);

	UpdateData(true);
	mProgressBarDlg.setProgPos(10, 100);

	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	if (right) {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUR.csv", theApp.m().points_forU);
	}
	else {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUL.csv", theApp.m().points_forU);
	}

	mProgressBarDlg.setProgPos(10, 100);

	UpdateData(true);

	std::string str = CW2A(m_str);
	std::string str1 = CW2A(m_str1);
	std::string path0, path1;
	path0 = str + "\\" + str1 + "-cam0.svo";
	path1 = str + "\\" + str1 + "-cam1.svo";

	theApp.m().filenames.clear();
	theApp.m().filenames.push_back(path0);
	theApp.m().filenames.push_back(path1);
	filename = true;

	bool a = theApp.m().open_svo(theApp.m().filenames, depthModeIndex, resolution2K, sensingMode_Fill);
	mProgressBarDlg.setProgPos(50, 300);

	mProgressBarDlg.setProgPos(10, 100);

	if (mProgressBarDlg.m_hWnd != NULL) {
		Sleep(100);
		mProgressBarDlg.DestroyWindow();
	}

	if (a)
	{
		MessageBox(_T("The svo ready for the Calculation."), _T("Info"));
		flag = true;
	}
	else
	{
		MessageBox(_T("The Svo paths are invalid. Pleaase provide valid path of the svo files."), _T("Notice"), MB_ICONHAND);
	}

	cwnd_B->SetWindowTextW(_T("Read SVO"));

}


void svoReading::OnBnClickedRStandard()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	sensingMode_Fill = false;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void svoReading::OnBnClickedRFill()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	sensingMode_Fill = true;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void svoReading::OnBnClickedRUltra()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	depthModeIndex = 1;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void svoReading::OnBnClickedRQuality()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	depthModeIndex = 2;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void svoReading::OnBnClickedRPerformance()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	depthModeIndex = 3;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void svoReading::OnBnClickedRLast()
{
	if (measurements_dlg.m_hWnd != NULL)
		measurements_dlg.DestroyWindow();

	UpdateData(1);
	depthModeIndex = 4;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}




void svoReading::OnBnClickedCheckpreview()
{
	UpdateData(true);
	check_preview = reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_preview))->GetCheck() == BST_CHECKED;
	if (check_preview) {

		SetTimer(3, 100, 0);
	}
	else {
		KillTimer(3);
		cv::destroyWindow("Preview");
	}
}


void svoReading::OnTimer(UINT_PTR nIDEvent)
{
	cv::Mat cv_bgrIm0, cv_bgrIm1;
	if (!theApp.m().extract_BGRIm(cv_bgrIm0, cv_bgrIm1)) {
		//KillTimer(1);
		cv::destroyWindow("Preview");
	}

	CDialog::OnTimer(nIDEvent);
}



// m_settings dialog

IMPLEMENT_DYNAMIC(m_settings, CDialog)

m_settings::m_settings(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_M_setting, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

m_settings::~m_settings()
{
}

void m_settings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

#if 0
	DDX_Text(pDX, IDC_Edit_ksize, Edit_ksize);
	DDX_Text(pDX, IDC_starting_frame_no, Edit_frame_starting_no);
	DDX_Text(pDX, IDC_ending_frame_no, Edid_ending_frame_no);
#else
	GetDlgItemTextW( IDC_Edit_ksize, Edit_ksize);
	GetDlgItemTextW(IDC_starting_frame_no, Edit_frame_starting_no);
	GetDlgItemTextW(IDC_ending_frame_no, Edid_ending_frame_no);

	GetDlgItemTextW(IDC_EDIT_LowerLimitOfRadius, lowerlimitradius);
	GetDlgItemTextW(IDC_EDIT_UpperLimitOfRadius, upperlimitradius);

#endif
}
BOOL m_settings::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckRadioButton(IDC_Bilateral_Filter, IDC_Gaussian_Blur, IDC_Gaussian_Blur);
	CheckRadioButton(IDC_RADIO_YED, IDC_RADIO_YY8, IDC_RADIO_YED);

	CheckRadioButton(IDC_Right, IDC_Left, IDC_Right);

	CheckRadioButton(IDC_RADIO_9points, IDC_RADIO_3points, IDC_RADIO_9points);


	CEdit *pKsize = (CEdit*)GetDlgItem(IDC_Edit_ksize);
	pKsize->SetWindowTextW(_T("375"));

	CEdit *startingFrame = (CEdit*)GetDlgItem(IDC_starting_frame_no);
	startingFrame->SetWindowTextW(_T("0"));

	CEdit *endingFrame = (CEdit*)GetDlgItem(IDC_ending_frame_no);
	endingFrame->SetWindowTextW(_T("10"));

	CEdit *lowerlimit = (CEdit*)GetDlgItem(IDC_EDIT_LowerLimitOfRadius);
	lowerlimit->SetWindowTextW(_T("1210"));

	CEdit *upperlimit = (CEdit*)GetDlgItem(IDC_EDIT_UpperLimitOfRadius);
	upperlimit->SetWindowTextW(_T("1250"));


	return true;
}

HBRUSH m_settings::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Create a brush with the background color
	static CBrush brush(RGB(55, 126, 71)); // white color

	if (pWnd->GetDlgCtrlID() == IDC_Using_frame_from)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_Using_frame_to)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_filter_ksize)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_string_upperlimit)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_string_lowerlimit)
		pDC->SetBkMode(TRANSPARENT);


	////
	if (pWnd->GetDlgCtrlID() == IDC_Edit_ksize) {
		static CBrush brush(RGB(230, 205, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_starting_frame_no) {
		static CBrush brush(RGB(255, 205, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_ending_frame_no) {
		static CBrush brush(RGB(255, 205, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_LowerLimitOfRadius) {
		static CBrush brush(RGB(255, 205, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_UpperLimitOfRadius) {
		static CBrush brush(RGB(255, 205, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}


	// Return the brush
	return (HBRUSH)brush.GetSafeHandle();
}



BEGIN_MESSAGE_MAP(m_settings, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_Bilateral_Filter, &m_settings::OnBnClickedBilateralFilter)
	ON_BN_CLICKED(IDC_Median_Blur, &m_settings::OnBnClickedMedianBlur)
	ON_BN_CLICKED(IDC_Gaussian_Blur, &m_settings::OnBnClickedGaussianBlur)
	ON_BN_CLICKED(IDC_Right, &m_settings::OnBnClickedRight)
	ON_BN_CLICKED(IDC_Left, &m_settings::OnBnClickedLeft)
	ON_BN_CLICKED(IDOK, &m_settings::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_YED, &m_settings::OnBnClickedRadioYed)
	ON_BN_CLICKED(IDC_RADIO_YY8, &m_settings::OnBnClickedRadioYy8)
	ON_BN_CLICKED(IDC_ideal_position_mirror, &m_settings::OnBnClickedidealpositionmirror)
	ON_BN_CLICKED(IDC_RADIO_9points, &m_settings::OnBnClickedRadio9points)
	ON_BN_CLICKED(IDC_RADIO_3points, &m_settings::OnBnClickedRadio3points)
END_MESSAGE_MAP()


// m_settings message handlers


void m_settings::OnBnClickedBilateralFilter()
{
	UpdateData(1);
	filterIndex = 0;
}


void m_settings::OnBnClickedMedianBlur()
{
	UpdateData(1);
	filterIndex = 1;
}


void m_settings::OnBnClickedGaussianBlur()
{
	UpdateData(1);
	filterIndex = 2;
}


void measurements::OnBnClickedButtonmsettings()
{
	if (mSetting_dlg.m_hWnd == NULL)
	{
		mSetting_dlg.Create(IDD_DIALOG_M_setting, this);
	}

		mSetting_dlg.ShowWindow(SW_SHOW);

	
}

void m_settings::OnBnClickedRight()
{
	right = true;

	UpdateData(true);

	//std::string item;
	//switch (theApp.m().itemIndex)
	//{
	//case 1:
	//	item = "YED";
	//	break;
	//case 2:
	//	item = "YY8";
	//	break;
	//}
	//theApp.m().points_forU.clear();
	//mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUR.csv", theApp.m().points_forU);
}

void m_settings::OnBnClickedLeft()
{
	right = false;

	UpdateData(true);

	//std::string item;
	//switch (theApp.m().itemIndex)
	//{
	//case 1:
	//	item = "YED";
	//	break;
	//case 2:
	//	item = "YY8";
	//	break;
	//}
	//theApp.m().points_forU.clear();
	//mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUL.csv", theApp.m().points_forU);
}

void m_settings::OnBnClickedOk()
{
	UpdateData(true);
	int startFrame = _ttoi(Edit_frame_starting_no)==0 ? 0 : _ttoi(Edit_frame_starting_no);
	int endFrame = _ttoi(Edid_ending_frame_no) == 0 ? 10 : _ttoi(Edid_ending_frame_no);
	int kSize = _ttoi(Edit_ksize) == 0 ? 375 : _ttoi(Edit_ksize);

	int lowerlimit = _ttoi(lowerlimitradius) == 0 ? 1210 : _ttoi(lowerlimitradius);
	int upperlimit = _ttoi(upperlimitradius) == 0 ? 1250 : _ttoi(upperlimitradius);

	if (endFrame < startFrame) {

		MessageBoxW(_T("Starting Frame No should be less than or equal to Ending Frame No."), _T("Notice"), MB_ICONHAND);
		return;
	}

	theApp.m().F_using_frames_from = startFrame;
	theApp.m().F_using_frames_to = endFrame;
	theApp.m().ksize = kSize;

	theApp.m().lowerLimit = lowerlimit;
	theApp.m().upperLimit = upperlimit;
	
	isCheckedSettings = 1;


	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	theApp.m().points_forU.clear();
	if (right) 
	{
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\" + item + "\\PointsforUR.csv", theApp.m().points_forU);
	}
	else
	{
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\" + item + "\\PointsforUL.csv", theApp.m().points_forU);
	}
	UpdateData(1);


	CDialog::OnOK();
}

void measurements::OnBnClickedOk()
{
	if (mSetting_dlg.m_hWnd != NULL)
		mSetting_dlg.ShowWindow(0);
	
	CDialog::OnOK();
}

void measurements::OnBnClickedBcalibration()
{
	UpdateData(1);

	if (mInputValueDlg.m_hWnd == NULL)
	{
		mInputValueDlg.Create(IDD_DIALOG_input, this);
	}
	mInputValueDlg.ShowWindow(SW_SHOW);

	
}

void measurements::OnBnClickedBSelectPoints()
{

	UpdateData(true);

	if (!isCheckedSettings)
	{
		MessageBox(_T("Please checke the settings first"), _T("info"), MB_ICONHAND);
		return;

	}

	theApp.m().extract_csvDepthPc(realtime, filterIndex);
	cv::Mat image = cv::imread(theApp.m().workingFolder + "Munia_Data\\Pics\\DepthImage.jpg");

	MessageBox(_T("Please select the points by clicking your mouse. After selection of the points, please press any kye of your kyeboard"), _T("info"));

	std::string str = "Pointsfor";
	if (theApp.m().selectPoint(1, right, 1, image, str))
	{
		MessageBox(_T("Points Selection has Completed"), _T("info"));
	}
	else
	{
		MessageBox(_T("Points Selection has Failed"), _T("info"));
	}


	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	if (right) {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUR.csv", theApp.m().points_forU);
	}
	else {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUL.csv", theApp.m().points_forU);

	}

	isCheckedSettings = 0;
}




void measurements::OnBnClickedButton3ddata()
{

	int result = MessageBoxW(_T("For 3d Radius Press YES \n \n For 3d Depth Press NO"), _T("Notice"), MB_YESNO);


	mProgressBarDlg.setProgPos(10, 300);
	mProgressBarDlg.setProgPos(10, 300);
	mProgressBarDlg.setProgPos(10, 300);


	//CWinThread* thread1 = AfxBeginThread(threadFunc, this);

	CWnd *cwnd_B = (CWnd*)GetDlgItem(IDC_BUTTON_3dData);
	cwnd_B->SetWindowTextW(_T("Saving 3D Data..."));


	theApp.m().extract_csvDepthPc(realtime, filterIndex);

	mProgressBarDlg.setProgPos(10, 300);
	mProgressBarDlg.setProgPos(10, 300);


	std::vector<cv::Point3f> data;


	if(result== IDYES)
		theApp.m().getVecFromMat(theApp.m().Pcimage, theApp.m().inputDepthImage, data, 1);
	else
		theApp.m().getVecFromMat(theApp.m().Pcimage, theApp.m().inputDepthImage, data, 0);

	


	mProgressBarDlg.setProgPos(5, 600);
	mProgressBarDlg.setProgPos(5, 600);
	mProgressBarDlg.setProgPos(5, 600);
	mProgressBarDlg.setProgPos(5, 600);
	mProgressBarDlg.setProgPos(5, 600);
	mProgressBarDlg.setProgPos(5, 600);
	mProgressBarDlg.setProgPos(5, 600);
	mProgressBarDlg.setProgPos(5, 600);
	mProgressBarDlg.setProgPos(5, 600);
	mProgressBarDlg.setProgPos(5, 600);


	//PostThreadMessage(thread1->m_nThreadID, WM_QUIT, 0, 0);

	if (mProgressBarDlg.m_hWnd != NULL) {
		Sleep(100);
		mProgressBarDlg.DestroyWindow();
	}

	MessageBox(_T("3D Data has been extracted"), _T("info"));
	
	cwnd_B->SetWindowTextW(_T("Take 3D Data"));

}


// svo_Recording dialog

IMPLEMENT_DYNAMIC(svo_Recording, CDialog)

svo_Recording::svo_Recording(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SVO_Record, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

svo_Recording::~svo_Recording()
{
}

void svo_Recording::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

#if 0
	DDX_Text(pDX, IDC_E_Gain0, edit_gainL);
	DDX_Text(pDX, IDC_E_Gain1, edit_gainR);
	DDX_Text(pDX, IDC_E_Exposure0, edit_exposureL);
	DDX_Text(pDX, IDC_E_Exposure1, edit_exposureR);
	DDX_Text(pDX, IDC_svo_path_0, m_str_svo0);
	DDX_Text(pDX, IDC_svo_path_1, m_str_svo1);
	DDX_Text(pDX, IDC_caono, edit_cap_no);
#else
	GetDlgItemTextW( IDC_E_Gain0, edit_gainL);
	GetDlgItemTextW(IDC_E_Gain1, edit_gainR);
	GetDlgItemTextW(IDC_E_Exposure0, edit_exposureL);
	GetDlgItemTextW(IDC_E_Exposure1, edit_exposureR);
	GetDlgItemTextW(IDC_svo_path_0, m_str_svo0);
	GetDlgItemTextW(IDC_svo_path_1, m_str_svo1);
	GetDlgItemTextW(IDC_caono, edit_cap_no);

#endif
}

BOOL svo_Recording::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckRadioButton(IDC_R_HD1080, IDC_R_HD2K, IDC_R_HD1080);

	mEditGainL = (CEdit*)GetDlgItem(IDC_E_Gain0);
	mEditGainL->SetWindowTextW(_T("1"));

	mEditGainR = (CEdit*)GetDlgItem(IDC_E_Gain1);
	mEditGainR->SetWindowTextW(_T("1"));

	mEditExposureL = (CEdit*)GetDlgItem(IDC_E_Exposure0);
	mEditExposureL->SetWindowTextW(_T("18"));

	mEditExposureR = (CEdit*)GetDlgItem(IDC_E_Exposure1);
	mEditExposureR->SetWindowTextW(_T("18"));

	mStCamL = (CStatic*)GetDlgItem(IDC_STATIC_camL);
	mStCamR = (CStatic*)GetDlgItem(IDC_STATIC_camR);
	mStGain = (CStatic*)GetDlgItem(IDC_STATIC_Gain);
	mStExposure = (CStatic*)GetDlgItem(IDC_STATIC_Exposure);

	mSvoRecord = (CButton*)GetDlgItem(IDC_B_Record_svo);

	mSvoPath0 = (CEdit*)GetDlgItem(IDC_svo_path_0);
	mSvoPath0->SetWindowTextW(working_folder + _T("Munia_Data\\SVOs\\Recorded_SVOs"));

	mSvoPath1 = (CEdit*)GetDlgItem(IDC_svo_path_1);
	mSvoPath1->SetWindowTextW(_T("RH or LH"));

	mEditCapno = (CEdit*)GetDlgItem(IDC_caono);
	mEditCapno->SetWindowTextW(_T("45"));


	mStCapno = (CStatic*)GetDlgItem(IDC_CapNo);

	mStPress_Record_svo = (CStatic*)GetDlgItem(IDC_STATIC_press_RecordSVO);


	return true;
}

HBRUSH svo_Recording::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Create a brush with the background color
	static CBrush brush(RGB(127, 130, 187)); // white color

	if (pWnd->GetDlgCtrlID() == IDC_CapNo)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_Gain)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_Exposure)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_camL)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_camR)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_press_RecordSVO)
		pDC->SetBkMode(TRANSPARENT);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
		pDC->SetBkMode(TRANSPARENT);

	////////////////

	if (pWnd->GetDlgCtrlID() == IDC_svo_path_0) {
		static CBrush brush(RGB(255, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_svo_path_1) {
		static CBrush brush(RGB(255, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}

	if (pWnd->GetDlgCtrlID() == IDC_caono) {
		static CBrush brush(RGB(255, 220, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	////

	if (pWnd->GetDlgCtrlID() == IDC_E_Gain0) {
		static CBrush brush(RGB(220, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_E_Gain1) {
		static CBrush brush(RGB(220, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_E_Exposure0) {
		static CBrush brush(RGB(220, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_E_Exposure1) {
		static CBrush brush(RGB(220, 255, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}

	////

	if (pWnd->GetDlgCtrlID() == IDC_B_Record_svo) {
		static CBrush brush(RGB(0, 255, 10)); // white color
		return (HBRUSH)brush.GetSafeHandle();
	}


	// Return the brush
	return (HBRUSH)brush.GetSafeHandle();
}


BEGIN_MESSAGE_MAP(svo_Recording, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_R_HD1080, &svo_Recording::OnBnClickedRHd1080)
	ON_BN_CLICKED(IDC_R_HD2K, &svo_Recording::OnBnClickedRHd2k)
	ON_BN_CLICKED(IDC_CHECK_CameraSettings, &svo_Recording::OnBnClickedCheckCamerasettings)
	ON_BN_CLICKED(IDC_B_Record_svo, &svo_Recording::OnBnClickedBRecordsvo)
	ON_BN_CLICKED(IDOK, &svo_Recording::OnBnClickedOk)
	ON_BN_CLICKED(IDC_B_opening_recording, &svo_Recording::OnBnClickedBopeningrecording)
	ON_BN_CLICKED(IDC_B_closing_recording, &svo_Recording::OnBnClickedBclosingrecording)
	ON_BN_CLICKED(IDC_BUTTON_selectFolder, &svo_Recording::OnBnClickedButtonselectfolder)
END_MESSAGE_MAP()


// svo_Recording message handlers


void svo_Recording::OnBnClickedRHd1080()
{
	resolution2K = false;
	theApp.m().transformation_matrix(resolution2K);
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void svo_Recording::OnBnClickedRHd2k()
{
	resolution2K = true;
	theApp.m().transformation_matrix(resolution2K);
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}




void svo_Recording::OnBnClickedCheckCamerasettings()
{
	check_defaultCamera = reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_CameraSettings))->GetCheck() == BST_CHECKED;

	//check_defaultCamera = !check_defaultCamera;
	if (check_defaultCamera) {
		mEditGainL->ShowWindow(SW_HIDE);
		mEditGainR->ShowWindow(SW_HIDE);
		mEditExposureL->ShowWindow(SW_HIDE);
		mEditExposureR->ShowWindow(SW_HIDE);

		mStCamL->ShowWindow(SW_HIDE);
		mStCamR->ShowWindow(SW_HIDE);
		mStGain->ShowWindow(SW_HIDE);
		mStExposure->ShowWindow(SW_HIDE);

	}
	else {
		mEditGainL->ShowWindow(SW_SHOW);
		mEditGainR->ShowWindow(SW_SHOW);
		mEditExposureL->ShowWindow(SW_SHOW);
		mEditExposureR->ShowWindow(SW_SHOW);

		mStCamL->ShowWindow(SW_SHOW);
		mStCamR->ShowWindow(SW_SHOW);
		mStGain->ShowWindow(SW_SHOW);
		mStExposure->ShowWindow(SW_SHOW);

	}
	UpdateData(true);

	if (!check_defaultCamera) {
		if (0 > _ttoi(edit_gainL) || _ttoi(edit_gainL) > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"), MB_ICONHAND);
			return;
		}
		if (0 > _ttoi(edit_gainR) || _ttoi(edit_gainR) > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"), MB_ICONHAND);
			return;
		}
		if (0 > _ttoi(edit_exposureL) || _ttoi(edit_exposureL) > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"), MB_ICONHAND);
			return;
		}
		if (0 > _ttoi(edit_exposureR) || _ttoi(edit_exposureR) > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"), MB_ICONHAND);
			return;
		}
	}

	if (realtime) {
		if (flag == true) {
			theApp.m().closecamera();
			//theApp.m().opencamera(check_defaultCamera, _ttoi(edit_gainL), _ttoi(edit_gainR), _ttoi(edit_exposureL), _ttoi(edit_exposureR), depthModeIndex, resolution2K, sensingMode_Fill);
			flag = false;
		}
	}
	else {
		if (flag == true) {
			theApp.m().closecamera();
			theApp.m().open_svo(theApp.m().filenames, depthModeIndex, resolution2K, sensingMode_Fill);
			flag = true;
		}
	}
}




void svo_Recording::OnBnClickedBRecordsvo()
{

	if (!flag) {
		MessageBox(_T("Please open recording first"), _T("info"));
		return;
	}

	//CWinThread* thread1 = AfxBeginThread(threadFunc, this);


	//mProgressBarDlg.SetTimer(1, 100, 0);
		//auto func = [](progressBar a) {	a.setProgPos(10, 100);
		//};
		//std::thread t(func, mProgressBarDlg);
		//t.join();
	mProgressBarDlg.setProgPos(10, 100);



	CWnd *cwnd_B_record = (CWnd*)GetDlgItem(IDC_B_Record_svo);
	cwnd_B_record->SetWindowTextW(_T("Recording..."));

	UpdateData(true);
	mProgressBarDlg.setProgPos(10, 100);

	int capno = _ttoi(edit_cap_no)==0? 45: _ttoi(edit_cap_no);

	mProgressBarDlg.setProgPos(10, 100);


	std::string date_and_time;
	std::time_t now = std::time(nullptr); // get current time as seconds since epoch
	std::tm local_time; // create a tm structure to hold the local time
	localtime_s(&local_time, &now); // convert to local time and store in local_time
	char buffer[80]; // create a buffer to hold the formatted date and time
	std::strftime(buffer, 80, "%d%m%Y-%H%M", &local_time); // format the date and time
	date_and_time= std::string(buffer); // return as a string


	std::ofstream indexFile;

	std::string str = CW2A(m_str_svo0);
	char* dir = &str[0];
	_mkdir(dir);



	std::string dirN = str + "\\Index";
	std::string indexFname = dirN + ".csv";
	std::ifstream indFile(indexFname);

	if (indFile.is_open())
	{
		std::string line;
		std::getline(indFile, line);
		std::stringstream ss(line);
		char comma;
		ss >> n ;
		indFile.close();
	}
	else { n = 0; }

	std::string str1 = CW2A(m_str_svo1);

	n++;
	std::string path_str0= str+ "\\"+ str1 +/*"-"+*/std::to_string(n)+ /*"-"+ date_and_time*/ +"-cam0.svo";
	std::string path_str1 = str +"\\" + str1 + /*"-" +*/ std::to_string(n) + /*"-" + date_and_time*/ + "-cam1.svo";

	theApp.m().svo_path[0] = &path_str0[0];
	theApp.m().svo_path[1] = &path_str1[0];
	

	indexFile.open(indexFname);
	indexFile << std::to_string(n);
	indexFile.close();



	//std::string str = CW2A(m_str_svo0);
	//theApp.m().svo_path[0] = &str[0];

	//std::string str1 = CW2A(m_str_svo1);
	//theApp.m().svo_path[1] = &str1[0];



	bool a = theApp.m().recording_svo(theApp.m().svo_path[0], theApp.m().svo_path[1], capno);

	
	mProgressBarDlg.setProgPos(20, 100);

	

	
	mProgressBarDlg.setProgPos(10,1000);

	//mProgressBarDlg.KillTimer(1);
	
	
	//PostThreadMessage(thread1->m_nThreadID, WM_QUIT, 0, 0);
	Sleep(100);
	if (mProgressBarDlg.m_hWnd != NULL) {
		mProgressBarDlg.DestroyWindow();
	}


	if (a)
	{
		MessageBox(_T("Recording Has been done"), _T("info"));
	}
	else
	{
		MessageBox(_T("Recording Has been Failed. Please check the path of SVO"), _T("Notice"), MB_ICONHAND);
	}

	cwnd_B_record->SetWindowTextW(_T("Record SVO"));
}


void CMuniamfcDlg::OnBnClickedRadiosvorecording()
{


	if (autoCalculation) {
		autoCalculation = 0;
		pBtn->SetCheck(0);
	}

	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}


	startingOptionIndex = 3;
	
	if (realtime_dlg.m_hWnd != NULL)
		realtime_dlg.DestroyWindow();

	if (svoRead_dlg.m_hWnd != NULL)
		svoRead_dlg.DestroyWindow();

}


void realtimeCameraOpening::OnBnClickedBCloseCamera()
{
	if(!flag) {
	MessageBox(_T("The camera hasn't opened yet"), _T("Notice"), MB_ICONEXCLAMATION);
	return;
	}

	mProgressBarDlg.setProgPos(10, 10);
	CWnd *cwnd_B = (CWnd*)GetDlgItem(IDC_B_Close_Camera);
	cwnd_B->SetWindowTextW(_T("Closing..."));


	bool a = theApp.m().closecamera();
	mProgressBarDlg.setProgPos(40, 100);
	mProgressBarDlg.setProgPos(40, 300);
	mProgressBarDlg.setProgPos(10, 100);

	if (mProgressBarDlg.m_hWnd != NULL) {
		Sleep(100);
		mProgressBarDlg.DestroyWindow();
	}

		if (a)
		{
			MessageBox(_T("The camera has been closed."), _T("Info"));
			flag = false;
		}
		else
		{
			MessageBox(_T("Failed"), _T("Notice"), MB_ICONHAND);
		}
		cwnd_B->SetWindowTextW(_T("Close Camera"));

}



void CMuniamfcDlg::OnBnClickedButtonstart()
{
	/*if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
	KillTimer(4);
	cv::destroyAllWindows();*/

	if (autoCalculation) {
		autoCalculation = 0;
		pBtn->SetCheck(0);
	}

	switch (startingOptionIndex) 
	{
	case 1:
		if (realtime_dlg.m_hWnd == NULL)
		{
			realtime_dlg.Create(IDD_DIALOG_Realtime, this);
		}
		realtime_dlg.ShowWindow(SW_SHOW);
	break;
	case 2:
		if (svoRead_dlg.m_hWnd == NULL)
		{
			svoRead_dlg.Create(IDD_DIALOG_SVO_reading, this);
		}			
		svoRead_dlg.ShowWindow(SW_SHOW);
	break;
	case 3:
		if (svoRecord_dlg.m_hWnd == NULL)
		{
			svoRecord_dlg.Create(IDD_DIALOG_SVO_Record, this);
		}
		svoRecord_dlg.ShowWindow(SW_SHOW);
	break;
	}

}





void realtimeCameraOpening::OnBnClickedButtongomeasementdialog()
{
	UpdateData(1);
	if (flag) {
		if (measurements_dlg.m_hWnd == NULL)
		{
			measurements_dlg.Create(IDD_DIALOG_Measurements, this);
		}
		measurements_dlg.ShowWindow(SW_SHOW);
	}
	else {
		MessageBoxW(_T("Please Open The Camera First."), _T("Notice"), MB_ICONEXCLAMATION);
	}
}


void svoReading::OnBnClickedButtongomeasementdialog()
{
	UpdateData(1);
	if (flag || calibration) {
		if (measurements_dlg.m_hWnd == NULL)
		{
			measurements_dlg.Create(IDD_DIALOG_Measurements, this);
		}
		measurements_dlg.ShowWindow(SW_SHOW);
	}
	else {
		MessageBoxW(_T("Please Read The SVO file First."), _T("Notice"), MB_ICONEXCLAMATION);
	}
}


void svo_Recording::OnBnClickedOk()
{
	if (this->m_hWnd != NULL)
		this->DestroyWindow();
	resolution2K = 0;
	check_defaultCamera = 0;
	if (flag)
	{
		theApp.m().closecamera();
		flag = 0;
	}

	CDialog::OnOK();
}


// progressBar dialog

IMPLEMENT_DYNAMIC(progressBar, CDialog)

progressBar::progressBar(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_progressBar, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

progressBar::~progressBar()
{
}

void progressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BOOL progressBar::OnInitDialog()
{
	CDialog::OnInitDialog();


	CProgressCtrl *progress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	CProgressCtrl *progress1 = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS2);
	CProgressCtrl *progress2 = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS2);

	progress->SetRange(0, 100);
	progress->SetStep(1);
	progress->SetPos(0);

	progress1->SetRange(0, 5000);
	progress1->SetStep(1);
	progress1->SetPos(0);

	progress2->SetRange(0, 15000);
	progress2->SetStep(1);
	progress2->SetPos(0);


	//GetDlgItem(IDC_STATIC_progressUpdate)->SetWindowTextW(_T("0"));
	//Sleep(1000);


	return true;
}

BEGIN_MESSAGE_MAP(progressBar, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// progressBar message handlers

UINT threadFunc(LPVOID pParam) {
	progressBar* ptr = (progressBar*)(pParam);
	
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	progressBar m;

	while (1) {
		m.setProgPos1(1, 1);
		GetMessage(&msg, NULL, 0, 0);
		if (msg.message == WM_QUIT)
		{
			AfxEndThread(msg.wParam);
		}

	}
	return 0;
}



bool progressBar::setProgPos(int stepsize, int pauseTime) {

	if (this->m_hWnd == NULL)
	{
		this->Create(IDD_DIALOG_progressBar);
		this->ShowWindow(SW_SHOW);
	}


	CProgressCtrl *progress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);

	for (int i = 0; i < stepsize; i++){
		
		progress->StepIt();


		//Sleep(1);
	}

	
	Sleep(pauseTime);

	return true;
}
bool progressBar::setProgPos1(int stepsize, int pauseTime) {

	if (this->m_hWnd == NULL)
	{
		//CDialog* pParent = (CDialog*)GetWindow(IDD_DIALOG_SVO_reading);
		//this->SetParent(IDD_DIALOG_Measurements);
		this->Create(IDD_DIALOG_progressBar);
		this->ShowWindow(SW_SHOW);
	}
	CProgressCtrl *progress1 = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS2);
	
	for (int i = 0; i < stepsize; i++) 
	{
		progress1->StepIt();
		Sleep(pauseTime);
	}


	return true;
}
bool progressBar::setProgPos2(int stepsize, int pauseTime) {

	
	CProgressCtrl *progress2 = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS2);

	for (int i = 0; i < stepsize; i++)
	{
		progress2->StepIt();

		//Sleep(1);
	}

	Sleep(pauseTime);

	return true;
}

HBRUSH progressBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// Create a brush with the background color
	static CBrush brush(RGB(117, 250, 141)); // white color

	//if (pWnd->GetDlgCtrlID() == IDC_STATIC_progressUpdate) {
	//	pDC->SetBkMode(TRANSPARENT);
	//}
	
	// Return the brush
	return (HBRUSH)brush.GetSafeHandle();
}

void progressBar::OnTimer(UINT_PTR nIDEvent)
{
	
	setProgPos2(10, 1);

	CDialog::OnTimer(nIDEvent);
}


void measurements::OnBnClickedCheckusernote()
{
	checkUserNote = !checkUserNote;

	if (checkUserNote) {
		userNote->ShowWindow(SW_SHOW);
	}
	else {
		userNote->ShowWindow(SW_HIDE);
	}
}


void measurements::OnBnClickedButtoncancel()
{
	checkUserNote = 0;
	right = 1;
	filterIndex = 2;
	viewResultsPlot = 0;
	theApp.m().idealPositionOFmirror = 0;

	UpdateData(1);

	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}

	KillTimer(3);
	cv::destroyWindow("Result's Plot");

	if (mSetting_dlg.m_hWnd != NULL)
		mSetting_dlg.DestroyWindow();

	this->DestroyWindow();

	CDialog::OnOK();
}

// Ucalibraton dialog

IMPLEMENT_DYNAMIC(Ucalibraton, CDialog)

Ucalibraton::Ucalibraton(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_Ucalibration, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

Ucalibraton::~Ucalibraton()
{
}

void Ucalibraton::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	GetDlgItemTextW(IDC_Edit_filename, m_str);
	GetDlgItemTextW(IDC_EDIT_ncol, edit_ncol);
	GetDlgItemTextW(IDC_EDIT_nrow, edit_nrow);

}
BOOL Ucalibraton::OnInitDialog()
{
	CDialog::OnInitDialog();

	CheckRadioButton(IDC_Right, IDC_Left, IDC_Right);
	CheckRadioButton(IDC_R_HD1080, IDC_R_HD2K, IDC_R_HD1080);
	CheckRadioButton(IDC_RADIO_YED, IDC_RADIO_YY8, IDC_RADIO_YED);

	pFilename0 = (CEdit*)GetDlgItem(IDC_Edit_filename);
	pFilename0->SetWindowTextW(_T(""));

	mEditNcol = (CEdit*)GetDlgItem(IDC_EDIT_ncol);
	mEditNcol->SetWindowTextW(_T("7"));

	mEditNrow = (CEdit*)GetDlgItem(IDC_EDIT_nrow);
	mEditNrow->SetWindowTextW(_T("5"));


	return true;
}

HBRUSH Ucalibraton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	static CBrush brush(RGB(55, 126, 71)); 

	if (pWnd->GetDlgCtrlID() == IDC_Edit_filename) {
		static CBrush brush(RGB(255, 255, 255)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_ncol) {
		static CBrush brush(RGB(255, 255, 255)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_nrow) {
		static CBrush brush(RGB(255, 255, 255)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
		pDC->SetBkMode(TRANSPARENT);

	return (HBRUSH)brush.GetSafeHandle();
}

BEGIN_MESSAGE_MAP(Ucalibraton, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_take_U, &Ucalibraton::OnBnClickedButtontakeU)
	ON_BN_CLICKED(IDC_BUTTON_combile_U, &Ucalibraton::OnBnClickedButtoncombileU)
	ON_BN_CLICKED(IDC_Right, &Ucalibraton::OnBnClickedRight)
	ON_BN_CLICKED(IDC_Left, &Ucalibraton::OnBnClickedLeft)
	ON_BN_CLICKED(IDOK, &Ucalibraton::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_change_Ucsv, &Ucalibraton::OnBnClickedButtonchangeUcsv)
	ON_BN_CLICKED(IDC_B_optimumU, &Ucalibraton::OnBnClickedBoptimumu)
	ON_BN_CLICKED(IDC_BUTTON_checkOptimizationResults, &Ucalibraton::OnBnClickedButtoncheckoptimizationresults)
	ON_BN_CLICKED(IDC_BUTTON_selectFolder, &Ucalibraton::OnBnClickedButtonselectfolder)
	ON_BN_CLICKED(IDC_BUTTON_camera_calibration, &Ucalibraton::OnBnClickedButtoncameracalibration)
	ON_BN_CLICKED(IDC_CHECK_manual, &Ucalibraton::OnBnClickedCheckmanual)
	ON_BN_CLICKED(IDC_R_HD1080, &Ucalibraton::OnBnClickedRHd1080)
	ON_BN_CLICKED(IDC_R_HD2K, &Ucalibraton::OnBnClickedRHd2k)
	ON_BN_CLICKED(IDC_RADIO_YED, &Ucalibraton::OnBnClickedRadioYed)
	ON_BN_CLICKED(IDC_RADIO_YY8, &Ucalibraton::OnBnClickedRadioYy8)
END_MESSAGE_MAP()


// Ucalibraton message handlers


void Ucalibraton::OnBnClickedButtontakeU()
{

	mProgressBarDlg.setProgPos(10, 50);

	CWnd *cwnd_B = (CWnd*)GetDlgItem(IDC_BUTTON_take_U);
	cwnd_B->SetWindowTextW(_T("Saving..."));

	UpdateData(true);
	mProgressBarDlg.setProgPos(10, 10);

	theApp.m().extract_csvDepthPc(realtime, filterIndex);
	mProgressBarDlg.setProgPos(30, 10);

	bool a = theApp.m().extract_csv_UforRadius(1, theApp.m().inputDepthImage, right);

	mProgressBarDlg.setProgPos(30, 10);


	mProgressBarDlg.setProgPos(10, 10);



	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	

	if (right) {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUR.csv", theApp.m().points_forU);
	}
	else {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\" + item + "\\PointsforUL.csv", theApp.m().points_forU);
	}
	mProgressBarDlg.setProgPos(10, 200);


	if (mProgressBarDlg.m_hWnd != NULL) {
		Sleep(100);
		mProgressBarDlg.DestroyWindow();
	}

	

	cwnd_B->SetWindowTextW(_T("Save U's CSVs"));
}


void Ucalibraton::OnBnClickedButtoncombileU()
{
	CWnd *cwnd_B = (CWnd*)GetDlgItem(IDC_BUTTON_combile_U);
	cwnd_B->SetWindowTextW(_T("Preparing..."));


	std::string strL, strR ;

	strL = theApp.m().workingFolder + "Munia_Data\\U\\forRdatas\\forRdataLH";
	strR = theApp.m().workingFolder + "Munia_Data\\U\\forRdatas\\forRdataRH";
#if 0
	bool a=theApp.m().combined_Ufactor(strL, strR);

	if (a)
	{
		MessageBox(_T("Calibration is Successful"), _T("info"));
	}
	else
	{
		MessageBox(_T("Calibration Failed"), _T("info"));
	}

#else
	std::vector< std::vector<cv::Point3d>> vectorsVector;
	std::vector<cv::Point3d> resultVector;

	//theApp.m().DoEvent();
	bool a=theApp.m().csvToVectors(vectorsVector, resultVector, strL, strR);
	//mProgressBarDlg.setProgPos(20, 10);

	if (!a)
		return;

	cwnd_B->SetWindowTextW(_T("Progressing..."));

	//theApp.m().DoEvent();


	bool b = 0;

	for (int i = 0; i < vectorsVector.size(); i++) {
		b=theApp.m().vectorOperations(resultVector, vectorsVector[i]);
		if(!b)
		{
			break;
			return;
		}

		//theApp.m().DoEvent();
	}
	//mProgressBarDlg.setProgPos(60, 10);

	cwnd_B->SetWindowTextW(_T("Extracting..."));

	bool c=theApp.m().extract_U_csv(resultVector);
	//mProgressBarDlg.setProgPos(10, 10);
	//if (mProgressBarDlg.m_hWnd != NULL) {
	//	Sleep(100);
	//	mProgressBarDlg.DestroyWindow();
	//}

	if (c)
	{
		MessageBox(_T("Calibration is Successful"), _T("info"));
	}
	else
	{
		MessageBox(_T("Calibration Failed"), _T("info"));
	}

#endif

	theApp.m().points_forR.clear();
	mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\forRdata.csv", theApp.m().points_forR);



	cwnd_B->SetWindowTextW(_T("Final U's CSV"));

}


void Ucalibraton::OnBnClickedRight()
{
	UpdateData(true);

	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	theApp.m().points_forU.clear();
	mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUR.csv", theApp.m().points_forU);
	right = true;
	UpdateData(true);

}


void Ucalibraton::OnBnClickedLeft()
{
	UpdateData(true);
	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}
	theApp.m().points_forU.clear();
	mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUL.csv", theApp.m().points_forU);
	right = false;
	UpdateData(true);

}



void Ucalibraton::OnBnClickedOk()
{
	theApp.m().transformation_matrix(resolution2K);

	right = 1;
	UpdateData(1);
	CDialog::OnCancel();
	this->DestroyWindow();
}


// inputValue dialog

IMPLEMENT_DYNAMIC(inputValue, CDialog)

inputValue::inputValue(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_input, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

inputValue::~inputValue()
{
}

void inputValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_takeInput, input_val);

	GetDlgItemTextW(IDC_EDIT_takeInput, input_val);
	
	CEdit* pinputVal = (CEdit*)GetDlgItem(IDC_EDIT_takeInput);
	pinputVal->SetPasswordChar('*');
}
BOOL inputValue::OnInitDialog()
{
	CDialog::OnInitDialog();


	//pinputVal->SetWindowTextW(_T("Enter a code"));



	return true;
}

HBRUSH inputValue::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	static CBrush brush(RGB(55, 126, 71));
	

	if (pWnd->GetDlgCtrlID() == IDC_EDIT_takeInput) {
		static CBrush brush(RGB(230, 205, 204)); // white color
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)brush.GetSafeHandle();
	}
	return (HBRUSH)brush.GetSafeHandle();
}
BEGIN_MESSAGE_MAP(inputValue, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &inputValue::OnBnClickedOk)
END_MESSAGE_MAP()


// inputValue message handlers




void inputValue::OnBnClickedOk()
{
	UpdateData(1);

	std::string str = CW2A(input_val);
	inputVal = &str[0];



	UpdateData(1);


	if (inputVal == "munia") {
		if (mUcalibrationDlg.m_hWnd == NULL)
		{
			mUcalibrationDlg.Create(IDD_DIALOG_Ucalibration);
		}
		mUcalibrationDlg.ShowWindow(SW_SHOW);
		if (this->m_hWnd != NULL)
		{
			this->ShowWindow(0);
		}
	}
	else {
		MessageBoxW(_T("Match the code"), _T("Info"),  MB_ICONHAND);

	
	}

	CEdit* pinputVal = (CEdit*)GetDlgItem(IDC_EDIT_takeInput);
	pinputVal->SetWindowTextW(_T(""));;
	

	CDialog::OnOK();
}



void svoReading::OnBnClickedBallsvocalculation()
{
	if (!isCheckedSettings)
	{
		MessageBox(_T("Please checke the settings first"), _T("info"), MB_ICONHAND);
		return;

	}
		mProgressBarDlg.setProgPos(10, 100);
		CWnd *cwnd_B = (CWnd*)GetDlgItem(IDC_B_all_svo_calculation);
		cwnd_B->SetWindowTextW(_T("Calculating..."));
		if (flag) {
			theApp.m().closecamera();
			flag = false;
		}
		mProgressBarDlg.setProgPos(10, 100);
		mProgressBarDlg.setProgPos(10, 100);
		mProgressBarDlg.setProgPos(10, 100);
		mProgressBarDlg.setProgPos(10, 100);
		mProgressBarDlg.setProgPos(10, 100);
		mProgressBarDlg.setProgPos(10, 100);

		UpdateData(true);


		std::string str = CW2A(m_str);
		std::string str1 = CW2A(m_str1);
		std::string path0, path1;

		std::string item;
		switch (theApp.m().itemIndex)
		{
		case 1:
			item = "YED";
			break;
		case 2:
			item = "YY8";
			break;
		}

		right = 0;
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUL.csv", theApp.m().points_forU);
		UpdateData(true);

		for (int i = 1; ; i++) {
			path0 = str + "\\" +"LH"+ str1 + /*"-" +*/ std::to_string(i) + "-cam0.svo";
			path1 = str + "\\" +"LH"+ str1 + /*"-" +*/ std::to_string(i) + "-cam1.svo";

			theApp.m().filenames.clear();
			theApp.m().filenames.push_back(path0);
			theApp.m().filenames.push_back(path1);

			bool a = theApp.m().open_svo(theApp.m().filenames, depthModeIndex, resolution2K, sensingMode_Fill);

			if (a) {
				flag = 1;
				std::vector<double> vec;
				theApp.m().extract_csvDepthPc(realtime, filterIndex);
				theApp.m().extract_Radius(theApp.m().inputDepthImage, vec, right, "LH"+ str1+/*"-" +*/ std::to_string(i));
				std::vector<int>l_index, u_index;
				int decision_index = theApp.m().checkingResults(vec, theApp.m().resultPlot, l_index, u_index, right);

				if (theApp.m().closecamera()) { flag = 0; }
			}
			else {
				break;
			}
		}

		mProgressBarDlg.setProgPos(20, 100);

		right = 1;
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUR.csv", theApp.m().points_forU);
		UpdateData(true);

		for (int i = 1; ; i++) {
			path0 = str + "\\" +"RH"+ str1 + /*"-" +*/ std::to_string(i) + "-cam0.svo";
			path1 = str + "\\" +"RH"+ str1 + /*"-" +*/ std::to_string(i) + "-cam1.svo";

			theApp.m().filenames.clear();
			theApp.m().filenames.push_back(path0);
			theApp.m().filenames.push_back(path1);

			bool a = theApp.m().open_svo(theApp.m().filenames, depthModeIndex, resolution2K, sensingMode_Fill);

			if (a) {
				flag = 1;
				std::vector<double> vec;

				theApp.m().extract_csvDepthPc(realtime, filterIndex);
				theApp.m().extract_Radius(theApp.m().inputDepthImage, vec, right, "RH"+str1 +/*"-"+*/ std::to_string(i));
				std::vector<int>l_index, u_index;
				int decision_index = theApp.m().checkingResults(vec, theApp.m().resultPlot, l_index, u_index, right);

				if (theApp.m().closecamera()) { flag = 0; }
			}
			else {
				break;
			}
		}


		mProgressBarDlg.setProgPos(10, 100);
		if (mProgressBarDlg.m_hWnd != NULL) {
			Sleep(100);
			mProgressBarDlg.DestroyWindow();
		}
		MessageBox(_T("Done."), _T("Info"));
		cwnd_B->SetWindowTextW(_T("Calculate All SVO"));

		isCheckedSettings = 0;
	
}


void Ucalibraton::OnBnClickedButtonchangeUcsv()
{
	int result = MessageBoxW(_T("If you need to change U from R, Press YES \n \n If you only need to change U, Press NO"), _T("Notice"), MB_YESNO);


	UpdateData(true);

	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	if (right) {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUR.csv", theApp.m().points_forU);
	}
	else {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\PointsforUL.csv", theApp.m().points_forU);

	}

	if (result == IDYES) {
		theApp.m().extract_csvDepthPc(realtime, filterIndex);
		theApp.m().change_points_UforRadius(theApp.m().inputDepthImage, theApp.m().points_forU, 1);
	}
	else
	{
		theApp.m().change_points_UforRadius(theApp.m().inputDepthImage, theApp.m().points_forU, 0);
	}

	theApp.m().points_forR.clear();
	mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\forRdata.csv", theApp.m().points_forR);
	UpdateData(true);

	MessageBox(_T("Done!"), _T("Info"));
}

void svo_Recording::OnBnClickedBopeningrecording()
{
	CWnd *cwnd_B_record = (CWnd*)GetDlgItem(IDC_B_opening_recording);
	cwnd_B_record->SetWindowTextW(_T("Opening..."));

	UpdateData(true);

	if (flag) {
		theApp.m().closecamera();

		flag = false;

	}


	int gainL = _ttoi(edit_gainL) == 0 ? 1 : _ttoi(edit_gainL);
	int gainR = _ttoi(edit_gainR) == 0 ? 1 : _ttoi(edit_gainR);
	int exposureL = _ttoi(edit_exposureL) == 0 ? 50 : _ttoi(edit_exposureL);
	int exposureR = _ttoi(edit_exposureR) == 0 ? 50 : _ttoi(edit_exposureR);

	if (!check_defaultCamera) {
		if (0 > gainL || gainL > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"), MB_ICONHAND);
			return;
		}
		if (0 > gainR || gainR > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"), MB_ICONHAND);
			return;
		}
		if (0 > exposureL || exposureL > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"), MB_ICONHAND);
			return;
		}
		if (0 > exposureR || exposureR > 100) {
			MessageBox(_T("Please enter value between 0 to 100"), _T("info"), MB_ICONHAND);
			return;
		}
	}



	bool a=theApp.m().opening_recording_svo(check_defaultCamera, gainL, gainR, exposureL, exposureR, resolution2K);
	

		if (a)
		{
			MessageBox(_T("The cameras are ready for Recording"), _T("info"));
			flag = true;
		}
		else
		{
			MessageBox(_T("Failed. Connection of the Camera"), _T("Notice"), MB_ICONHAND);
		}

		cwnd_B_record->SetWindowTextW(_T("Opening Recording"));
		
}


void svo_Recording::OnBnClickedBclosingrecording()
{
	if (flag) {
		theApp.m().closecamera();
		flag = false;
		MessageBox(_T("Done!"), _T("Info"));
	}else
		MessageBox(_T("Cameras haven't openned yet!"), _T("Info"));


}

void Ucalibraton::OnBnClickedBoptimumu()
{
	UpdateData(1);
	std::string str = CW2A(m_str);
	std::vector<cv::Point3d>resultsPoints;
	std::vector<cv::Point3d>opU;

	if (str == "")
		return;
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
	theApp.m().points_optimum_UforRadius(str, resultsPoints, opU, right, realtime, sensingMode_Fill, depthModeIndex, resolution2K, filterIndex);
	
	
	MessageBox(_T("Done!"), _T("Info"));
}


void svoReading::OnBnClickedCheck2()
{
	calibration = !calibration;
}


void measurements::OnBnClickedCheckViewresultsplot()
{
	UpdateData(true);

	//viewResultsPlot = reinterpret_cast<CButton*>(GetDlgItem(IDC_CHECK_ViewResultsPlot))->GetCheck() == BST_CHECKED;

	viewResultsPlot = !viewResultsPlot;
	if (viewResultsPlot) {

		SetTimer(3, 100, 0);
		
	}
	else {
		KillTimer(3);
		cv::destroyWindow("Result's Plot");
	}
}


void measurements::OnTimer(UINT_PTR nIDEvent)
{
	std::string winName = "Result's Plot";
	if (!theApp.m().imageShow_inMFC(theApp.m().resultPlot, winName, 2.6)) {
		cv::destroyWindow(winName);
	}

	std::string winName1 = "Points's Position Change";
	if (!theApp.m().imageShow_inMFC(theApp.m().imPosi, winName1, 1)) {
		cv::destroyWindow(winName1);
	}


	CDialog::OnTimer(nIDEvent);
}


void measurements::OnBnClickedBCalculationflexipoints()
{

	UpdateData(true);

	theApp.m().calibrationFromIdealMirror = true; //21-Agust-2024

	theApp.m().extract_csvDepthPc(realtime, filterIndex);
	cv::Mat image = cv::imread(theApp.m().workingFolder + "Munia_Data\\Pics\\DepthImage.jpg");

	//MessageBox(_T("Please select the points by clicking your mouse. After selection of the points, please press any kye of your kyeboard"), _T("info"));

	std::string str = "Flexible_Pointsfor";
	theApp.m().selectPoint(1, right, 1, image, str);
	
	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	if (right) {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\"+str+"UR.csv", theApp.m().points_forU);
	}
	else {
		theApp.m().points_forU.clear();
		mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\"+str+"UL.csv", theApp.m().points_forU);
	}

		UpdateData(1);
		std::vector<double> vec;
		std::string strUser = CW2A(UserNote);


		theApp.m().extract_Radius(theApp.m().inputDepthImage, vec, right, strUser, 1, theApp.m().itemIndex);
		std::vector<int>l_index, u_index;
		int decision_index = theApp.m().checkingResults(vec, theApp.m().resultPlot, l_index, u_index, right, strUser);
		userNote->SetWindowTextW(_T(""));


		str = "Pointsfor";
		if (right) {
			theApp.m().points_forU.clear();
			mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\" + str + "UR.csv", theApp.m().points_forU);
		}
		else {
			theApp.m().points_forU.clear();
			mamun_util::pointsCsv2pointsVector(theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\" + str + "UL.csv", theApp.m().points_forU);
		}

		UpdateData(1);

}


void measurements::OnBnClickedButtonCalculation()
{


	UpdateData(1);
	if (!flag) {
		MessageBox(_T("Please open the camera"), _T("info"));
		return;
	}

	mProgressBarDlg.setProgPos(10, 2);

	CWnd *cwnd_B_calculation = (CWnd*)GetDlgItem(IDC_Button_Calculation);
	cwnd_B_calculation->SetWindowTextW(_T("Measuring..."));

	std::vector<cv::Point3d> tpoints;//21-Agust-2024
	double d=theApp.m().checkingMirrorPosition(theApp.m().points_forU, tpoints, right, realtime);//21-Agust-2024
	//mamun_util::twoPointVectorInterchange(theApp.m().points_forU, tpoints);//21-Agust-2024
	if (d == -361 || d==-999)
	{
		userNote->SetWindowTextW(_T(""));
		cwnd_B_calculation->SetWindowTextW(_T("Measure Radius"));
		mProgressBarDlg.DestroyWindow();
		return;
	}

	std::vector<double> vec;
	theApp.m().extract_csvDepthPc(realtime, filterIndex);
	mProgressBarDlg.setProgPos(10, 100);



	std::string strUser = CW2A(UserNote);

	theApp.m().extract_Radius(theApp.m().inputDepthImage, vec, right, strUser, 1, theApp.m().itemIndex);
	mProgressBarDlg.setProgPos(10, 100);

	//mamun_util::twoPointVectorInterchange(theApp.m().points_forU, tpoints); //21-Agust-2024


	std::ostringstream oss;
	for (double d : vec) {
		oss << d << " ";
	}
	CString str = CString(oss.str().c_str());

	if (mProgressBarDlg.m_hWnd != NULL) {
		Sleep(100);
		mProgressBarDlg.DestroyWindow();
	}

	std::vector<int>l_index, u_index;
	int decision_index = theApp.m().checkingResults(vec, theApp.m().resultPlot, l_index, u_index, right, strUser);

	std::ostringstream oss1;
	for (int i : l_index) {
		oss1 << i << " ";
	}
	CString str1 = CString(oss1.str().c_str());

	std::ostringstream oss2;
	for (int i : u_index) {
		oss2 << i << " ";
	}
	CString str2 = CString(oss2.str().c_str());



	//switch(decision_index)
	//{
	//case 1:
	//	MessageBox(str + _T(" \n Good Mirror"), _T("Radius"), MB_OK /*| MB_ICONQUESTION*/);
	//	break;
	//case 2:
	//	MessageBox(str+_T(" \n ")+str2+_T(" no points are NG 1"), _T("Radius"), MB_OK /*| MB_ICONQUESTION*/);
	//	break;
	//case 3:
	//	MessageBox(str + _T(" \n ") + str1+_T(" no points are NG 2"), _T("Radius"), MB_OK /*| MB_ICONQUESTION*/);
	//	break;
	//}


	userNote->SetWindowTextW(_T(""));

	cwnd_B_calculation->SetWindowTextW(_T("Measure Radius"));
}


void CMuniamfcDlg::OnTimer(UINT_PTR nIDEvent)
{
	countFortimeKilling = countFortimeKilling + 1;

		std::string str="Streaming";
		cv::Mat cv_bgrIm0, cv_bgrIm1;
		
		if (!theApp.m().extract_BGRIm(cv_bgrIm0, cv_bgrIm1, str)) {
			cv::destroyWindow(str);
		}

		std::string winName = "Result's Plot";
		if (!theApp.m().imageShow_inMFC(theApp.m().resultPlot, winName, 2.6)) {
			cv::destroyWindow(winName);
		}

		std::string winName1 = "Points's Position Change";
		if (!theApp.m().imageShow_inMFC(theApp.m().imPosi, winName1, 1)) {
			cv::destroyWindow(winName1);
		}


		theApp.m().plc_workingFn(autoCalculation);
	

	if (!autoCalculation)
	{
		
		cv::destroyAllWindows();
		if (flag) {
			theApp.m().closecamera();
			flag = false;
		}
		KillTimer(4);
		plcFlag = 0;

	}

	if (theApp.m().shutdownpc) 
	{
		cv::destroyAllWindows();
		if (flag){
			theApp.m().closecamera();
			flag = false;
		}

		EndDialog(ID_Exit);

		theApp.m().ShutdownSystem();
		KillTimer(4);
		plcFlag = 0;
	}


	CDialogEx::OnTimer(nIDEvent);
}


void CMuniamfcDlg::OnBnClickedCheckautomatic()
{
	autoCalculation = !autoCalculation;

	if (autoCalculation)
	{
		if (!flag)
		{
			bool cameraOpen = theApp.m().opencamera(0, 1, 1, 15, 15, 1, false, 1);
			if (cameraOpen)
			{
				flag = 1;
				SetTimer(4, 100, 0);
				plcFlag = 1;
			}
		}
	}
}


void CMuniamfcDlg::OnClose()
{
	autoCalculation = 0;

	if (flag) {
		theApp.m().closecamera();
		flag = 0;
	}


	if (plcFlag)
	{
		countFortimeKilling = 0;

		while (1) {

			MSG msg;
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			if (countFortimeKilling == 1) {
				break;
			}
		}
	}


	CDialogEx::OnClose();
}


void Ucalibraton::OnBnClickedButtoncheckoptimizationresults()
{
	UpdateData(1);
	std::string item;
	switch (theApp.m().itemIndex)
	{
	case 1:
		item = "YED";
		break;
	case 2:
		item = "YY8";
		break;
	}

	std::string str = theApp.m().workingFolder + "Munia_Data\\U\\"+item+"\\OptimumUR_";
	std::string str1;
	if (right) {str1 = str+"R.csv";}
	else { str1 = str+"L.csv"; }


	std::vector<cv::Point3d> optimumdata;
	csv2pointsVector(str1, optimumdata, 2, 3, 4);


	std::vector<int>ErrorPointsIndex;

	for (int i = 0; i < optimumdata.size(); i++) {

		
		if (optimumdata[i+1].x > 900) {
			ErrorPointsIndex.push_back(i+1);
		}
	}

	if (ErrorPointsIndex.size() == 0) {
		MessageBox(_T("All Point's Optimization are good"), _T("Info"));

	}
	else {

		std::ostringstream oss;
		for (int i : ErrorPointsIndex) {
			oss << i << " ";
		}
		CString str = CString(oss.str().c_str());
		MessageBox(str+_T(" no point's Optimization are not good"), _T("Info"));

	}

}


void svo_Recording::OnBnClickedButtonselectfolder()
{
	CFolderPickerDialog folderDialog;
	if (folderDialog.DoModal() == IDOK)
	{
		std::wstring fullPathname = folderDialog.GetPathName();

		mSvoPath0->SetWindowTextW(fullPathname.c_str());

	}
}




void svoReading::OnBnClickedButtonselectfolder()
{
	CFolderPickerDialog folderDialog;
	if (folderDialog.DoModal() == IDOK)
	{
		std::wstring fullPathname = folderDialog.GetPathName();

		pFilename0->SetWindowTextW(fullPathname.c_str());

	}
}


void Ucalibraton::OnBnClickedButtonselectfolder()
{
	CFolderPickerDialog folderDialog;
	if (folderDialog.DoModal() == IDOK)
	{
		std::wstring fullPathname = folderDialog.GetPathName();

		pFilename0->SetWindowTextW(fullPathname.c_str());

	}
}




void Ucalibraton::OnBnClickedButtoncameracalibration()
{
	UpdateData(1);

	bool fullHD = !resolution2K;
	bool option = !manualPointSelect;


	int ncol = _ttoi(edit_ncol);
	int nrow = _ttoi(edit_nrow);

	if (flag) {
		bool cameraclose = theApp.m().closecamera();
		flag = 0;
	}

	bool cameraOpen = theApp.m().opencamera(1, 1, 1, 18, 18, 1, resolution2K, false);
	flag = 1;

	step:


#if 01
	if (cameraOpen) 
	{
		bool aa=theApp.m().take_2_BGRIm(fullHD);
		
		if (!aa)
			return;
	}

	int result = MessageBoxW(_T("Okay?"), _T("Notice"), MB_YESNO);

	if (result != IDYES)
	{
		goto step;
	}



	std::string str0, str1;
	if (fullHD)
	{
		str0 = working_folder + "Munia_Data\\Camera_Calibration_Files\\cam0_HD.png";
		str1 = working_folder + "Munia_Data\\Camera_Calibration_Files\\cam1_HD.png";
	}
	else
	{
		str0 = working_folder + "Munia_Data\\Camera_Calibration_Files\\cam0_2k.png";
		str1 = working_folder + "Munia_Data\\Camera_Calibration_Files\\cam1_2k.png";
	}

	cv::Mat img0, img1;

	img0 = cv::imread(str0);
	img1 = cv::imread(str1);


#else
	img0=cv::imread("E:\\vs_projects\\munia_versions\\Munia_postIndian_YY8_version\\Munia\\calibration_data\\1080\\cam0.png");
	img1 = cv::imread("E:\\vs_projects\\munia_versions\\Munia_postIndian_YY8_version\\Munia\\calibration_data\\1080\\cam1.png");

	cv::imshow("1", img0);
	cv::imshow("2", img1);
	cv::waitKey(0);

#endif
	theApp.m().getHRT_matrix(img0, img1, fullHD, ncol, nrow, option);

	result = MessageBoxW(_T("Okay?"), _T("Notice"), MB_YESNO);

	if (result != IDYES)
	{
		goto step;
	}

}


void Ucalibraton::OnBnClickedCheckmanual()
{
	UpdateData(1);
	manualPointSelect = !manualPointSelect;
}


void Ucalibraton::OnBnClickedRHd1080()
{
	UpdateData(true);
	resolution2K = false;
	theApp.m().transformation_matrix(resolution2K);
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void Ucalibraton::OnBnClickedRHd2k()
{
	UpdateData(true);
	resolution2K = true;
	theApp.m().transformation_matrix(resolution2K);
	if (flag) {
		theApp.m().closecamera();
		flag = false;
	}
}


void Ucalibraton::OnBnClickedRadioYed()
{
	theApp.m().itemIndex = 1;
	theApp.m().transformation_matrix(resolution2K);
	UpdateData(1);
}


void Ucalibraton::OnBnClickedRadioYy8()
{
	theApp.m().itemIndex = 2;
	theApp.m().transformation_matrix(resolution2K);
	UpdateData(1);
}


void m_settings::OnBnClickedRadioYed()
{
	isCheckedSettings = 1;
	theApp.m().itemIndex = 1;
	theApp.m().transformation_matrix(resolution2K);

	//theApp.m().lowerLimit = 1200;
	//theApp.m().upperLimit = 1260;
	//theApp.m().upperLimit2 = 1400;


	UpdateData(1);
}


void m_settings::OnBnClickedRadioYy8()
{
	isCheckedSettings = 1;
	theApp.m().itemIndex = 2;
	theApp.m().transformation_matrix(resolution2K);

	//theApp.m().lowerLimit = 1200;
	//theApp.m().upperLimit = 1290;
	//theApp.m().upperLimit2 = 1400;

	UpdateData(1);
}


void m_settings::OnBnClickedidealpositionmirror()
{
	theApp.m().idealPositionOFmirror = !theApp.m().idealPositionOFmirror;

}





void m_settings::OnBnClickedRadio9points()
{
	theApp.m().judge_by_ec_only = 0;
}


void m_settings::OnBnClickedRadio3points()
{
	theApp.m().judge_by_ec_only = 1;
}
