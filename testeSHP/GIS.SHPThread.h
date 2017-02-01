#if !defined(_SHPTHREAD_)
#define _SHPTHREAD_

#include "GIS.GDIThread.h"
#include "GIS.SHPReader.h"
#include "GIS.MapMetrics.h"

using MapGIS::MapMetrics;
using MapGIS::SHPVector;

class SHPThread : public GDIThread
{
public:
	DECLARE_DYNAMIC(SHPThread)
	SHPThread(CWnd *pWnd, HDC hDC, CPen *pPen, CPen *pPenSelected, CBrush *pBrush, MapMetrics *pMapMetrics, SHPVector *pVector);

// Attributes
public:
	CRect m_rectPosition;

	MapMetrics *m_pMapMetrics;
	SHPVector *m_pVector;

	CPen* m_phPen;
	CPen* m_phPenSelected;
	CBrush* m_phBrush;

// Operations
public:
	virtual BOOL InitInstance();
	virtual void SingleStep();

// Overrides
	//{{AFX_VIRTUAL(SHPThread)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~SHPThread() {};

	// Generated message map functions
	//{{AFX_MSG(SHPThread)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif //_SHPTHREAD_