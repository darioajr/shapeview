// testeSHP.h : main header file for the TESTESHP application
//

#if !defined(AFX_TESTESHP_H__F96F647C_B38B_4122_86F4_132EF741F9E1__INCLUDED_)
#define AFX_TESTESHP_H__F96F647C_B38B_4122_86F4_132EF741F9E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTesteSHPApp:
// See testeSHP.cpp for the implementation of this class
//

class CTesteSHPApp : public CWinApp
{
public:
	CTesteSHPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTesteSHPApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTesteSHPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTESHP_H__F96F647C_B38B_4122_86F4_132EF741F9E1__INCLUDED_)
