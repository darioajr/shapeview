// DXFReader.cpp : implementation file
//

#include "stdafx.h"
#include "GIS.DXFReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using MapGIS::DXFReader;

DXFReader::DXFReader()
{
	m_LayerType		= Vector;
	m_LayerSubType	= DXF;
	_extents		= new RectangleD;
	m_phColorsACI	= NULL;
	m_bOpened		= FALSE;
}

DXFReader::DXFReader(CString filename, MapMetrics *mapmetrics, CPen *pen)
{
	m_LayerType		= Vector;
	m_LayerSubType	= DXF;
	m_pMapmetrics	= mapmetrics;
	_extents		= new RectangleD;
	m_phColorsACI	= pen;
	m_bOpened		= FALSE;

	Open(filename);
}

DXFReader::~DXFReader()
{
	Close();
	delete _extents;
}

BOOL DXFReader::Close() 
{
   if (!m_bOpened) return FALSE;

   m_Vector.RemoveAll();
   
   m_bOpened = FALSE;
   
	return TRUE;
}

long DXFReader::Open(LPCTSTR FileName) 
{
   HANDLE hFile;
   
   hFile = ::CreateFile(FileName ,GENERIC_READ ,FILE_SHARE_READ
      ,NULL ,OPEN_EXISTING ,FILE_ATTRIBUTE_NORMAL ,NULL);
   
   if (hFile == INVALID_HANDLE_VALUE)
      return FALSE;

   DWORD dwSize   = ::GetFileSize(hFile,NULL);
   DWORD dwReturn = 0;

   char *pcBuffer = NULL;
   pcBuffer = new char[dwSize];

   char *pErro = NULL;

   if(pcBuffer == NULL)
	   return 0;

   if (::ReadFile(hFile,pcBuffer,dwSize,&dwReturn,0) == FALSE || dwReturn != dwSize)
      return 0;

   ::CloseHandle(hFile);

   char *pcGroup = NULL;
   char *pcValue = NULL;

   PointD pt;
   PointD *pPT = NULL;

   BOOL b_Header = FALSE;
   BOOL b_ClodedPolyline = FALSE;
   CString sGroupName;

   int dwGroupPos       = 0;
   DWORD dwGroupPosBefore = 0;
   DWORD dwGroupNumber    = 0;

   for (;dwGroupPos<(int)dwSize;)
   {
      dwGroupPosBefore = dwGroupPos;

      pcGroup = ReadLine(pcBuffer,dwSize,dwGroupPos);
      if (pcGroup == NULL) break;

      pcValue = ReadLine(pcBuffer,dwSize,dwGroupPos);
      if (pcValue == NULL) break;

      dwGroupNumber = atoi(pcGroup);

	  switch(dwGroupNumber)
	  {
	  case DXF_HEADER:
		  {
			  if(NameGroup(pcValue) == "$EXTMIN")
			  {
				  dwGroupPosBefore = dwGroupPos;
				  
				  pcGroup = ReadLine(pcBuffer,dwSize,dwGroupPos);
				  if (pcGroup == NULL) break;
				  
				  pcValue = ReadLine(pcBuffer,dwSize,dwGroupPos);
				  if (pcValue == NULL) break;
				  
				  _extents->setXmin(strtod(pcValue,&pErro));

				  dwGroupPosBefore = dwGroupPos;
				  
				  pcGroup = ReadLine(pcBuffer,dwSize,dwGroupPos);
				  if (pcGroup == NULL) break;
				  
				  pcValue = ReadLine(pcBuffer,dwSize,dwGroupPos);
				  if (pcValue == NULL) break;
				  
				  _extents->setYmin(strtod(pcValue,&pErro));
			  }
			  else if(NameGroup(pcValue) == "$EXTMAX")
			  {
				  dwGroupPosBefore = dwGroupPos;
				  
				  pcGroup = ReadLine(pcBuffer,dwSize,dwGroupPos);
				  if (pcGroup == NULL) break;
				  
				  pcValue = ReadLine(pcBuffer,dwSize,dwGroupPos);
				  if (pcValue == NULL) break;
				  
				   _extents->setXmax(strtod(pcValue,&pErro));

				  dwGroupPosBefore = dwGroupPos;
				  
				  pcGroup = ReadLine(pcBuffer,dwSize,dwGroupPos);
				  if (pcGroup == NULL) break;
				  
				  pcValue = ReadLine(pcBuffer,dwSize,dwGroupPos);
				  if (pcValue == NULL) break;
				  
					_extents->setYmax(strtod(pcValue,&pErro));
			  }
		  }
		  break;
		  
	  case DXF_OBJECT:
		  {
			  sGroupName = NameGroup(pcValue);
			  
			  if(sGroupName  == "POLYLINE" ||
			     sGroupName  == "LINE"     ||
			     sGroupName  == "CIRCLE"   ||
			     sGroupName  == "POINT"    ||
			     sGroupName  == "TEXT")
			  {
				  BOOL bFirst     = TRUE;
				  BOOL bXComplete = FALSE;
				  BOOL bYComplete = FALSE;
				  BOOL bFinish    = FALSE;
				  CString LayerName;
				  
				  DXFObject *Object = new DXFObject;
				  if(Object == NULL) return FALSE;
					  
					  if(sGroupName == "POLYLINE")
						  Object->Entitie = DXF_POLYLINE;
					  else if(sGroupName  == "LINE")
						  Object->Entitie = DXF_LINE;
					  else if(sGroupName  == "CIRCLE")
						  Object->Entitie = DXF_CIRCLE;
					  else if(sGroupName  == "POINT")
						  Object->Entitie = DXF_POINT;
					  else if(sGroupName  == "TEXT")
						  Object->Entitie = DXF_TEXT;
					  
					  for (;dwGroupPos<(int)dwSize;)
					  {
						  dwGroupPosBefore = dwGroupPos;
						  
						  pcGroup = ReadLine(pcBuffer,dwSize,dwGroupPos);
						  if (pcGroup == NULL) break;
						  
						  pcValue = ReadLine(pcBuffer,dwSize,dwGroupPos);
						  if (pcValue == NULL) break;
						  
						  dwGroupNumber = atoi(pcGroup);
						  
						  if (bXComplete == TRUE && bYComplete == TRUE && bFirst == FALSE)
						  {
							  pPT = new PointD;
							  if(pPT == NULL) return FALSE;
							  pPT->X = pt.X;
							  pPT->Y = pt.Y;
							  
							  Object->Vertex.AddTail(pPT);
							  
							  MAX_EXTEND(_extents,pt.X,pt.Y);
							  
							  bXComplete = FALSE;
							  bYComplete = FALSE;

							  if(Object->Entitie == DXF_LINE &&
								  Object->Vertex.GetCount() > 1)
								  break;

							  if(Object->Entitie == DXF_CIRCLE ||
								  Object->Entitie == DXF_POINT ||
								  Object->Entitie == DXF_TEXT)
								  break;
						  }
						  
						  switch(dwGroupNumber)
						  {
						  case 0:
							  if(Object->Entitie != DXF_POLYLINE)
							  {
								  dwGroupPosBefore = dwGroupPos;
								  bFinish = TRUE;
							  }
							  else
							  {
								  if(NameGroup(pcValue) == "SEQEND")
								  {
									  bFinish = TRUE;
									  
									  if(Object->Entitie == DXF_POLYLINE && b_ClodedPolyline == TRUE)
									  {
										  pPT = new PointD;
										  if(pPT == NULL) return FALSE;
										  
										  if(Object->Vertex.GetCount() > 0)
										  {
											  PointD tmp = *Object->Vertex.GetHead();
											  
											  pPT->X = tmp.X;
											  pPT->Y = tmp.Y;
											  
											  Object->Vertex.AddTail(pPT);
											  
											  MAX_EXTEND(_extents,tmp.X,tmp.Y);
											  
											  b_ClodedPolyline = FALSE;
										  }
									  }

								  }
								  else if(NameGroup(pcValue) == "VERTEX")
								  {
									  bFirst     = FALSE;
									  bXComplete = FALSE;
									  bYComplete = FALSE;
								  }
							  }
							  break;
						  case 1:
							  Object->Text = pcValue;
							  break;
						  case 5:
							  Object->Handle = atol(pcValue);
							  break;
						  case 8:
							  LayerName = pcValue;
							  break;
						  case 10:
							  pt.X       = strtod(pcValue,&pErro);
							  bXComplete = TRUE;
							  break;
						  case 11:
							  if(Object->Entitie == DXF_LINE)
							  {
								  pt.X       = strtod(pcValue,&pErro);
								  bXComplete = TRUE;
							  }
							  break;
						  case 20:
							  pt.Y       = strtod(pcValue,&pErro);
							  bYComplete = TRUE;

							  if(Object->Entitie == DXF_LINE ||
								  Object->Entitie == DXF_POINT ||
								  Object->Entitie == DXF_TEXT)
							  {
									bFirst = FALSE;
							  }
							  break;
						  case 21:
							  if(Object->Entitie == DXF_LINE)
							  {
								  pt.Y       = strtod(pcValue,&pErro);
								  bYComplete = TRUE;
								  bFirst	 = FALSE;
							  }
							  break;
						  case 40:
							  if(Object->Entitie == DXF_CIRCLE)
							  {
								  Object->Size = strtod(pcValue,&pErro);
								  bFirst	 = FALSE;
							  }
							  break;
						  case 62:
								  Object->ACI = atoi(pcValue);
						  case 50:
							  if(Object->Entitie == DXF_TEXT)
								  Object->Angle = atoi(pcValue);
							  break;
  						  case 70:
							  if(Object->Entitie == DXF_POLYLINE)
							  {
								  if(atoi(pcValue) == 1 ||
									  atoi(pcValue) == 32)
								  {
									  b_ClodedPolyline = TRUE;
								  }
							  }
							  break;

  						  default:
							  break;

						  }
						  
						  if(bFinish == TRUE)
							  break;
						 
					  }

					  DWORD dwCountLayerIndex = 0;
					  POSITION pos = NULL;
					  pos = m_Layers.GetHeadPosition();
					  while (pos != NULL)
					  {
						  CString NameofLayer = m_Layers.GetNext(pos);
						  if(NameofLayer == LayerName)
							  break;

						  dwCountLayerIndex++;
					  }

					  POSITION poslayer = NULL;
					  poslayer = m_Vector.Layers.FindIndex(dwCountLayerIndex);
					  if(poslayer != NULL)
					  {
						  DXFLayer *LayerX = m_Vector.Layers.GetAt(poslayer);
						  LayerX->Objects.AddHead(Object);
						  m_Vector.Layers.SetAt(poslayer,LayerX);
					  }

			  }
			  else if(NameGroup(pcValue) == "LAYER")
			  {
				  BOOL b2 = FALSE,b62 = FALSE,b1 = FALSE;;
				  char Name[256];
				  short ColorACI;
				  BOOL bFinish = FALSE;
				  
				  for (;dwGroupPos<(int)dwSize;)
				  {
					  dwGroupPosBefore = dwGroupPos;

					  pcGroup = ReadLine(pcBuffer,dwSize,dwGroupPos);
					  if (pcGroup == NULL) break;
					  
					  pcValue = ReadLine(pcBuffer,dwSize,dwGroupPos);
					  if (pcValue == NULL) break;
					  
					  dwGroupNumber = atoi(pcGroup);
					  
					  if (b2 == TRUE && b62 == TRUE)
					  {
						  if(m_Layers.Find(Name,m_Layers.GetHeadPosition()) == NULL)
						  {
							  m_Layers.AddTail(Name);

							  DXFLayer *pLayerX = NULL;
							  pLayerX          = new DXFLayer;
							  pLayerX->Name    = Name;
							  pLayerX->ACI     = ColorACI;

							  m_Vector.Layers.AddTail(pLayerX);
							  break;
						  }
					  }

					  switch(dwGroupNumber)
					  {
					  case 0:
						  dwGroupPos = dwGroupPosBefore;
						  bFinish = TRUE;
						  break;
					  case 2:
						  strcpy(Name,pcValue);
						  b2 = TRUE;
						  break;
					  case 62:
						  ColorACI = atoi(pcValue);
						  b62 = TRUE;
						  break;
					  }

					  if(bFinish == TRUE)
						  break;
				  }

				  if(bFinish == TRUE)
					  break;


			  }
			  else if (NameGroup(pcValue) == "BLOCK")
			  {
				  BOOL bFinish = FALSE;

				  for (;dwGroupPos<(int)dwSize;)
				  {
					  dwGroupPosBefore = dwGroupPos;
					  
					  pcGroup = ReadLine(pcBuffer,dwSize,dwGroupPos);
					  if (pcGroup == NULL) break;
					  
					  pcValue = ReadLine(pcBuffer,dwSize,dwGroupPos);
					  if (pcValue == NULL) break;
					  
					  dwGroupNumber = atoi(pcGroup);
					  
					  if ( NameGroup(pcValue) == "ENDBLK")
						  bFinish = TRUE;
					  
					  if(bFinish == TRUE)
						  break;
					  
				  }
				  
				  if(bFinish == TRUE)
					  break;
			  }

		  }
      	  break;
	  }


   }

   delete[] pcBuffer;

   m_bOpened = TRUE;

   return m_bOpened;
}

CString DXFReader::NameGroup(CString string)
{
	string.MakeUpper();
	string.Remove(' ');

	return string;
}

char* DXFReader::ReadLine(char* pcSimbol,int dwTotal,int &dwPosition)
{
   char *pcTemp=pcSimbol+dwPosition;

   for (;dwPosition<dwTotal;dwPosition++)
   {
	   if (pcSimbol[dwPosition] == 0x0d)
		   pcSimbol[dwPosition] = '\0';
	   
	   if (pcSimbol[dwPosition] == 0x0a)
	   {
		   dwPosition++;
		   break;
	   }
   }
   
   if (dwPosition>dwTotal)
	   pcTemp = NULL;
   
   return pcTemp;
}

void DXFReader::Draw(CWnd *pWnd, CDC *pDC)
{
	pDC->SetBkMode(TRANSPARENT);

	DXFLayer *Layer   = NULL;
	DXFObject *Object = NULL;
	POSITION POS1	 = NULL;
	POSITION POS2	 = NULL;
	POSITION POS3	 = NULL;
	PointD strTemp;
	CRect rc = m_pMapmetrics->_canvas;
	
	CRect Circle;
	Circle.left		 = 0;
	Circle.top		 = 0;
	Circle.right	 = 0;
	Circle.bottom	 = 0;
	
	CPoint pt;
	pt.x			 = 0;
	pt.y			 = 0;
	HGDIOBJ     g_hOldPen = NULL;
	
	POS1 = m_Vector.Layers.GetHeadPosition();
	for (;POS1 != NULL;)
	{
		Layer = m_Vector.Layers.GetNext(POS1);
		if(Layer->Visible)
		{
			POS2 = Layer->Objects.GetHeadPosition();
			for (;POS2 != NULL;)
			{
				Object = Layer->Objects.GetNext(POS2);
				
				switch(Object->Entitie)
				{
				case DXF_POINT:
					{
						POS3 = Object->Vertex.GetHeadPosition();
						if(POS3 != NULL)
						{
							pt = m_pMapmetrics->World2Pixel(Object->Vertex.GetNext(POS3));
							
							if (pt.x < rc.left || pt.x > rc.right || pt.y < rc.top || pt.y > rc.bottom)
								continue;
							
							pDC->SetPixel(pt.x,pt.y,Object->ACI != 0 ? Object->ACI : Layer->ACI);
						}
					}
					break;
				case DXF_LINE:
					{
						POS3 = Object->Vertex.GetHeadPosition();
						if(POS3 != NULL)
						{
							int i = 0;
							POINT *pPoint = ::new POINT[Object->Vertex.GetCount()+1];
							
							for (;POS3 != NULL;)
							{
								pPoint[i] = m_pMapmetrics->World2Pixel(Object->Vertex.GetNext(POS3));
								i++;
							}
							
							g_hOldPen = pDC->SelectObject(m_phColorsACI[Object->ACI != 0 ? Object->ACI : Layer->ACI]);
							
							pDC->Polyline(pPoint,Object->Vertex.GetCount());
							
							pDC->SelectObject(g_hOldPen);
							
							::delete []pPoint;
						}
					}
					break;
				case DXF_CIRCLE:
					{
						POS3 = Object->Vertex.GetHeadPosition();
						if(POS3 != NULL)
						{
							strTemp = *Object->Vertex.GetHead();
							strTemp.X -= (int)Object->Size;
							strTemp.Y -= (int)Object->Size;
							
							pt = m_pMapmetrics->World2Pixel(&strTemp);
							
							Circle.left = pt.x;
							Circle.top  = pt.y;
							
							strTemp = *Object->Vertex.GetHead();
							strTemp.X += (int)Object->Size;
							strTemp.Y += (int)Object->Size;
							
							pt = m_pMapmetrics->World2Pixel(&strTemp);
							
							Circle.right  = pt.x;
							Circle.bottom = pt.y;
							
							g_hOldPen = pDC->SelectObject(m_phColorsACI[Object->ACI != 0 ? Object->ACI : Layer->ACI]);
							
							pDC->Ellipse(Circle.left,Circle.top,Circle.right,Circle.bottom);
							pDC->SelectObject(g_hOldPen);
						}
					}
					break;
				case DXF_POLYLINE:
					{
						POS3 = Object->Vertex.GetHeadPosition();
						if(POS3 != NULL)
						{
							int i = 0;
							POINT *pPoint = ::new POINT[Object->Vertex.GetCount()+1];
							
							for (;POS3 != NULL;)
							{
								pPoint[i] = m_pMapmetrics->World2Pixel(Object->Vertex.GetNext(POS3));
								i++;
							}
							
							g_hOldPen = pDC->SelectObject(m_phColorsACI[Object->ACI != 0 ? Object->ACI : Layer->ACI]);
							
							pDC->Polyline(pPoint,Object->Vertex.GetCount());
							
							pDC->SelectObject(g_hOldPen);
							
							::delete []pPoint;
						}
					}
					break;
				case DXF_TEXT:
					{
						POS3 = Object->Vertex.GetHeadPosition();
						if(POS3 != NULL)
						{
							pt = m_pMapmetrics->World2Pixel(Object->Vertex.GetNext(POS3));
							
							if (pt.x < rc.left || pt.x > rc.right || pt.y < rc.top || pt.y > rc.bottom)
								continue;
							
							int FontSize = -((((int)Object->Size)*::GetDeviceCaps(pDC->m_hDC,LOGPIXELSY))/72);
							
							if (FontSize == 0)
								FontSize = 1;
							
							LOGFONT lf = {FontSize,0,Object->Angle,0,300,0,0,0,0,1,2,1,34, "Arial"};
							
							HGDIOBJ hFont    = ::CreateFontIndirect(&lf);
							HGDIOBJ hOldFont = pDC->SelectObject(hFont);
							
							LOGPEN lp;
							m_phColorsACI[Object->ACI != 0 ? Object->ACI :Layer->ACI].GetLogPen(&lp);
							
							pDC->SetTextColor(lp.lopnColor);
							pDC->SetTextAlign(TA_LEFT|TA_BASELINE);
							pDC->TextOut(pt.x, pt.y, Object->Text, Object->Text.GetLength()); 
							
							pDC->SelectObject(hOldFont);
							::DeleteObject(hFont);
						}
					}
					
					break;
					
				}
			}
		}
	}
}

void DXFReader::DrawLabels()
{
}

CString DXFReader::QueryPoint( int x, int y, CString PipeFields )
{
	return CString("");
}

CString DXFReader::QueryObject(DWORD Handle)
{
	return CString("");
}

void DXFReader::SelectObject(DWORD Handle)
{
}

void DXFReader::ClearSelected()
{
}

BOOL DXFReader::GetRectObject(DWORD Handle, RectangleD *rc)
{
	return FALSE;
}