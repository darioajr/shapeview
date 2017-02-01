#if !defined(_MAP_)
#define _MAP_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "GIS.Mapmetrics.h"
#include "GIS.IMapLayer.h"

//AutoCad Color Index (DXF)
const COLORREF ACI[] =
{
	0,
	RGB(255 ,  0 ,0  ),
	RGB(255 ,255 ,0  ),
	RGB(  0 ,255 ,0  ),
	RGB(  0 ,255 ,255),
	RGB(  0 ,  0 ,255),
	RGB(255 ,  0 ,255),
	RGB(  0 ,  0 ,0  ),
	RGB(128 ,128 ,128),
	RGB(192 ,192 ,192),
	RGB(255 ,  0 ,0  ),
	RGB(255 ,127 ,127),
	RGB(165 ,  0 ,0  ),
	RGB(165 , 82 ,82 ),
	RGB(127 ,  0 ,0  ),
	RGB(127 , 63 ,63 ),
	RGB( 76 ,  0 ,0  ),
	RGB( 76 , 38 ,38 ),
	RGB( 38 ,  0 ,0  ),
	RGB( 38 , 19 ,19 ),
	RGB(255 , 63 ,0  ),
	RGB(255 ,159 ,127),
	RGB(165 , 41 ,0  ),
	RGB(165 ,103 ,82 ),
	RGB(127 , 31 ,0  ),
	RGB(127 , 79 ,63 ),
	RGB( 76 , 19 ,0  ),
	RGB( 76 , 47 ,38 ),
	RGB( 38 ,  9 ,0  ),
	RGB( 38 , 23 ,19 ),
	RGB(255 ,127 ,0  ),
	RGB(255 ,191 ,127),
	RGB(165 , 82 ,0  ),
	RGB(165 ,124 ,82 ),
	RGB(127 , 63 ,0  ),
	RGB(127 , 95 ,63 ),
	RGB( 76 , 38 ,0  ),
	RGB( 76 , 57 ,38 ),
	RGB( 38 , 19 ,0  ),
	RGB( 38 , 28 ,19 ),
	RGB(255 ,191 ,0  ),
	RGB(255 ,223 ,127),
	RGB(165 ,124 ,0  ),
	RGB(165 ,145 ,82 ),
	RGB(127 , 95 ,0  ),
	RGB(127 ,111 ,63 ),
	RGB( 76 , 57 ,0  ),
	RGB( 76 , 66 ,38 ),
	RGB( 38 , 28 ,0  ),
	RGB( 38 , 33 ,19 ),
	RGB(255 ,255 ,0  ),
	RGB(255 ,255 ,127),
	RGB(165 ,165 ,0  ),
	RGB(165 ,165 ,82 ),
	RGB(127 ,127 ,0  ),
	RGB(127 ,127 ,63 ),
	RGB( 76 , 76 ,0  ),
	RGB( 76 , 76 ,38 ),
	RGB( 38 , 38 ,0  ),
	RGB( 38 , 38 ,19 ),
	RGB(191 ,255 ,0  ),
	RGB(223 ,255 ,127),
	RGB(124 ,165 ,0  ),
	RGB(145 ,165 ,82 ),
	RGB( 95 ,127 ,0  ),
	RGB(111 ,127 ,63 ),
	RGB( 57 , 76 ,0  ),
	RGB( 66 , 76 ,38 ),
	RGB( 28 , 38 ,0  ),
	RGB( 33 , 38 ,19 ),
	RGB(127 ,255 ,0  ),
	RGB(191 ,255 ,127),
	RGB( 82 ,165 ,0  ),
	RGB(124 ,165 ,82 ),
	RGB( 63 ,127 ,0  ),
	RGB( 95 ,127 ,63 ),
	RGB( 38 , 76 ,0  ),
	RGB( 57 , 76 ,38 ),
	RGB( 19 , 38 ,0  ),
	RGB( 28 , 38 ,19 ),
	RGB( 63 ,255 ,0  ),
	RGB(159 ,255 ,127),
	RGB( 41 ,165 ,0  ),
	RGB(103 ,165 ,82 ),
	RGB( 31 ,127 ,0  ),
	RGB( 79 ,127 ,63 ),
	RGB( 19 , 76 ,0  ),
	RGB( 47 , 76 ,38 ),
	RGB(  9 , 38 ,0  ),
	RGB( 23 , 38 ,19 ),
	RGB(  0 ,255 ,0  ),
	RGB(127 ,255 ,127),
	RGB(  0 ,165 ,0  ),
	RGB( 82 ,165 ,82 ),
	RGB(  0 ,127 ,0  ),
	RGB( 63 ,127 ,63 ),
	RGB(  0 , 76 ,0  ),
	RGB( 38 , 76 ,38 ),
	RGB(  0 , 38 ,0  ),
	RGB( 19 , 38 ,19 ),
	RGB(  0 ,255 ,63 ),
	RGB(127 ,255 ,159),
	RGB(  0 ,165 ,41 ),
	RGB( 82 ,165 ,103),
	RGB(  0 ,127 ,31 ),
	RGB( 63 ,127 ,79 ),
	RGB(  0 , 76 ,19 ),
	RGB( 38 , 76 ,47 ),
	RGB(  0 , 38 ,9  ),
	RGB( 19 , 38 ,23 ),
	RGB(  0 ,255 ,127),
	RGB(127 ,255 ,191),
	RGB(  0 ,165 ,82 ),
	RGB( 82 ,165 ,124),
	RGB(  0 ,127 ,63 ),
	RGB( 63 ,127 ,95 ),
	RGB(  0 , 76 ,38 ),
	RGB( 38 , 76 ,57 ),
	RGB(  0 , 38 ,19 ),
	RGB( 19 , 38 ,28 ),
	RGB(  0 ,255 ,191),
	RGB(127 ,255 ,223),
	RGB(  0 ,165 ,124),
	RGB( 82 ,165 ,145),
	RGB(  0 ,127 ,95 ),
	RGB( 63 ,127 ,111),
	RGB(  0 , 76 ,57 ),
	RGB( 38 , 76 ,66 ),
	RGB(  0 , 38 ,28 ),
	RGB( 19 , 38 ,33 ),
	RGB(  0 ,255 ,255),
	RGB(127 ,255 ,255),
	RGB(  0 ,165 ,165),
	RGB( 82 ,165 ,165),
	RGB(  0 ,127 ,127),
	RGB( 63 ,127 ,127),
	RGB(  0 , 76 ,76 ),
	RGB( 38 , 76 ,76 ),
	RGB(  0 , 38 ,38 ),
	RGB( 19 , 38 ,38 ),
	RGB(  0 ,191 ,255),
	RGB(127 ,223 ,255),
	RGB(  0 ,124 ,165),
	RGB( 82 ,145 ,165),
	RGB(  0 , 95 ,127),
	RGB( 63 ,111 ,127),
	RGB(  0 , 57 ,76 ),
	RGB( 38 , 66 ,76 ),
	RGB(  0 , 28 ,38 ),
	RGB( 19 , 33 ,38 ),
	RGB(  0 ,127 ,255),
	RGB(127 ,191 ,255),
	RGB(  0 , 82 ,165),
	RGB( 82 ,124 ,165),
	RGB(  0 , 63 ,127),
	RGB( 63 , 95 ,127),
	RGB(  0 , 38 ,76 ),
	RGB( 38 , 57 ,76 ),
	RGB(  0 , 19 ,38 ),
	RGB( 19 , 28 ,38 ),
	RGB(  0 , 63 ,255),
	RGB(127 ,159 ,255),
	RGB(  0 , 41 ,165),
	RGB( 82 ,103 ,165),
	RGB(  0 , 31 ,127),
	RGB( 63 , 79 ,127),
	RGB(  0 , 19 ,76 ),
	RGB( 38 , 47 ,76 ),
	RGB(  0 ,  9 ,38 ),
	RGB( 19 , 23 ,38 ),
	RGB(  0 ,  0 ,255),
	RGB(127 ,127 ,255),
	RGB(  0 ,  0 ,165),
	RGB( 82 , 82 ,165),
	RGB(  0 ,  0 ,127),
	RGB( 63 , 63 ,127),
	RGB(  0 ,  0 ,76 ),
	RGB( 38 , 38 ,76 ),
	RGB(  0 ,  0 ,38 ),
	RGB( 19 , 19 ,38 ),
	RGB( 63 ,  0 ,255),
	RGB(159 ,127 ,255),
	RGB( 41 ,  0 ,165),
	RGB(103 , 82 ,165),
	RGB( 31 ,  0 ,127),
	RGB( 79 , 63 ,127),
	RGB( 19 ,  0 ,76 ),
	RGB( 47 , 38 ,76 ),
	RGB(  9 ,  0 ,38 ),
	RGB( 23 , 19 ,38 ),
	RGB(127 ,  0 ,255),
	RGB(191 ,127 ,255),
	RGB( 82 ,  0 ,165),
	RGB(124 , 82 ,165),
	RGB( 63 ,  0 ,127),
	RGB( 95 , 63 ,127),
	RGB( 38 ,  0 ,76 ),
	RGB( 57 , 38 ,76 ),
	RGB( 19 ,  0 ,38 ),
	RGB( 28 , 19 ,38 ),
	RGB(191 ,  0 ,255),
	RGB(223 ,127 ,255),
	RGB(124 ,  0 ,165),
	RGB(145 , 82 ,165),
	RGB( 95 ,  0 ,127),
	RGB(111 , 63 ,127),
	RGB( 57 ,  0 ,76 ),
	RGB( 66 , 38 ,76 ),
	RGB( 28 ,  0 ,38 ),
	RGB( 33 , 19 ,38 ),
	RGB(255 ,  0 ,255),
	RGB(255 ,127 ,255),
	RGB(165 ,  0 ,165),
	RGB(165 , 82 ,165),
	RGB(127 ,  0 ,127),
	RGB(127 , 63 ,127),
	RGB( 76 ,  0 ,76 ),
	RGB( 76 , 38 ,76 ),
	RGB( 38 ,  0 ,38 ),
	RGB( 38 , 19 ,38 ),
	RGB(255 ,  0 ,191),
	RGB(255 ,127 ,223),
	RGB(165 ,  0 ,124),
	RGB(165 , 82 ,145),
	RGB(127 ,  0 ,95 ),
	RGB(127 , 63 ,111),
	RGB( 76 ,  0 ,57 ),
	RGB( 76 , 38 ,66 ),
	RGB( 38 ,  0 ,28 ),
	RGB( 38 , 19 ,33 ),
	RGB(255 ,  0 ,127),
	RGB(255 ,127 ,191),
	RGB(165 ,  0 ,82 ),
	RGB(165 , 82 ,124),
	RGB(127 ,  0 ,63 ),
	RGB(127 , 63 ,95 ),
	RGB( 76 ,  0 ,38 ),
	RGB( 76 , 38 ,57 ),
	RGB( 38 ,  0 ,19 ),
	RGB( 38 , 19 ,28 ),
	RGB(255 ,  0 ,63 ),
	RGB(255 ,127 ,159),
	RGB(165 ,  0 ,41 ),
	RGB(165 , 82 ,103),
	RGB(127 ,  0 ,31 ),
	RGB(127 , 63 ,79 ),
	RGB( 76 ,  0 ,19 ),
	RGB( 76 , 38 ,47 ),
	RGB( 38 ,  0 ,9  ),
	RGB( 38 , 19 ,23 ),
	RGB( 84 , 84 ,84 ),
	RGB(118 ,118 ,118),
	RGB(152 ,152 ,152),
	RGB(186 ,186 ,186),
	RGB(220 ,220 ,220),
	RGB(255 ,255 ,255)
};

namespace MapGIS
{
	class Map : public CObject  
	{
	public:
		Map();
		virtual ~Map();
		int Add( CString filename,CRect rect );
		void Insert( int index, IMapLayer *ml );
		void Remove( int index );

		void Center( int x, int y );
		void Center( double x, double y );

		void Draw(CWnd *pWnd, CDC *pDC);

		void ZoomObject(DWORD Handle);
		void ZoomArea( int sx, int sy, int ex, int ey );
		void ZoomArea( RectangleD &rect );
		void ZoomExtents();
		void ZoomIn();
		void ZoomIn( int x, int y );
		void ZoomIn( double x, double y );
		void ZoomOut();
		void ZoomOut( int x, int y );
		void ZoomOut( double x, double y );

		void SetZoomObjectScale(UINT Scale);

		void SetLayerActive(DWORD index);

		int GetCount();
		
		IMapLayer* GetLayer(int index);

		void setMetrics(MapMetrics *value);
		MapMetrics* getMetrics();

		RectangleD* getExtents();
		
		CString GetLayerNames();

		BOOL IsModified( );
		void SelectObject(DWORD Handle);
		void ClearSelected();
		MapMetrics *_mapMetrics;
	private:
		BOOL m_bModified;
		void CalculateExtents();
		void SetModifiedFlag( BOOL bModified = TRUE );
		
		CList<IMapLayer*,IMapLayer*> List;

		CBrush *m_phBrush;
		CBrush *m_phBrushActive;
		UINT m_iPenWidth;
		UINT m_iZoomObjectScale;
		COLORREF m_dwBrushColor;
		CPen *m_phPen;
		CPen *m_phPenSelected;
		CPen *m_phColorsACI;
	};
}
#endif //_MAP_
