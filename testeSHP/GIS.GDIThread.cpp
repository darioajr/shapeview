#include "stdafx.h"
#include "GIS.GDIThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// critical section to protect while drawing to the DC
CRITICAL_SECTION GDIThread::m_csGDILock;

// m_hAnotherDead is used to signal that one or more threads have ended
//  (it is an auto-reset event; and starts out not signaled)
HANDLE GDIThread::m_hAnotherDead = CreateEvent(NULL, FALSE, FALSE, NULL);

/////////////////////////////////////////////////////////////////////////////
// CGDIThread

IMPLEMENT_DYNAMIC(GDIThread, CWinThread)

BEGIN_MESSAGE_MAP(GDIThread, CWinThread)
	//{{AFX_MSG_MAP(GDIThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

GDIThread::GDIThread(CWnd* pWnd, HDC hDC)
{
	m_bAutoDelete = FALSE;
	m_pMainWnd = pWnd;
	m_pMainWnd->GetClientRect(&m_rectBorder);
	m_hDC = hDC;

	// kill event starts out in the signaled state
	m_hEventKill = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);
}

// The reason we don't just get a CDC from our owner and simply use it is because
// MFC GDI objects can't be passed between threads.  So we must instead pass a
// handle and then reconvert them back to MFC objects.  The reason  for this is
// because MFC maintains a list of all GDI objects on a per thread basis.  So if
// you pass a GDI object to another thread, it won't be in the correct list
// and MFC will assert.

BOOL GDIThread::InitInstance()
{
	// thread setup
	m_dc.Attach(m_hDC);

	// loop but check for kill notification
	// dario
	//while (WaitForSingleObject(m_hEventKill, 0) == WAIT_TIMEOUT)
	if(IsWindowVisible(m_pMainWnd->m_hWnd))
		//m_pMainWnd->SendMessage(WMU_INITPAINT,0,0);
		PostMessage(m_pMainWnd->m_hWnd,WMU_INITPAINT,0,0);
	
	SingleStep();

	// avoid entering standard message loop by returning FALSE
	return FALSE;
}

BOOL GDIThread::ExitInstance()
{
	// thread cleanup
	m_dc.Detach();

	return TRUE;
}

void GDIThread::Delete()
{
	// calling the base here won't do anything but it is a good habit
	CWinThread::Delete();

	// acknowledge receipt of kill notification
	VERIFY(SetEvent(m_hEventDead));
	VERIFY(SetEvent(m_hAnotherDead));
}

GDIThread::~GDIThread()
{
	CloseHandle(m_hEventKill);
	CloseHandle(m_hEventDead);
}

void GDIThread::KillThread()
{
	// Note: this function is called in the context of other threads,
	//  not the thread itself.

	// reset the m_hEventKill which signals the thread to shutdown
	VERIFY(SetEvent(m_hEventKill));

	// allow thread to run at higher priority during kill process
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	WaitForSingleObject(m_hEventDead, INFINITE);
	WaitForSingleObject(m_hThread, INFINITE);

	// now delete CWinThread object since no longer necessary
	delete this;
}

void GDIThread::UpdateBorder()
{
	// Since another thread can access this variable, we block them when we write
	// it. In this case, it is overkill since the other thread would just animate
	// in the wrong direction while reading intermediate values and finally adjust
	// itself when the new value is valid.  It is a good idea to avoid such
	// anomalies, however.

	EnterCriticalSection(&GDIThread::m_csGDILock);
	{
		if(IsWindowVisible(m_pMainWnd->m_hWnd))
			m_pMainWnd->GetClientRect(&m_rectBorder);
	}
	LeaveCriticalSection(&GDIThread::m_csGDILock);
}
