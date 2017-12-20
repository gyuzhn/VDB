// TaskDesign.h : main header file for the TaskDesign application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTaskDesignApp:
// See TaskDesign.cpp for the implementation of this class
//

class CTaskDesignApp : public CWinApp
{
public:
	CTaskDesignApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTaskDesignApp theApp;