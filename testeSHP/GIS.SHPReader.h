#if !defined(_SHPREADER_)
#define _SHPREADER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "GIS.DbaseReader.h"
#include "GIS.BinaryReader.h"
#include "GIS.PointD.h"
#include "GIS.RetangleD.h"
#include "GIS.IMapLayer.h"
#include "GIS.MapMetrics.h"
#include "GIS.GDIThread.h"

using MapGIS::BinaryReader;
using MapGIS::PointD;
using MapGIS::RectangleD;
using MapGIS::IMapLayer;
using MapGIS::MapMetrics;

#define REDRAW (WM_USER+111)
#define STOP   (WM_USER+222)

namespace MapGIS
{
	class ShapeRecord;
	
	typedef CList<ShapeRecord*,ShapeRecord*> CShapeVector;
	
	typedef enum ShapeType
	{
			SHAPE_NULL			= 0,
			
			SHAPE_POINT			= 1,
			SHAPE_POLYLINE		= 3,
			SHAPE_ARC			= 3,
			SHAPE_POLYGON		= 5,
			SHAPE_MULTIPOINT	= 8,
			
			SHAPE_POINTZ		= 11,
			SHAPE_POLYLINEZ		= 13,
			SHAPE_ARCZ			= 13,
			SHAPE_POLYGONZ		= 15,
			SHAPE_MULTIPOINTZ	= 18,
			
			SHAPE_POINTM		= 21,
			SHAPE_POLYLINEM		= 23,
			SHAPE_ARCM			= 23,
			SHAPE_POLYGONM		= 25,
			SHAPE_MULTIPOINTM	= 28,
			SHAPE_MULTIPATCH	= 31
	};
	
	class ShapeHeader : public CObject
	{
	public:
		ShapeHeader()
		{
			this->mMax		 = 0.0;
			this->mMin		 = 0.0;
			this->xMax		 = 0.0;
			this->xMin		 = 0.0;
			this->yMax		 = 0.0;
			this->yMin		 = 0.0;
			this->zMax		 = 0.0;
			this->zMin		 = 0.0;
			this->FileCode	 = 0;
			this->FileLength = 0;
			this->Version	 = 0;
			this->m_ShapeType= SHAPE_NULL;
		};
		
	public:
		double		mMax;
		double		mMin;
		double		xMax;
		double		xMin;
		double		yMax;
		double		yMin;
		double		zMax;
		double		zMin;
		int			FileCode;
		int			FileLength;
		int			Version;
		ShapeType	m_ShapeType;
	};
	
	
	class ShapePolygon : public CObject
	{
	public:
		ShapePolygon()
		{
			this->m_ShapeType = SHAPE_POLYGON;
			this->xMin		= MAXDOUBLE;
			this->yMin		= MAXDOUBLE;
			this->xMax		= MINDOUBLE;
			this->yMax		= MINDOUBLE;
		};
		virtual ~ShapePolygon()
		{
		};

		void RemoveAll()
		{
			m_parts.RemoveAll();

			while (m_points.GetCount() > 0)
				delete m_points.RemoveTail();
		}

		void AddParts(int part)
		{
			this->m_parts.AddTail(part);
		};
		void AddPoints(PointD* p)
		{
			this->m_points.AddTail(p);
			
			if  (p->X < xMin)
				xMin = p->X;

			if  (p->Y < yMin)
				yMin = p->Y;
			
			if  (p->X > xMax)
				xMax = p->X;

			if  (p->Y > yMax)
				yMax = p->Y;
		};

		DWORD GetSize()
		{
			return m_points.GetCount();
		}

		PointD* GetVertex(int index)
		{
			POSITION POS = m_points.FindIndex(index);
			if(POS)
				return m_points.GetAt(POS);

			return NULL;
		}

		BOOL PointIn(PointD* p)
		{
			int     j, inside_flag, xflag0;
			double   dv0 ;
			int     crossings, yflag0, yflag1 ;
			PointD *Vertex0, *Vertex1;
			
			Vertex0 = GetVertex(GetSize()-1);
			
			yflag0 = ( dv0 = Vertex0->Y - p->Y ) >= 0.0 ;
			
			crossings = 0 ;
			
			for ( j = 0 ; j < (int)GetSize() ; j++ )
			{
				if ( j & 0x1 )
				{
					Vertex0 = GetVertex(j);
					yflag0 = ( dv0 = Vertex0->Y - p->Y ) >= 0.0 ;
				} 
				else
				{
					Vertex1 = GetVertex(j);
					yflag1 = ( Vertex1->Y >= p->Y ) ;
				}
				
				if ( yflag0 != yflag1 )
				{
					if ( ( xflag0 = ( Vertex0->X >= p->X ) ) == ( Vertex1->X >= p->X ) )
					{
						
						if ( xflag0 ) crossings++ ;
					} 
					else 
					{
						crossings += (Vertex0->X -
							dv0*( Vertex1->X-Vertex0->X)/(Vertex1->Y-Vertex0->Y)) >= p->X ;
					}
				}
			}
			
			inside_flag = crossings & 0x01 ;
			
			return (inside_flag) ;
		}


	public:
		int		m_ShapeType;
		RectangleD m_Polygon;
		double	xMin;
		double	yMin;
		double	xMax;
		double	yMax;
		
		CList<int,int> m_parts;
		CList<PointD*,PointD*> m_points;
	};

	class ShapePoint : public CObject
	{
	public:
		ShapePoint()
		{
			this->m_ShapeType = SHAPE_POINT;
			this->point = NULL;
			this->point = new PointD;
		};
		virtual ~ShapePoint()
		{
			
		};

		void RemoveAll()
		{
			delete point;
		}

	public:
		int		m_ShapeType;
		PointD *point;
	};
	
	class ShapeRecord : public CObject
	{
	public:
		ShapeRecord()
		{
			this->m_Data	 = NULL;
			this->m_Selected = FALSE;
		}

		virtual ~ShapeRecord()
		{
		}

		void RemoveAll()
		{
			if(m_Data)
			{
				switch((ShapeType)m_ShapeType)
				{
				case SHAPE_POINT:
					{
						((ShapePoint*)m_Data)->RemoveAll();
						delete (ShapePoint*)m_Data;	
					}
					break;
				case SHAPE_POLYGON:
					{
						((ShapePolygon*)m_Data)->RemoveAll();
						delete (ShapePolygon*)m_Data;	
					}
					break;
				default:
					break;
				}
			}
		}

		int			m_RecordNum;
		int			m_Length;
		ShapeType   m_ShapeType;
		void		*m_Data;
		BOOL		m_Selected;

	};

	class SHPVector
	{
	public:
		SHPVector(){}
		virtual ~SHPVector(){}
		void RemoveAll()
		{
			while(this->Records.GetCount())
			{
				ShapeRecord *pShapeRecord = this->Records.RemoveTail();
				pShapeRecord->RemoveAll();
				delete pShapeRecord;
			}
		}
		DWORD GetCount()
		{
			return Records.GetCount();
		}
		
	public:
		CList<ShapeRecord*,ShapeRecord*> Records;
	};

	
	class SHPReader : public IMapLayer
	{
	private:
		CString	  m_sShapeFilename;
		ShapeHeader *m_pShapeHeader;
		SHPVector m_Vector;
		BOOL	m_bThread;

	private:		
		BOOL ReadFile();

		BOOL ReadShapeNull(BinaryReader *br, int num, int size);
		BOOL ReadShapePolygon(BinaryReader *br, int num, int size);
		BOOL ReadShapePoint(BinaryReader *br, int num, int size);
		BOOL ReadShapePolyline(BinaryReader *br, int num, int size);
		BOOL ReadShapeMultiPoint(BinaryReader *br, int num, int size);

		BOOL ReadHeader(BinaryReader *br);
		void AddRecord(ShapeRecord* r);
		void FreeAllRecords();
		BOOL Inicialize();
		BOOL Finalize();
		static void	SwapWord( int length, void *wordP );
		GDIThread *m_pThread;
		void DeleteThread();
		double azA(PointD ptA,PointD ptB,PointD ptP);
		double azB(PointD ptA,PointD ptB,PointD ptP);
		void ObjetoMaisPerto(int x, int y, UINT &id);
		void ObjetoDentro(int x, int y, long &id);
		
	public:
		SHPReader();
		SHPReader(CString filename, MapMetrics *mapmetrics, CBrush *brush, CBrush *brushactive, CPen *pen, CPen *penselected);
		~SHPReader();

		MapMetrics *_mapmetrics;
		CBrush *m_phBrush;
		CBrush *m_phBrushActive;
		CPen *m_phPen;
		CPen *m_phPenSelected;
		
		DWORD GetCountShape();
		BOOL Open(const CString filename);
		BOOL Close();
		BOOL IsOpen();
		CString GetRecord(int index, CString PipeField);

		virtual void Draw(CWnd *pWnd, CDC *pDC);
		virtual void DrawLabels();
		virtual CString QueryPoint( int x, int y, CString PipeFields );
		virtual CString QueryObject(DWORD Handle);
		virtual void SelectObject(DWORD Handle);
		virtual void ClearSelected();
		virtual BOOL GetRectObject(DWORD Handle, RectangleD *rc);
	};
}
#endif // !defined(_SHPREADER_)