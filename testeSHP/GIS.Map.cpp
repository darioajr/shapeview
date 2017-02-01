#include "stdafx.h"

#include "GIS.Map.h"
#include "GIS.IMapLayer.h"

using MapGIS::IMapLayer;
using MapGIS::Map;

//Geo
#include "GIS.MapMetrics.h"

using MapGIS::MapMetrics;

//DataSources
#include "GIS.SHPReader.h"
#include "GIS.DXFReader.h"

using MapGIS::SHPReader;
using MapGIS::DXFReader;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define FILETYPE_SHP _T(".SHP")
#define FILETYPE_DXF _T(".DXF")

Map::Map() : _mapMetrics(NULL)
{
	_mapMetrics		= new MapMetrics();
	
	m_bModified		= FALSE;
	m_dwBrushColor	= RGB(128,128,128);
	
	m_iPenWidth		= 1;
	
	m_phBrush		= new CBrush(m_dwBrushColor);
	m_phBrushActive	= new CBrush(RGB(255,128,64));
	m_phPen			= new CPen(PS_SOLID,m_iPenWidth,RGB(0,0,0));
	m_phPenSelected	= new CPen(PS_SOLID,m_iPenWidth*2,RGB(255,0,0));

	m_phColorsACI	= NULL;
	m_phColorsACI	= ::new CPen[256];
	
	for (int i=0;i<256;i++)
		m_phColorsACI[i].CreatePen(PS_SOLID,m_iPenWidth,ACI[i]);

	m_iZoomObjectScale = 200;
}

Map::~Map()
{
	while(List.GetCount() > 0)
		delete List.RemoveTail();

	for (int i=0;i<256;i++)
		::DeleteObject(m_phColorsACI[i]);
	
	::delete[] m_phColorsACI;

	delete _mapMetrics;
	delete m_phBrush;
	delete m_phBrushActive;
	delete m_phPen;
	delete m_phPenSelected;
}

int Map::Add( CString filename,CRect rect )  
{
	filename.MakeUpper();
	filename.TrimRight();

	_mapMetrics->_canvas = rect;

	if(filename.Find(FILETYPE_SHP) != -1)
	{
		IMapLayer *ml = NULL;
		ml = new SHPReader( filename, _mapMetrics, m_phBrush, m_phBrushActive, m_phPen, m_phPenSelected);
		ml->m_LayerType = Vector;
		ml->m_LayerSubType = Shapefile;
		List.AddTail(ml); 
	}
	else if(filename.Find(FILETYPE_DXF) != -1)
	{
		IMapLayer *ml = NULL;
		ml = new DXFReader( filename, _mapMetrics, m_phColorsACI);
		ml->m_LayerType = Vector;
		ml->m_LayerSubType = Shapefile;
		List.AddTail(ml); 
	}
	else
	{
		AfxMessageBox(_T("Arquivo nao suportado!"));
		return 0;
	}
	
	CalculateExtents();

	SetModifiedFlag();
	
	return 0;
}

void Map::Insert(int index, IMapLayer *ml )
{
	POSITION POS = List.FindIndex(index);
	if(POS)
	{
		List.InsertAfter(POS, ml);
		CalculateExtents();
	}
}

int Map::GetCount()
{
	return List.GetCount();
}

IMapLayer* Map::GetLayer(int index)
{
	POSITION POS = List.FindIndex(index);
	if(POS)
		return List.GetAt(POS);
	else
		return NULL;
}

void Map::Remove( int index )
{
	POSITION POS = List.FindIndex(index);
	if(POS)
	{
		IMapLayer *ml;
		ml = List.GetAt(POS);
		List.RemoveAt(POS);
		delete ml;
		CalculateExtents();
	}
}

void Map::SetLayerActive(DWORD index)
{
	POSITION POS = List.GetHeadPosition();
	while (POS)
	{
		IMapLayer *layer = List.GetNext(POS);
		layer->m_bActive = FALSE;
	}

	IMapLayer *layer = GetLayer(index);
	if(layer)
		layer->m_bActive = TRUE;
}

void Map::CalculateExtents()
{
	if ( List.GetCount() == 0 )
	{
		_mapMetrics->Reset();
		_mapMetrics->UpdateMetrics();
	}
	else
	{
		_mapMetrics->_extents.setXmin(MAXDOUBLE);
		_mapMetrics->_extents.setYmin(MAXDOUBLE);
		_mapMetrics->_extents.setXmax(MINDOUBLE);
		_mapMetrics->_extents.setYmax(MINDOUBLE);
		
		POSITION POS = List.GetHeadPosition();
		while (POS)
		{
			IMapLayer *layer = List.GetNext(POS);

			if  ( layer->_extents->Xmin() < _mapMetrics->_extents.Xmin() )
				_mapMetrics->_extents.setXmin(layer->_extents->Xmin());

			if  ( layer->_extents->Ymin() < _mapMetrics->_extents.Ymin() )
				_mapMetrics->_extents.setYmin(layer->_extents->Ymin());
			
			if  ( layer->_extents->Xmax() > _mapMetrics->_extents.Xmax() )
				_mapMetrics->_extents.setXmax(layer->_extents->Xmax());

			if  ( layer->_extents->Ymax() > _mapMetrics->_extents.Ymax() )
				_mapMetrics->_extents.setYmax(layer->_extents->Ymax());
		}
	}
	
	if ( _mapMetrics->_viewport.DeltaX() == 0 &&
		_mapMetrics->_viewport.DeltaY() == 0 )
	{
		_mapMetrics->_extents.FixGeoAspect();
		
		_mapMetrics->_viewport.setXmin(_mapMetrics->_extents.Xmin());
		_mapMetrics->_viewport.setYmin(_mapMetrics->_extents.Ymin());
		_mapMetrics->_viewport.setXmax(_mapMetrics->_extents.Xmax());
		_mapMetrics->_viewport.setYmax(_mapMetrics->_extents.Ymax());
	}
	
	_mapMetrics->_extents.FixGeoAspect();
}
		
void Map::Center( int x, int y )
{
	double dx = (double) x;
	double dy = (double) y;
	
	_mapMetrics->Pixel2World( dx, dy );

	Center( dx, dy );

	SetModifiedFlag();
}
		
void Map::Center( double x, double y )
{
	double halfdeltax;
	double halfdeltay;
	
	halfdeltax = _mapMetrics->_viewport.DeltaX() / 2;
	halfdeltay = _mapMetrics->_viewport.DeltaY() / 2;
	
	_mapMetrics->_viewport.setXmin(x - halfdeltax);
	_mapMetrics->_viewport.setXmax(x + halfdeltax);
	
	_mapMetrics->_viewport.setYmin(y - halfdeltay);
	_mapMetrics->_viewport.setYmax(y + halfdeltay);

	SetModifiedFlag();
}

void Map::SetModifiedFlag( BOOL bModified )
{
	m_bModified = bModified;
}

BOOL Map::IsModified( )
{
	return m_bModified;
}

void Map::Draw(CWnd *pWnd, CDC *pDC)
{
	ASSERT(pDC);

	_mapMetrics->UpdateMetrics();
	
	POSITION POS = List.GetHeadPosition();
	
	while(POS)
	{
		IMapLayer *layer = List.GetAt(POS);
		switch(layer->m_LayerType)
		{
		case Vector:
			{
				//uso futuro
				switch(layer->m_LayerSubType)
				{
				case Shapefile:
					layer->Draw(pWnd, pDC);
					break;
				case DXF:
					layer->Draw(pWnd, pDC);
					break;
				default:
					break;
				}
			}
			break;
		case Raster:
			//uso futuro
			break;
		default:
			break;
		}
		
		List.GetNext(POS);
	}
}

RectangleD* Map::getExtents()
{
	return &_mapMetrics->_extents;
}

void Map::ZoomArea( int sx, int sy, int ex, int ey )
{
	double dsx = (double) sx;
	double dsy = (double) sy;
	double dex = (double) ex;
	double dey = (double) ey;
	
	if ( sx != ex || sy != ey )
	{
		_mapMetrics->Pixel2World( dsx, dsy );
		_mapMetrics->Pixel2World( dex, dey );
		
		_mapMetrics->_viewport.setXmin(dsx);
		_mapMetrics->_viewport.setXmax(dex);
		
		_mapMetrics->_viewport.setYmin(dey);
		_mapMetrics->_viewport.setYmax(dsy);

		_mapMetrics->_viewport.FixGeoAspect();
	}
}

void Map::ZoomArea( RectangleD &rect )
{
	_mapMetrics->_viewport = rect;
}

void Map::ZoomExtents()
{
	_mapMetrics->_viewport.setXmin(_mapMetrics->_extents.Xmin());
	_mapMetrics->_viewport.setYmin(_mapMetrics->_extents.Ymin());
	_mapMetrics->_viewport.setXmax(_mapMetrics->_extents.Xmax());
	_mapMetrics->_viewport.setYmax(_mapMetrics->_extents.Ymax());
}

void Map::ZoomIn()
{
	double quarterdeltax;
	double quarterdeltay;
	
	quarterdeltax = _mapMetrics->_viewport.DeltaX() / 4;
	quarterdeltay = _mapMetrics->_viewport.DeltaY() / 4;
	
	_mapMetrics->_viewport.setXmin(_mapMetrics->_viewport.Xmin() + quarterdeltax);
	_mapMetrics->_viewport.setXmax(_mapMetrics->_viewport.Xmax() - quarterdeltax);
	
	_mapMetrics->_viewport.setYmin(_mapMetrics->_viewport.Ymin() + quarterdeltay);
	_mapMetrics->_viewport.setYmax(_mapMetrics->_viewport.Ymax() - quarterdeltay);
}

void Map::ZoomIn( int x, int y )
{
	Center( x, y );
	ZoomIn();
}

void Map::ZoomIn( double x, double y )
{
	Center( x, y );
	ZoomIn();
}

void Map::ZoomOut()
{
	double thirddeltax;
	double thirddeltay;
	
	thirddeltax = _mapMetrics->_viewport.DeltaX() / 3;
	thirddeltay = _mapMetrics->_viewport.DeltaY() / 3;
	
	_mapMetrics->_viewport.setXmin(_mapMetrics->_viewport.Xmin() - thirddeltax);
	_mapMetrics->_viewport.setXmax(_mapMetrics->_viewport.Xmax() + thirddeltax);
	
	_mapMetrics->_viewport.setYmin(_mapMetrics->_viewport.Ymin() - thirddeltay);
	_mapMetrics->_viewport.setYmax(_mapMetrics->_viewport.Ymax() + thirddeltay);
}

void Map::ZoomOut( int x, int y )
{
	Center( x, y );
	ZoomOut();
}

void Map::ZoomOut( double x, double y )
{
	Center( x, y );
	ZoomOut();
}

CString Map::GetLayerNames()
{
	CString sNames;
	POSITION POS = List.GetHeadPosition();
	while(POS)
	{
		IMapLayer *layer = List.GetNext(POS);
		sNames += layer->LayerName();
		sNames += _T("|");
	}
	
	return sNames;
}

MapMetrics* Map::getMetrics()
{
	return _mapMetrics;
}

void Map::setMetrics(MapMetrics *value)
{
	_mapMetrics = value;
}

void Map::ZoomObject(DWORD Handle)
{
	POSITION POS = List.GetHeadPosition();
	while(POS)
	{
		RectangleD rc;
		IMapLayer *layer = List.GetNext(POS);
		layer->SelectObject(Handle);
		if(layer->GetRectObject(Handle,&rc))
		{
			rc.setXmin(rc.Xmin()-m_iZoomObjectScale);
			rc.setXmax(rc.Xmax()+m_iZoomObjectScale);
			rc.setYmin(rc.Ymin()-m_iZoomObjectScale);
			rc.setYmax(rc.Ymax()+m_iZoomObjectScale);

			ZoomArea(rc);
		}
	}
}

void Map::SelectObject(DWORD Handle)
{
	POSITION POS = List.GetHeadPosition();
	while(POS)
	{
		IMapLayer *layer = List.GetNext(POS);
		layer->SelectObject(Handle);
	}
}

void Map::ClearSelected()
{
	POSITION POS = List.GetHeadPosition();
	while(POS)
	{
		IMapLayer *layer = List.GetNext(POS);
		layer->ClearSelected();
	}
}

void Map::SetZoomObjectScale(UINT Scale)
{
	m_iZoomObjectScale = Scale;
}