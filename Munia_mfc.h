
// Munia_mfc.h : main header file for the PROJECT_NAME application
//

#pragma once


#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "munia_functions.h"

// CMuniamfcApp:
// See Munia_mfc.cpp for the implementation of this class
//

class CMuniamfcApp : public CWinApp
{
public:
	CMuniamfcApp();
	inline munia_function_class& m() { return F_munia_object; }



private:
	munia_function_class F_munia_object;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMuniamfcApp theApp;
