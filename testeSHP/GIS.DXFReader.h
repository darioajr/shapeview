#if !defined(_DXFREADER_)
#define _DXFREADER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "GIS.PointD.h"
#include "GIS.IMapLayer.h"
#include "GIS.MapMetrics.h"

using MapGIS::PointD;
using MapGIS::IMapLayer;
using MapGIS::MapMetrics;

#define MAX_EXTEND(a,b,c)  \
          if (a->UL.X > b && b > 0)  \
             a->UL.X = b;   \
          if (a->UL.Y > c && c > 0)   \
             a->UL.Y = c;    \
          if (a->LR.X < b && b > 0) \
             a->LR.X = b;  \
          if (a->LR.Y < c && c > 0)\
             a->LR.Y = c;


namespace MapGIS
{
	typedef enum DXFObjectType
	{
		DXF_OBJECT		= 0,
		DXF_POINT		= 1,
		DXF_LINE		= 2,
		DXF_CIRCLE		= 3,
		DXF_POLYLINE	= 4,
		DXF_TEXT		= 5,
		DXF_HEADER		= 9
	};

	class DXFObject
	{
	public:
		DXFObject()
		{
			this->Entitie   = 0;
			this->Angle		= 0;
			this->Size		= 1;
			this->Text		= "";
			this->ACI		= 0;
			this->Handle	= 0;
		}
		virtual ~DXFObject() {}

		void RemoveAll()
		{
			while(this->Vertex.GetCount())
				delete this->Vertex.RemoveTail();
		}

		DXFObject &operator= (DXFObject &object)
		{
			this->Entitie   = object.Entitie;
			this->Angle     = object.Angle;
			this->Size      = object.Size;
			this->Text		= object.Text;
			
			this->Vertex.RemoveAll();
			this->Vertex.AddTail(&object.Vertex);
			
			return *this;
		}
	public:
		unsigned char Entitie;
		CString Text;
		int Angle;
		COLORREF ACI;
		double Size;
		DWORD Handle;
		CList<PointD*,PointD*> Vertex;
	};
	
	class DXFLayer
	{
	public:
		DXFLayer()
		{
			this->Visible   = TRUE;
			this->Name      = "";
			this->ACI       = 5; //Preto como padrao
		}
		virtual ~DXFLayer() {}
		
		void RemoveAll()
		{
			while(this->Objects.GetCount())
			{
				DXFObject *pDXFObject = this->Objects.RemoveTail();
				pDXFObject->RemoveAll();
				delete pDXFObject;
			}
		}

		DXFLayer &operator= (DXFLayer &layer)
		{
			this->Visible   = TRUE;
			this->Name      = layer.Name;
			this->ACI       = layer.ACI;
			
			this->Objects.RemoveAll();
			this->Objects.AddTail(&layer.Objects);
			
			return *this;
		}
	public:
		BOOL Visible;
		CString Name;
		COLORREF ACI;
		CList<DXFObject*,DXFObject*> Objects;
	};
	
	class DXFVector
	{
	public:
		DXFVector(){}
		virtual ~DXFVector(){}
		void RemoveAll()
		{
			while(this->Layers.GetCount())
			{
				DXFLayer *pDXFLayer = this->Layers.RemoveTail();
				pDXFLayer->RemoveAll();
				delete pDXFLayer;
			}
		}
		
	public:
		CList<DXFLayer*,DXFLayer*> Layers;
	};

	class DXFReader : public IMapLayer
	{
	public:
		DXFReader();
		DXFReader(CString filename, MapMetrics *mapmetrics, CPen *pen);
		~DXFReader();

	private:
		CString NameGroup(CString string);
		char*   ReadLine(char* pcSimbol,int dwTotal,int &dwPosition);

	public:
		BOOL    Close();
		long    Open(LPCTSTR FileName);

	public:	
		virtual void Draw(CWnd *pWnd, CDC *pDC);
		virtual void DrawLabels();
		virtual CString QueryPoint( int x, int y, CString PipeFields );
		virtual CString QueryObject(DWORD Handle);
		virtual void SelectObject(DWORD Handle);
		virtual void ClearSelected();
		virtual BOOL GetRectObject(DWORD Handle, RectangleD *rc);
		
		DXFVector	m_Vector;

		BOOL        m_bOpened;

		CStringList m_Layers;

		MapMetrics  *m_pMapmetrics;

		CPen		*m_phColorsACI;
	};
}
#endif // !defined(_DXFREADER_)