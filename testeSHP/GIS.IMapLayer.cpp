#include "stdafx.h"
#include "GIS.IMapLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using MapGIS::IMapLayer;


IMapLayer::IMapLayer()
{
	m_bActive = FALSE;
}

IMapLayer::~IMapLayer()
{
}

CString IMapLayer::LayerName()
{
	return m_sLayerName;
}

UINT IMapLayer::DrawingType()
{
	return (UINT)m_LayerType;
}

UINT IMapLayer::DrawingSubType()
{
	return (UINT)m_LayerSubType;
}

RectangleD* IMapLayer::Extents()
{
	return _extents;
}
