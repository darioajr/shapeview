#if !defined(_GDITHREAD_)
#define _GDITHREAD_

#include "StdAfx.h"

#include "GIS.Message.h"

class GDIThread : public CWinThread
{
public:
	DECLARE_DYNAMIC(GDIThread)
	GDIThread(CWnd* pWnd, HDC hDC);

// Attributes
public:
	CRect m_rectBorder;
	HDC m_hDC;
	CDC m_dc;
	HANDLE m_hEventKill;
	HANDLE m_hEventDead;
	static HANDLE m_hAnotherDead;

	static CRITICAL_SECTION m_csGDILock;

// Operations
public:
	void KillThread();
	void UpdateBorder();
	virtual void SingleStep() = 0;

// Overrides
	//{{AFX_VIRTUAL(GDIThread)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GDIThread();
	virtual void Delete();

protected:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

	// Generated message map functions
	//{{AFX_MSG(GDIThread)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif //GDITHREAD