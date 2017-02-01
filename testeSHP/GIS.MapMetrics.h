#if !defined(_MAPMETRICS_)
#define _MAPMETRICS_

#include "stdafx.h"
#include "GIS.RetangleD.h"

namespace MapGIS
{
	class MapMetrics : public CObject 
	{
	public:
		RectangleD	_extents;
		RectangleD	_viewport;
		double		_mapUnitsPerPixelX;
		double		_mapUnitsPerPixelY;
		CRect		_canvas;

	public:
		MapMetrics()
		{
		}

		virtual ~MapMetrics()
		{
		}

		RectangleD* getExtents()
		{
			return &_extents;
		}
		
		void Reset()
		{
			_viewport.Reset();
			_extents.Reset();
		}

		void setExtents(RectangleD &value)
		{
			_extents = value;
		}
		
		void Pixel2World( double &dx, double &dy )
		{
			dx = _viewport.Xmin() + (dx * _mapUnitsPerPixelX);
			dy = _viewport.Ymax() - (dy * _mapUnitsPerPixelY);
		}

		void Pixel2World( CPoint point, PointD* tempPoint )
		{
			tempPoint->X = _viewport.Xmin() + ( point.x * _mapUnitsPerPixelX );
			tempPoint->Y = _viewport.Ymax() - ( point.y * _mapUnitsPerPixelY );
		}

		void UpdateMetrics()
		{
			CalculateUnitsPerPixel();
		}

		RectangleD* getViewport()
		{
			return &_viewport;
		}

		void setViewport(RectangleD &value)
		{
			_viewport = value;
		}

		POINT World2Pixel( double x, double y )
		{
			POINT pt;

			pt.x = (long)((x - _viewport.Xmin()) / _mapUnitsPerPixelX);
			pt.y = (long)((_viewport.Ymax() - y) / _mapUnitsPerPixelY);

			return pt;
		}
		
		POINT World2Pixel( PointD *ptD )
		{
			POINT pt;
		
			pt.x = (long)(( ptD->X - _viewport.Xmin() ) / _mapUnitsPerPixelX);
			pt.y = (long)(( _viewport.Ymax() - ptD->Y ) / _mapUnitsPerPixelY);

			return pt;
		}
				
		void World2Pixel( PointD oldPoint, CPoint &newPoint )
		{			
			newPoint.x = (int) ( ( oldPoint.X - _viewport.Xmin() ) / _mapUnitsPerPixelX );
			newPoint.y = (int) ( ( _viewport.Ymax() - oldPoint.Y ) / _mapUnitsPerPixelY );
		}
		
	private:
		void CalculateUnitsPerPixel()
		{
			_mapUnitsPerPixelX = _viewport.DeltaX() / _canvas.Width();
			_mapUnitsPerPixelY = _viewport.DeltaY() / _canvas.Height();
		}
	};
}

#endif //_MAPMETRICS_