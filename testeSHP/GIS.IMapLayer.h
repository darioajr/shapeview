#if !defined(_IMAPLAYER_)
#define _IMAPLAYER_

#include "stdafx.h"

#include "GIS.RetangleD.h"
#include "GIS.MapMetrics.h"
#include "GIS.Message.h"

using MapGIS::RectangleD;
using MapGIS::MapMetrics;

#define MAXDOUBLE 999999999
#define MINDOUBLE -999999999

namespace MapGIS
{
	typedef enum LayerType { Vector, Raster };
	typedef enum LayerSubType { Shapefile, Mif, E00, GeoTIFF, DXF };

	class IMapLayer
	{
	public:
		IMapLayer();
		virtual ~IMapLayer();
		virtual void Draw(CWnd *pWnd, CDC *pDC)=0;
		virtual void DrawLabels()=0;
		virtual CString QueryPoint( int x, int y, CString PipeFields )=0;
		virtual CString QueryObject(DWORD Handle)=0;
		virtual void SelectObject(DWORD Handle)=0;
		virtual void ClearSelected()=0;
		virtual BOOL GetRectObject(DWORD Handle, RectangleD *rc)=0;
	public:
		CString LayerName();
		UINT DrawingType();
		UINT DrawingSubType();
		RectangleD* Extents();
		
	public:
		CString m_sLayerName;
		LayerType m_LayerType;
		LayerSubType m_LayerSubType;
		RectangleD *_extents;
		BOOL m_bActive;
	};
}

#endif //_IMAPLAYER_