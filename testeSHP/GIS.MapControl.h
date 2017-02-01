#if !defined(_MAPCONTROL_)
#define _MAPCONTROL_

#include "stdafx.h"

#include "GIS.Map.h"
#include "GIS.Message.h"

using MapGIS::Map;

namespace MapGIS
{
	typedef enum PointerMode { None, Select, Zoom, ZoomArea, Center, Query };

	class MapControl : public CWnd
	{	
	public:
		MapControl();
		DECLARE_DYNCREATE(MapControl)
		virtual ~MapControl();
		
	protected:
		Map *_map;
		
		double _MouseGeoX;
		double _MouseGeoY;
		int _MouseActionBeginX;
		int _MouseActionBeginY;
		CPoint _MouseActionBeginScreen;
		bool _ButtonDown;
		PointerMode _PointerMode;
		int _queryLayer;
		CRect m_rectDraw;
		BOOL m_bFlickerFree;
		COLORREF m_BkColor;

		CDC* m_buffer;
		CBitmap* m_bitmap;
		int m_width, m_height;

		DWORD m_nTimer;
		CString m_sPipeFields;

		

	protected:
		DECLARE_MESSAGE_MAP()

		virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnTimer(UINT nIDEvent) ;
		
	private:
		void Init();
		void Clean();
		void Repaint();
		void OnStartTimer();
		void OnStopTimer() ;
		void DrawZoom(HDC hDC, CRect rc);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanvas)
	protected:
		virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL


	public:
		void SetFlickerFree(BOOL bFlick);
		void Draw(CDC *pDC = NULL);
		void AddLayer( CString filename );
		void RemoveLayer( int index );
		void MoveLayerUp( int index );
		void MoveLayerDown( int index );
		void SaveViewToImage(CString Filename);
		void QueryPoint( int x, int y );
		void SetBkColor(COLORREF color);
		void SetBrushColor(COLORREF color);
		void SetPenWidth(int size);
		COLORREF GetBkColor();
		BOOL Attach( CWnd *pWnd, UINT ID );
		void ZoomWindow(RectangleD &rect);
		void ZoomExtents();
		void Command(PointerMode uiCommand);
		void SelectObject(DWORD Handle);
		void ZoomObject(DWORD Handle);
		void ClearSelected();
		CString QueryObject(DWORD Handle);
		void Invalidate();
		void SetQueryFields(CString PipeFields);
		void SetLayerActive(DWORD index);
	};
}

#endif //_MAPCONTROL_