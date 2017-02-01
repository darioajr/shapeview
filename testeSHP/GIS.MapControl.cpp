#include "stdafx.h"
#include "DIB.h"
#include "GIS.MapControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using MapGIS::MapControl;
using MapGIS::PointerMode;

#define EVENT_TIMER_DRAW 1
#define TIMER_REDRAW 250

MapControl::MapControl() : _map(NULL),m_width(0), m_height(0), m_bitmap(NULL), m_buffer(NULL),
  m_bFlickerFree(true)

{
	_map			= new Map();
	m_BkColor		= RGB(255,255,255);
	_queryLayer		= -1;
	_PointerMode	= None;
}

BOOL MapControl::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

int MapControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Init();

	return 0;
}

BOOL MapControl::Attach( CWnd *pWnd, UINT ID )
{
	CRect rc;
	CWnd *ctrl = (CWnd *)pWnd->GetDlgItem(ID);
	ctrl->GetWindowRect(&rc);
	ctrl->DestroyWindow();

	return Create((LPCTSTR)NULL,(LPCTSTR)_T(""),WS_VISIBLE|WS_CHILD|WS_BORDER|WS_CLIPCHILDREN|WS_TABSTOP,rc,pWnd,ID,(CCreateContext*)NULL);
}

MapControl::~MapControl()
{
	if(_map)
		delete _map;

	Clean();
}

void MapControl::Clean()
{
	if (m_buffer != NULL)
	{
		delete m_buffer;
		m_buffer = NULL;
	}

	if (m_bitmap != NULL)
	{
		delete m_bitmap;
		m_bitmap = NULL;
	}
}

IMPLEMENT_DYNCREATE(MapControl, CWnd)

BEGIN_MESSAGE_MAP(MapControl, CWnd)
	//{{AFX_MSG_MAP(MapControl)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE( WMU_INITPAINT, OnStartTimer )
	ON_MESSAGE( WMU_EXITPAINT, OnStopTimer )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void MapControl::PreSubclassWindow() 
{	
	CWnd::PreSubclassWindow();
}

void MapControl::Init()
{		
	CRect rect;
	GetClientRect(rect);

	m_width = rect.Width();
	m_height = rect.Height();

	m_rectDraw = rect;
	
	_map->_mapMetrics->_canvas = m_rectDraw;

 	CClientDC dc(this);

	Clean();

	if(m_bitmap == NULL)
	{
		m_bitmap = new CBitmap();
		m_bitmap->CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	}

	if(m_buffer == NULL)
	{
		m_buffer = new CDC();
		m_buffer->CreateCompatibleDC(&dc);
		m_buffer->SelectObject(m_bitmap);
	}

	Repaint();
}

void MapControl::Repaint()
{
	if( m_bFlickerFree )
	{
		CClientDC dc(this);
		CRect rect;
		GetClientRect(rect);

		m_buffer->FillSolidRect(rect,GetBkColor());

		Draw(m_buffer);

		dc.BitBlt(0,0,rect.Width(),rect.Height(),m_buffer,0,0,SRCCOPY);
	}
	else
	{
		Invalidate();
	}
}

void MapControl::OnStartTimer() 
{
   m_nTimer = SetTimer(EVENT_TIMER_DRAW, TIMER_REDRAW, 0);
}

void MapControl::OnStopTimer() 
{
   KillTimer(m_nTimer);   

	if( m_bFlickerFree )
	{
		CClientDC dc(this);
		CRect rect;
		GetClientRect(rect);

		dc.BitBlt(0,0,rect.Width(),rect.Height(),m_buffer,0,0,SRCCOPY);
	}
}

void MapControl::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == EVENT_TIMER_DRAW)
	if( m_bFlickerFree )
	{
		CClientDC dc(this);
		CRect rect;
		GetClientRect(rect);

		dc.BitBlt(0,0,rect.Width(),rect.Height(),m_buffer,0,0,SRCCOPY);
	}

	CWnd::OnTimer(nIDEvent);
}

void MapControl::SetFlickerFree(BOOL bFlick)
{
	m_bFlickerFree = bFlick;
}

BOOL MapControl::OnEraseBkgnd(CDC* pDC) 
{
	if(m_bFlickerFree)
		return TRUE;
	else
		return CWnd::OnEraseBkgnd(pDC);
}

void MapControl::SetBkColor(COLORREF color)
{
	m_BkColor = color;
	Repaint();
}

COLORREF MapControl::GetBkColor()
{
	return m_BkColor;
}

void MapControl::ZoomExtents()
{
	_map->ZoomExtents();
	Repaint();
}

void MapControl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	switch( _PointerMode )
	{
	case None:
		break;

	case Select:
		break;
					
	case Zoom:
		break;
					
	case ZoomArea:
		_MouseActionBeginX = point.x;
		_MouseActionBeginY = point.y;
		_ButtonDown = true;

		::SetCapture(this->m_hWnd);
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));

		break;
					
	case Center:
		_map->Center( point.x, point.y );
		Repaint();
		break;
					
	case Query:
		break;
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void MapControl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	switch( _PointerMode )
	{
	case None:
		break;

	case Select:
		break;
			
	case Zoom:					
		if ( nFlags != MK_CONTROL )
		{
			_map->ZoomIn( point.x, point.y );
			Repaint();
		} 
		else 
		{
			_map->ZoomOut( point.x, point.y );
			Repaint();
		}
		break;
			
	case ZoomArea:
		{
			::ReleaseCapture();
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));

			_map->ZoomArea( min(_MouseActionBeginX,point.x), min(_MouseActionBeginY,point.y), max(point.x,_MouseActionBeginX), max(point.y,_MouseActionBeginY));

			Repaint();

			_PointerMode = None;
		}
		break;

	case Center:
		break;
			
	case Query:
		//if ( nFlags != MK_CONTROL )
		//{
			QueryPoint( point.x, point.y );
			Repaint();
		//} 
		break;
	}

	_ButtonDown = false;
	

	CWnd::OnLButtonUp(nFlags, point);
}

void MapControl::OnMouseMove(UINT nFlags, CPoint point) 
{
	double tempx = (double) point.x;
	double tempy = (double) point.y;
	
	if (_map->GetCount() > 0)
	_map->getMetrics()->Pixel2World( tempx, tempy );

	_MouseGeoX = tempx;
	_MouseGeoY = tempy;

	::SendMessage(GetParent()->m_hWnd,WMU_COORDINATE,(DWORD)&tempx,(DWORD)&tempy);

	if (::GetCapture() == this->m_hWnd) 
	{
		if(_PointerMode == None)
			return;
		
		switch( _PointerMode )
		{
		case Select:
			break;
			
		case Zoom:
			break;
			
		case ZoomArea:
			if ( _ButtonDown == true )
				DrawZoom(m_buffer->m_hDC,CRect(_MouseActionBeginX,_MouseActionBeginY,point.x,point.y));
			break;
			
		case Center:
			break;
			
		case Query:
			break;
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void MapControl::OnPaint() 
{
	CPaintDC dc(this);

	::SetBkMode(dc,TRANSPARENT);

	CRect rect;
	GetClientRect(rect);

	if( m_bFlickerFree )
		dc.BitBlt(0,0,rect.Width(),rect.Height(),m_buffer,0,0,SRCCOPY);
	else
	{
		CRgn rgn;
		rgn.CreateRectRgnIndirect(rect);

		dc.SelectClipRgn(&rgn);

		dc.FillSolidRect(rect,GetBkColor());

		Draw(&dc);
	}
}

void MapControl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	Init();
	
}

void MapControl::Draw(CDC *pDC)
{
	if ( _map->GetCount() > 0 )
	{
		_map->Draw(this, pDC);				
	}
}
	
void MapControl::AddLayer( CString filename )
{			
	BeginWaitCursor();
	_map->Add( filename,m_rectDraw );
	EndWaitCursor();

	Repaint();
}

void MapControl::SetLayerActive(DWORD index)
{
	if ( index >= 0 && index < _map->GetCount() )
	{
		_queryLayer = index;
		_map->SetLayerActive(index);
		Repaint();
	}
}

void MapControl::RemoveLayer( int index )
{
	if ( index > 0 && index < _map->GetCount() )
	{
		_map->Remove( index );
		Repaint();
	}
}

void MapControl::MoveLayerUp( int index )
{
	if ( _map->GetCount() > 0 && index != 0 )
	{
		_map->Insert( index - 1, _map->GetLayer(index) );
		_map->Remove( index + 1 );
		Repaint();
	}
}

void MapControl::MoveLayerDown( int index )
{
	if ( _map->GetCount() > 0 && index != _map->GetCount() - 1)
	{
		_map->Insert( index + 2, _map->GetLayer(index) );
		_map->Remove( index );
		Repaint();
	}
}
		
void MapControl::SaveViewToImage(CString Filename)
{
	CDIB dib;
	dib.CreateFromHandle(m_bitmap,24);
	dib.SaveDIB(Filename,CDIB::BMP);
}
		
void MapControl::QueryPoint( int x, int y )
{
	if ( _queryLayer != -1 )
	{
		IMapLayer *ml = _map->GetLayer(_queryLayer);
		CString result = ml->QueryPoint( x, y, m_sPipeFields );
		if(result.GetLength() > 0)
			::SendMessage(GetParent()->m_hWnd,WMU_QUERYRESULT,0,(DWORD)&result);
	}
	else
	{
		for(int i=0;i<_map->GetCount();i++)
		{
			IMapLayer *ml = _map->GetLayer(i);
			CString result = ml->QueryPoint( x, y, m_sPipeFields );
			if(result.GetLength() > 0)
				::SendMessage(GetParent()->m_hWnd,WMU_QUERYRESULT,0,(DWORD)&result);
		}
	}
}

CString MapControl::QueryObject(DWORD Handle)
{
	CString result;
	if ( _queryLayer != -1 )
	{
		IMapLayer *ml = _map->GetLayer(_queryLayer);
		if(ml)
			result = ml->QueryObject(Handle);
	}

	return result;
}

void MapControl::ZoomWindow(RectangleD &rect)
{
	_map->ZoomArea( rect );

	Repaint();
}

void MapControl::DrawZoom(HDC hDC, CRect rc)
{
	int i = m_buffer->SaveDC();
	m_buffer->SetROP2(R2_NOTXORPEN);

	CPen hPen(PS_SOLID,1,RGB(0,0,0));
	CPen* pOldPen = m_buffer->SelectObject( &hPen );
	
	m_buffer->SetBkMode(TRANSPARENT);
	m_buffer->Rectangle(&rc);

	m_buffer->SelectObject(pOldPen);
	::DeleteObject(hPen);

	m_buffer->RestoreDC(-1);
	Invalidate();
} 

void MapControl::Command(PointerMode uiCommand)
{
	_PointerMode = uiCommand;
}

void MapControl::SelectObject(DWORD Handle)
{
	_map->SelectObject(Handle);
	Repaint();
}

void MapControl::ClearSelected()
{
	_map->ClearSelected();
	Repaint();
}

void MapControl::ZoomObject(DWORD Handle)
{
	_map->ZoomObject(Handle);
	Repaint();
}

void MapControl::Invalidate()
{
	Repaint();
}

void MapControl::SetBrushColor(COLORREF color)
{

}

void MapControl::SetPenWidth(int size)
{

}

void MapControl::SetQueryFields(CString PipeFields)
{
	m_sPipeFields = PipeFields;
}