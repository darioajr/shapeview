#include "stdafx.h"
#include "GIS.SHPThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

using MapGIS::ShapeRecord;
using MapGIS::ShapeType;
using MapGIS::ShapePoint;
using MapGIS::ShapePolygon;

IMPLEMENT_DYNAMIC(SHPThread, GDIThread)

BEGIN_MESSAGE_MAP(SHPThread, CCmdTarget)
	//{{AFX_MSG_MAP(SHPThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

SHPThread::SHPThread(CWnd *pWnd, HDC hDC, CPen *pPen, CPen *pPenSelected, CBrush *pBrush, MapMetrics *pMapMetrics, SHPVector *pVector) : GDIThread(pWnd, hDC)
{
	m_pMapMetrics	= pMapMetrics;
	m_pVector		= pVector;
	m_phPen			= pPen;
	m_phPenSelected	= pPenSelected;
	m_phBrush		= pBrush;
}

BOOL SHPThread::InitInstance()
{
	return GDIThread::InitInstance();
}

void SHPThread::SingleStep()
{
	// Since all threads share the same HDC it is necessary
	// to block all other threads while we render in the HDC
//	EnterCriticalSection(&GDIThread::m_csGDILock);
	{
		POSITION POS1	 = NULL;
		POSITION POS2	 = NULL;
		POSITION POS3	 = NULL;
		
		m_dc.SetBkMode(TRANSPARENT);
		
		ShapeRecord *pRecord = NULL;
		
		POS1 = m_pVector->Records.GetHeadPosition();
		for (;POS1 != NULL;)
		{
			if(WaitForSingleObject(m_hEventKill, 0) != WAIT_TIMEOUT)
				break;

			pRecord = m_pVector->Records.GetNext(POS1);
			
			switch(pRecord->m_ShapeType)
			{
			case 0:
				break;
			case 1:
				{
					POINT pt = m_pMapMetrics->World2Pixel(((ShapePoint*)pRecord->m_Data)->point->X,((ShapePoint*)pRecord->m_Data)->point->Y);
					m_dc.SetPixel(pt,RGB(0,0,0));
				}
				break;
			case 5:
				{
					POS2 = ((ShapePolygon*)pRecord->m_Data)->m_parts.GetHeadPosition();
					if(POS2 != NULL)
					{
						DWORD dwConta = 0;
						dwConta = ((ShapePolygon*)pRecord->m_Data)->m_points.GetCount();
						int i = 0;
						POINT *pPoint = ::new POINT[dwConta+1];
						POS3 = ((ShapePolygon*)pRecord->m_Data)->m_points.GetHeadPosition();
						for (;POS3 != NULL;i++)
						{
							PointD  *p = ((ShapePolygon*)pRecord->m_Data)->m_points.GetNext(POS3);
							pPoint[i] = m_pMapMetrics->World2Pixel(p->X,p->Y);
							TRACE("xy(%d,%d)\n",pPoint[i].x,pPoint[i].y);
						}
						
						CPen* pOldPen = NULL;
						pOldPen = pRecord->m_Selected ? m_dc.SelectObject(m_phPenSelected) : m_dc.SelectObject(m_phPen);

						//CPen* pOldPen = m_dc.SelectObject( m_phPen );
						CBrush* pBrush = m_dc.SelectObject( m_phBrush );
						
						m_dc.Polygon(pPoint,dwConta);
						
						m_dc.SelectObject( pOldPen );
						m_dc.SelectObject( pBrush );
						
						::delete []pPoint;
					}
				}
				break;
			}
		}
		// Win32 optimizes GDI calls by collecting them in a batch
		// and then thunking the whole batch at once on a per
		// thread basis.  Since we share an HDC with multiple
		// threads, we must flush the batch before yielding to
		// other threads that will adjust the HDC.  To see what
		// I mean, comment out the GdiFlush() function call and
		// watch the results.
		GdiFlush();

		if(IsWindowVisible(m_pMainWnd->m_hWnd))
			PostMessage(m_pMainWnd->m_hWnd,WMU_EXITPAINT,0,0);
			//m_pMainWnd->SendMessage(WMU_EXITPAINT,0,0);
	}
	//LeaveCriticalSection(&GDIThread::m_csGDILock);
}
