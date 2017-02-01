#include "stdafx.h"

#include "GIS.SHPReader.h"
#include "GIS.SHPThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using MapGIS::SHPReader;
using MapGIS::ShapeRecord;
using MapGIS::DbaseReader;

#define PI 3.1415926535897932384626433832795

#include "math.h" 

SHPReader::SHPReader()
{
	_extents = new RectangleD;
	Inicialize();
	m_LayerType		= Vector;
	m_LayerSubType	= Shapefile;
	m_pThread		= NULL;
	m_bThread		= FALSE;
}

SHPReader::SHPReader(CString filename, MapMetrics *mapmetrics, CBrush *brush, CBrush *brushactive, CPen *pen, CPen *penselected)
{
	m_phBrush		= brush;
	m_phBrushActive	= brushactive;
	m_phPen			= pen;
	m_phPenSelected = penselected;
	_mapmetrics		= mapmetrics;
	_extents		= new RectangleD;

	m_LayerType		= Vector;
	m_LayerSubType	= Shapefile;
	m_pThread		= NULL;
	m_bThread		= FALSE;

	Inicialize();
	Open(filename);
}

SHPReader::~SHPReader()
{
	DeleteThread();

	Finalize();

	delete _extents;
}

BOOL SHPReader::Inicialize()
{
	m_sShapeFilename = _T("");
	m_pShapeHeader	 = NULL;

	return TRUE;
}

BOOL SHPReader::Finalize()
{
	TRY
	{
		m_sShapeFilename = _T("");

		if (m_pShapeHeader != NULL)
		{
			delete  m_pShapeHeader;
		}

		FreeAllRecords();

		return TRUE;
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	AND_CATCH(CMemoryException, pEx)
	{
		AfxAbort();
	}
	END_CATCH

	return FALSE;
}

void SHPReader::FreeAllRecords()
{
	m_Vector.RemoveAll();
}

DWORD SHPReader::GetCountShape()
{
	return m_Vector.GetCount();
}

BOOL SHPReader::ReadFile()
{
	FileStream *fs = NULL;
	BinaryReader *br = NULL;

	TRY
	{
		int shapeType	= 0;
		int num			= 0;
		int size		= 0;
		int shapeCount  = 0;
		
		fs = new FileStream(m_sShapeFilename, FileStream::Open);
		br = new BinaryReader(fs);

		ReadHeader(br);
		
		// Le registros do arquivo
		DWORD len = fs->GetLength();
		while (fs->GetPosition() < len)
		{
			// Le numero do registro
			num	= br->ReadInt32();
			SwapWord(sizeof(num),&num); //BigEndian

			// Le tamanho do registro
			size = br->ReadInt32();
			SwapWord(sizeof(size),&size); //BigEndian

			// Le tipo de Shape
			shapeType	= br->ReadInt32();
			
			switch((ShapeType)shapeType)
			{
			case SHAPE_NULL:
				ReadShapeNull(br, num,size);
				break;
			case SHAPE_POINT:
				ReadShapePoint(br, num,size);	
				break;
			case SHAPE_ARC:
				ReadShapePolyline(br, num,size);	
				break;
			case SHAPE_POLYGON:
				ReadShapePolygon(br, num,size);	
				break;
			case SHAPE_MULTIPOINT:
				break;
			default:
				break;
			}
			
			////TRACE("Registro ->%d\n", num);
			////TRACE("Ponteiro Arquivo ->%d\n", fs->GetPosition());
		}
		
		////TRACE("Numero de shapes <%d>\n",shapeCount);
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	AND_CATCH(CMemoryException, pEx)
	{
		AfxAbort();
	}
	END_CATCH

	////TRACE(_T("Registros %d  \n"), GetCountShape());

	if(fs&&br)
	{
		delete br;
		delete fs;
	}

	return TRUE;
}

BOOL SHPReader::ReadHeader(BinaryReader *br)
{
	TRY
	{
		m_pShapeHeader->FileCode = br->ReadInt32();
		SwapWord(sizeof(m_pShapeHeader->FileCode), &m_pShapeHeader->FileCode);

		////TRACE(_T("File Code \t: %d\n"), m_pShapeHeader->FileCode);
		
		//Nao usado
		br->ReadInt32();
		br->ReadInt32();
		br->ReadInt32();
		br->ReadInt32();
		br->ReadInt32();

		m_pShapeHeader->FileLength = br->ReadInt32();
		SwapWord(sizeof(m_pShapeHeader->FileLength), &m_pShapeHeader->FileLength);

		////TRACE(_T("File Length \t: %d\n"), m_pShapeHeader->FileLength);
		
		m_pShapeHeader->Version = br->ReadInt32();
		////TRACE(_T("File Version \t: %d\n"), m_pShapeHeader->Version);
		
		m_pShapeHeader->m_ShapeType = (ShapeType)br->ReadInt32();
		////TRACE(_T("ShapeType \t: %d\n"), m_pShapeHeader->m_ShapeType);
		
		m_pShapeHeader->xMin = br->ReadDouble();
		m_pShapeHeader->yMin = br->ReadDouble();
		//TRACE (_T("XMin <%f> \t YMin <%f>\n"), m_pShapeHeader->xMin, m_pShapeHeader->yMin);
		
		m_pShapeHeader->xMax = br->ReadDouble();
		m_pShapeHeader->yMax = br->ReadDouble();
		//TRACE (_T("XMax <%f> \t YMax <%f>\n"), m_pShapeHeader->xMax, m_pShapeHeader->yMax);
		
		m_pShapeHeader->zMin = br->ReadDouble();
		m_pShapeHeader->zMax = br->ReadDouble();
		//TRACE (_T("ZMin <%f> \t ZMax <%f>\n"), m_pShapeHeader->zMin, m_pShapeHeader->zMax);
		
		m_pShapeHeader->mMin = br->ReadDouble();
		m_pShapeHeader->mMax = br->ReadDouble();
		//TRACE (_T("MMin <%f> \t MMax <%f>\n"), m_pShapeHeader->mMin, m_pShapeHeader->mMax);

		_extents->setXmin(m_pShapeHeader->xMin);
		_extents->setXmax(m_pShapeHeader->xMax);
		_extents->setYmin(m_pShapeHeader->yMin);
		_extents->setYmax(m_pShapeHeader->yMax);
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	AND_CATCH(CMemoryException, pEx)
	{
		AfxAbort();
	}
	END_CATCH


	return TRUE;
}

BOOL SHPReader::ReadShapePolygon(BinaryReader *br, int num, int size)
{
	TRY
	{
		ShapeRecord *thisRec = new ShapeRecord;
		thisRec->m_Data = new ShapePolygon;
		
		((ShapePolygon*)thisRec->m_Data)->m_ShapeType = SHAPE_POLYGON;
		thisRec->m_RecordNum	    = num;
		thisRec->m_Length			= size;
		thisRec->m_ShapeType		= SHAPE_POLYGON;

		((ShapePolygon*)thisRec->m_Data)->xMin = br->ReadDouble();
		((ShapePolygon*)thisRec->m_Data)->yMin = br->ReadDouble();
		((ShapePolygon*)thisRec->m_Data)->xMax = br->ReadDouble();
		((ShapePolygon*)thisRec->m_Data)->yMax = br->ReadDouble();
		
		//TRACE("Shape Polygon\n");
		//TRACE("XMin\t<%g>\tXMax\t<%g>\n", ((ShapePolygon*)thisRec->m_Data)->xMin, ((ShapePolygon*)thisRec->m_Data)->xMax);
		//TRACE("YMin\t<%g>\tYMax\t<%g>\n", ((ShapePolygon*)thisRec->m_Data)->yMin, ((ShapePolygon*)thisRec->m_Data)->yMax);
		
		//Partes
		int parts=0;
		parts = br->ReadInt32();
		//TRACE("Numero de Partes\t<%d>\n",parts);
		
		//Pontos
		int points=0;
		points = br->ReadInt32();
		//TRACE("Numero de Pontos\t<%u>\n",points);

		//Le partes
		for (int j = 0; j != parts; j++)
		{
			int temp=0;
			temp = br->ReadInt32();
			((ShapePolygon*)thisRec->m_Data)->AddParts(temp);
			//TRACE("Parte %d = %d\n", j, temp);
		}
		
		//Le pontos
		for (int k = 0; k != points; k++)
		{
			PointD *p = new PointD;
			p->X = br->ReadDouble();
			p->Y = br->ReadDouble();
			((ShapePolygon*)thisRec->m_Data)->AddPoints(p);
			//TRACE("Ponto %d  Shape <%d> X <%g> Y <%g>\n", k, ((ShapePolygon*)thisRec->m_Data)->m_ShapeType, p->X, p->Y);
		}

		AddRecord(thisRec);
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	AND_CATCH(CMemoryException, pEx)
	{
		AfxAbort();
	}
	END_CATCH

	return TRUE;
}

BOOL SHPReader::ReadShapeNull(BinaryReader *br, int num, int size)
{
	return TRUE;
}

BOOL SHPReader::ReadShapePoint(BinaryReader *br, int num, int size)
{
	TRY
	{
		ShapeRecord *thisRec = new ShapeRecord;
		thisRec->m_Data = new ShapePoint;
		
		((ShapePoint*)thisRec->m_Data)->m_ShapeType = SHAPE_POINT;
		thisRec->m_RecordNum	    = num;
		thisRec->m_Length			= size;
		thisRec->m_ShapeType		= SHAPE_POINT;

		((ShapePoint*)thisRec->m_Data)->point->X = br->ReadDouble();
		((ShapePoint*)thisRec->m_Data)->point->Y = br->ReadDouble();
		
		//TRACE("Shape Point\n");
		//TRACE("X\t<%g>\tY\t<%g>\n", ((ShapePoint*)thisRec->m_Data)->point->X, ((ShapePoint*)thisRec->m_Data)->point->Y);
		
		AddRecord(thisRec);
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	AND_CATCH(CMemoryException, pEx)
	{
		AfxAbort();
	}
	END_CATCH

	return TRUE;
}

BOOL SHPReader::ReadShapePolyline(BinaryReader *br, int num, int size)
{
	return TRUE;
}

BOOL SHPReader::ReadShapeMultiPoint(BinaryReader *br, int num, int size)
{
	return TRUE;
}

void SHPReader::AddRecord(ShapeRecord *r)
{
	m_Vector.Records.AddTail(r);
}

BOOL SHPReader::Close()
{
	Finalize();
	Inicialize();

	return FALSE;
}

BOOL SHPReader::IsOpen()
{
	return m_Vector.GetCount() > 0 ? TRUE : FALSE;
}

BOOL SHPReader::Open(const CString filename)
{
	if(IsOpen())
		return false;

	BOOL returnVal = FALSE;

	TRY
	{
		m_pShapeHeader = new ShapeHeader;
		m_sShapeFilename = filename;

		//TRACE(_T("Arquivo : %s\n"), m_sShapeFilename);

		returnVal = ReadFile();
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	AND_CATCH(CMemoryException, pEx)
	{
		AfxAbort();
	}
	END_CATCH

	return returnVal;
}

CString SHPReader::GetRecord(int index, CString PipeField)
{
	CString sName = m_sShapeFilename;
	sName = sName.Mid(0,sName.GetLength()-4);
	sName += _T(".dbf");

	DbaseReader db(sName);
	return db.GetRecord(index,PipeField);
}

void SHPReader::SelectObject(DWORD Handle)
{
	POSITION POS = m_Vector.Records.FindIndex(Handle);
	if(POS)
	{
		ShapeRecord* rec = m_Vector.Records.GetAt(POS);
		rec->m_Selected = TRUE;
		m_Vector.Records.SetAt(POS,rec);
	}
}

void SHPReader::ClearSelected()
{
	POSITION POS = m_Vector.Records.GetHeadPosition();
	while(POS)
	{
		ShapeRecord* rec = m_Vector.Records.GetAt(POS);
		rec->m_Selected = FALSE;
		m_Vector.Records.SetAt(POS,rec);
		m_Vector.Records.GetNext(POS);
	}
}

BOOL SHPReader::GetRectObject(DWORD Handle, RectangleD *rc)
{
	POSITION POS = m_Vector.Records.FindIndex(Handle);
	if(POS)
	{
		ShapeRecord* rec = m_Vector.Records.GetAt(POS);
		rc->setXmin(((ShapePolygon*)rec->m_Data)->xMin);
		rc->setXmax(((ShapePolygon*)rec->m_Data)->xMax);
		rc->setYmin(((ShapePolygon*)rec->m_Data)->yMin);
		rc->setYmax(((ShapePolygon*)rec->m_Data)->yMax);
		return TRUE;
	}
	return FALSE;
}

void SHPReader::DrawLabels()
{
}

CString SHPReader::QueryPoint( int x, int y, CString PipeFields )
{
	long id = -1;
	ClearSelected();
	//ObjetoMaisPerto(x,y,id);
	ObjetoDentro(x,y,id);
	if(id >= 0)
	{
		SelectObject(id);
		return GetRecord(id,PipeFields);
	}
	return "";
}

CString SHPReader::QueryObject(DWORD Handle)
{
	return GetRecord(Handle,"");
}

void SHPReader::SwapWord( int length, void *wordP )
{
    INT	i;
    UCHAR temp;

    for( i=0; i < length/2; i++ )
    {
		temp = ((UCHAR *) wordP)[i];
		((UCHAR *)wordP)[i] = ((UCHAR *) wordP)[length-i-1];
		((UCHAR *) wordP)[length-i-1] = temp;
    }
}

void SHPReader::DeleteThread()
{
	if(m_pThread)
	{
		VERIFY(SetEvent(m_pThread->m_hEventKill));
		
		WaitForSingleObject(GDIThread::m_hAnotherDead, INFINITE);
		
		Sleep(1*2);
		
		WaitForSingleObject(m_pThread->m_hEventDead, 0);
		
		VERIFY(WaitForSingleObject(m_pThread->m_hThread, INFINITE) == WAIT_OBJECT_0);
		
		delete m_pThread;
		
		m_pThread = NULL;
	}
}

void SHPReader::Draw(CWnd *pWnd, CDC *pDC)
{
	if(m_bThread)
	{
		DeleteThread();
		
		m_pThread = new SHPThread(pWnd, pDC->m_hDC, m_phPen, m_phPenSelected, m_phBrush,_mapmetrics,&m_Vector);
		
		if (m_pThread == NULL)
			return;
		
		ASSERT_VALID(m_pThread);
		m_pThread->m_pThreadParams = NULL;
		
		// Create Thread in a suspended state so we can set the Priority
		// before it starts getting away from us
		if (!m_pThread->CreateThread(CREATE_SUSPENDED))
		{
			delete m_pThread;
			return;
		}
		
		// If you want to make the sample more sprightly, set the thread priority here
		// a little higher. It has been set at idle priority to keep from bogging down
		// other apps that may also be running.
		VERIFY(m_pThread->SetThreadPriority(THREAD_PRIORITY_IDLE));
		// Now the thread can run wild
		m_pThread->ResumeThread();
	}
	else
	{
		POSITION POS1	 = NULL;
		POSITION POS2	 = NULL;
		POSITION POS3	 = NULL;
		
		pDC->SetBkMode(TRANSPARENT);
		
		ShapeRecord *pRecord = NULL;
		
		POS1 = m_Vector.Records.GetHeadPosition();
		for (;POS1 != NULL;)
		{
			pRecord = m_Vector.Records.GetNext(POS1);
			
			switch(pRecord->m_ShapeType)
			{
			case SHAPE_NULL:
				break;
			case SHAPE_POINT:
				{
					POINT pt = _mapmetrics->World2Pixel(((ShapePoint*)pRecord->m_Data)->point->X,((ShapePoint*)pRecord->m_Data)->point->Y);
					pDC->SetPixel(pt,RGB(0,0,0));
				}
				break;
			case SHAPE_POLYGON:
				{
					POS2 = ((ShapePolygon*)pRecord->m_Data)->m_parts.GetHeadPosition();
					if(POS2 != NULL)
					{
						DWORD dwConta = 0;
						dwConta = ((ShapePolygon*)pRecord->m_Data)->m_points.GetCount();
						int i = 0;
						POINT *pPoint = ::new POINT[dwConta+1];
						POS3 = ((ShapePolygon*)pRecord->m_Data)->m_points.GetHeadPosition();
						for (;POS3 != NULL;i++)
						{
							PointD  *p = ((ShapePolygon*)pRecord->m_Data)->m_points.GetNext(POS3);
							pPoint[i] = _mapmetrics->World2Pixel(p->X,p->Y);
						}
						
						CPen* pOldPen = NULL;
						pOldPen = pRecord->m_Selected ? pDC->SelectObject(m_phPenSelected) : pDC->SelectObject(m_phPen);

						CBrush *m_phBrushSelected = new CBrush(RGB(255,0,0)); 
						
						CBrush* pBrush = NULL;

						if(m_bActive)
							pBrush = pRecord->m_Selected ? pDC->SelectObject( m_phBrushSelected ) : pDC->SelectObject(m_phBrushActive);
						else
							pBrush = pRecord->m_Selected ? pDC->SelectObject( m_phBrushSelected ) : pDC->SelectObject(m_phBrush);
						
						pDC->Polygon(pPoint,dwConta);
						
						pDC->SelectObject( pOldPen );
						pDC->SelectObject( pBrush );
						::DeleteObject(m_phBrushSelected);
						
						::delete []pPoint;
						delete m_phBrushSelected;
					}
				}
				break;
			}
		}
		
	}
}

void SHPReader::ObjetoDentro(int x, int y, long &id)
{
	PointD *ptx = NULL;
	ptx = new PointD(x,y);

	_mapmetrics->Pixel2World(ptx->X,ptx->Y);

   	ShapeRecord *pRecord = NULL;
	POSITION POS1	 = NULL;
	POS1 = m_Vector.Records.GetHeadPosition();
	int i = 0;	
	for (i=0;POS1 != NULL;i++)
	{
		pRecord = m_Vector.Records.GetNext(POS1);
		
		switch(pRecord->m_ShapeType)
		{
		case SHAPE_NULL:
			break;
		case SHAPE_POINT:
			break;
		case SHAPE_POLYGON:
			{
				if(((ShapePolygon*)pRecord->m_Data)->PointIn(ptx))
				{
					id = i;
					break;
				}	
			}
			break;
		}
    }
	delete ptx;
}

void SHPReader::ObjetoMaisPerto(int x, int y, UINT &id)
{
	PointD *pt1,*pt2,*ptx;
	ptx = new PointD(x,y);

	_mapmetrics->Pixel2World(ptx->X,ptx->Y);

	double hMaisPerto = 9999999999.9;
	
   	ShapeRecord *pRecord = NULL;
	POSITION POS1	 = NULL;
	POSITION POS2	 = NULL;
	POSITION POS3	 = NULL;
	int i = 0;	
	POS1 = m_Vector.Records.GetHeadPosition();
	for (i=0;POS1 != NULL;i++)
	{
		pRecord = m_Vector.Records.GetNext(POS1);
			
			switch(pRecord->m_ShapeType)
			{
			case SHAPE_NULL:
				break;
			case SHAPE_POINT:
				break;
			case SHAPE_POLYGON:
				{
					POS2 = ((ShapePolygon*)pRecord->m_Data)->m_parts.GetHeadPosition();
					if(POS2 != NULL)
					{
						DWORD dwConta = 0;
						dwConta = ((ShapePolygon*)pRecord->m_Data)->m_points.GetCount();
						POS3 = ((ShapePolygon*)pRecord->m_Data)->m_points.GetHeadPosition();
						for (;POS3 != NULL;)
						{
							pt1 = ((ShapePolygon*)pRecord->m_Data)->m_points.GetNext(POS3);
							if (POS3 == NULL) break;
							pt2 = ((ShapePolygon*)pRecord->m_Data)->m_points.GetAt(POS3);

							double dA = sqrt(pow(ptx->X-pt1->X,2)+pow(ptx->Y-pt1->Y,2));
							double dB = sqrt(pow(ptx->X-pt2->X,2)+pow(ptx->Y-pt2->Y,2));
							double Ang1 = azA(*pt1,*pt2,*ptx),Ang2 = azB(*pt1,*pt2,*ptx);
							double h = dA*sin(Ang1*PI/180);
							
							if (fabs(h) < hMaisPerto && (Ang1 < 90 || Ang1 > 270) && (Ang2 < 90 || Ang2 > 270))
							{
								hMaisPerto = fabs(h);
								id = i;
							}
							else if (fabs(dA) < hMaisPerto)
							{
								hMaisPerto = fabs(dA);
								id = i;
							}
							else if (fabs(dB) < hMaisPerto)
							{
								hMaisPerto = fabs(dB);
								id = i;
							}

						}
					}
				}
				break;
         }
      }
	delete ptx;
}

double SHPReader::azA(PointD ptA,PointD ptB,PointD ptP)
{
   double DX1 = ptA.X - ptP.X;
   double DY1 = ptA.Y - ptP.Y;
   double AZ1 = (DY1 != 0) ? atan(fabs(DX1)/fabs(DY1))*180/PI : 90;
   if (DX1 < 0)
   {
      if (DY1 < 0)
         AZ1 += 180;
      else
         AZ1 = 360-AZ1;
   }
   else if (DY1 < 0)
      AZ1 = 180-AZ1;
   
   double DX2 = ptB.X - ptA.X;
   double DY2 = ptB.Y - ptA.Y;
   double AZ2 = (DY2 != 0) ? atan(fabs(DX2)/fabs(DY2))*180/PI : 90;
   if (DX2 < 0)
   {
      if (DY2 < 0)
         AZ2 += 180;
      else
         AZ2 = 360-AZ2;
   }
   else if (DY2 < 0)
      AZ2 = 180-AZ2;
   
   double Ang = AZ2 - AZ1 + 180;
   for (;Ang < 0;) Ang = Ang + 360;
   for (;Ang >= 360;) Ang = Ang - 360;

   return Ang;
}

double SHPReader::azB(PointD ptA,PointD ptB,PointD ptP)
{
   double DX1 = ptB.X - ptP.X;
   double DY1 = ptB.Y - ptP.Y;
   double AZ1 = (DY1 != 0) ? atan(fabs(DX1)/fabs(DY1))*180/PI : 90;
   if (DX1 < 0)
   {
      if (DY1 < 0)
         AZ1 += 180;
      else
         AZ1 = 360-AZ1;
   }
   else if (DY1 < 0)
      AZ1 = 180-AZ1;
   
   double DX2 = ptA.X - ptB.X;
   double DY2 = ptA.Y - ptB.Y;
   double AZ2 = (DY2 != 0) ? atan(fabs(DX2)/fabs(DY2))*180/PI : 90;
   if (DX2 < 0)
   {
      if (DY2 < 0)
         AZ2 += 180;
      else
         AZ2 = 360-AZ2;
   }
   else if (DY2 < 0)
      AZ2 = 180-AZ2;
   
   double Ang = AZ2 - AZ1 + 180;
   for (;Ang < 0;) Ang = Ang + 360;
   for (;Ang >= 360;) Ang = Ang - 360;

   return Ang;
}


	//pDC->FillSolidRect(rand()%255,rand()%255,rand()%255,rand()%255,RGB(0,0,0));
	/*
	GenerateDrawList();
							
			for ( int a=0; a<NUM_THREADS; ++a )
			{
				if ( a == NUM_THREADS - 1 )
				{
					drawThreads[ a ] = new RenderThread( _mapMetrics, _features,
						_ShapeType, a * threadBoundary, _features.Length, _pen, _brush );
				}
				else
				{
					drawThreads[ a ] = new RenderThread( _mapMetrics, _features, 
						_ShapeType, a * threadBoundary, (a + 1) * threadBoundary, _pen, _brush );
				}

				threads[ a ] = new Thread( new ThreadStart( drawThreads[a].Start ) );
				threads[ a ].Start();
			}

			for ( int b=0; b<NUM_THREADS; ++b )
			{
				threads[b].Join();
			}

			// if a feature has been highlighted with a query
			
			if ( _highlightedFeature != -1 )
			{
				this.Highlight( _highlightedFeature, SystemColors.Highlight );
			}
*/


/*
		/// <summary>
		/// Using the LabelSourceField property of this class, we label 
		/// each VectorFeature that is in view with the data contained
		/// in that field of the .dbf record.
		/// </summary>
		public void DrawLabels()
		{
			Graphics gr = Graphics.FromImage( _mapMetrics.Canvas );
			gr.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAliasGridFit;
			ArrayList labelList = new ArrayList();

			
			if ( _ShapeType == ShapeType.Point )
			{					
				int posx;
				int posy;
				bool drawLabel;
				string tempLabel;

				for ( int a=0; a<_features.Length; a++ )
				{
					if ( _features[a].IsInViewport == false ) continue;
					
					drawLabel = true;
					tempLabel = _db[a][_labelSourceField];
					tempLabel.Trim();

					SizeF fontSize = gr.MeasureString( tempLabel, _labelFont );
					
					posx = (int)((_features[a][0][0].X + (_pen.Width /2)) - ( fontSize.Width / 2 ));
					posy = (int)(_features[a][0][0].Y - ( fontSize.Height ));

					Rectangle labelArea = new Rectangle( posx, posy,
						(int) fontSize.Width, (int) fontSize.Height );

					gr.ResetTransform();
					gr.TranslateTransform( (float) posx, (float) posy );

					labelArea.Inflate( 2, 2 );
					
					foreach ( Rectangle ll in labelList )
					{
						if ( ll.IntersectsWith( labelArea ) ) drawLabel = false;
					}
					
					if ( drawLabel == false ) 
					{
						continue;
					}
					else
					{
						labelList.Add( labelArea );
					}

					if ( _labelBackground == true && !tempLabel.StartsWith(" ") )
					{
						gr.FillRectangle( new SolidBrush( _labelFillColor ), 0, 0, 
							fontSize.Width, fontSize.Height );
						gr.DrawRectangle( new Pen( _labelForeColor, 1f ), 0, 0, 
							fontSize.Width, fontSize.Height );
					}
				
					gr.DrawString( tempLabel, _labelFont, 
						new SolidBrush( _labelForeColor ), 0, 0 );
				}
			}


			if ( _ShapeType == ShapeType.Polygon ||
				  _ShapeType == ShapeType.Multipoint )
			{
				double centerx;
				double centery;
				bool drawLabel;
				string tempLabel;

				for ( int a=0; a<_features.Length; a++ )
				{
					if ( _features[a].IsInViewport == false ) continue;
					
					drawLabel = true;
					tempLabel = _db[a][_labelSourceField];
					tempLabel.Trim();

					SizeF fontSize = gr.MeasureString( tempLabel, _labelFont );
				
					centerx = _features[a].LabelAnchor.X;
					centery = _features[a].LabelAnchor.Y;
			
					_mapMetrics.World2Pixel( ref centerx, ref centery );

					centerx = (int)( centerx - ( fontSize.Width / 2 ));
					centery = (int)( centery - ( fontSize.Height / 2 ));

					Rectangle labelArea = new Rectangle( (int)centerx, (int)centery,
						(int)fontSize.Width, (int)fontSize.Height );

					gr.ResetTransform();
					gr.TranslateTransform( (float) centerx, (float) centery );

					labelArea.Inflate( 2, 2 );

					foreach ( Rectangle ll in labelList )
					{
						if ( ll.IntersectsWith( labelArea ) ) drawLabel = false;
					}
					
					if ( drawLabel == false ) 
					{
						continue;
					}
					else
					{
						labelList.Add( labelArea );
					}

					if ( _labelBackground == true && !tempLabel.StartsWith(" ") )
					{
						gr.FillRectangle( new SolidBrush( _labelFillColor ), 0, 0, 
							fontSize.Width, fontSize.Height );
						gr.DrawRectangle( new Pen( _labelForeColor, 1f ), 0, 0, 
							fontSize.Width, fontSize.Height );
					}
			
					gr.DrawString( tempLabel, _labelFont, 
						new SolidBrush( _labelForeColor ), 0, 0 );
				}
			}

			
			if ( _ShapeType == ShapeType.Line )
			{
				double centerx;
				double centery;
				bool drawLabel;
				string tempLabel;
				
				for ( int a=0; a<_features.Length; a++ )
				{
					if ( _features[a].IsInViewport == false ) continue;
					
					drawLabel = true;
					tempLabel = _db[a][_labelSourceField];
					tempLabel.Trim();

					SizeF fontSize = gr.MeasureString( tempLabel, _labelFont );

					centerx = _features[a].LabelAnchor.X;
					centery = _features[a].LabelAnchor.Y;
			
					_mapMetrics.World2Pixel( ref centerx, ref centery );

					Rectangle labelArea = new Rectangle( (int)(centerx -fontSize.Width / 2), (int)(centery -fontSize.Width / 2),
						(int)fontSize.Width, (int)fontSize.Height );

					gr.ResetTransform();

					gr.TranslateTransform( (float)centerx, (float)centery );
					gr.RotateTransform( _features[a].FeatureAngle );
					gr.TranslateTransform( -fontSize.Width / 2, fontSize.Height * .20f );
					
					

					labelArea.Inflate( 10, 20 );
					
					foreach ( Rectangle ll in labelList )
					{
						if ( labelArea.IntersectsWith( ll ) ) 
						{
							drawLabel = false;
						}							
					}
					
					if ( drawLabel == false ) 
					{
						continue;
					}
					else
					{
						labelList.Add( labelArea );
					}

					if ( _labelBackground == true && !tempLabel.StartsWith(" ") )
					{
						gr.FillRectangle( new SolidBrush( _labelFillColor ), 0, 0, 
							fontSize.Width, fontSize.Height );
						gr.DrawRectangle( new Pen( _labelForeColor, 1f ), 0, 0, 
							fontSize.Width, fontSize.Height );
					}

					gr.DrawString( tempLabel, _labelFont, 
						new SolidBrush( _labelForeColor ), 0, 0 );
				}			
			}                
				gr.Dispose();
		}

*/
/*
void SHPReader::GenerateDrawList()
{				
	if ( _mapMetrics->_viewport->Xmin() == _mapMetrics->_extents->Xmin() &&
		_mapMetrics->_viewport->Ymin() == _mapMetrics->_extents->Ymin() &&
		_mapMetrics->_viewport->Xmax() == _mapMetrics->_extents->Xmax() &&
		_mapMetrics->_viewport->Ymax() == _mapMetrics->_extents->Ymax() )
	{
		POSITION POS = m_Vector.GetHeadPosition();
		while(POS)
		{
			ShapeRecord* rec = m_Vector.GetAt(POS);
			rec->IsInViewport = TRUE;
			m_Vector.SetAt(POS, rec);
			m_Vector.GetNext(POS);
		}
		
		return;
	}
	
	if ( m_ShapeType == ShapeType::SHAPE_POINT )
	{
		for ( int a=0; a < _FeatureCount; ++a )
		{
			_features[ a ].IsInViewport = 
				_mapMetrics.Viewport.Contains( _features[ a ].GetPoint( 0, 0 ) );
		}
	}
	else
	{			
		for ( int a=0; a < _FeatureCount; ++a )
		{
			_features[ a ].IsInViewport = _mapMetrics.Viewport.Intersects( _features[ a ].Extents );
		}
	}
}


		
		/// <summary>
		/// Highlight a geographic object.
		/// </summary>
		/// @param record_number Array index of the record to be highlighted.
		/// @param c The color to use to highlight the record.
		private void Highlight(int record_number, Color c)
		{
			RenderThread drawThread;
			Pen tempPen = (Pen) _pen.Clone();
			tempPen.Color = c;
			
			if ( _ShapeType == ShapeType.Line )
			{
				tempPen.Width += 2;
				tempPen.StartCap = LineCap.RoundAnchor;
				tempPen.EndCap = LineCap.RoundAnchor;
			}

			HatchBrush tempBrush = new HatchBrush( HatchStyle.Percent70, c, Color.Transparent );

			drawThread = new RenderThread( _mapMetrics, _features,
				_ShapeType, record_number, record_number + 1, tempPen, tempBrush );

			drawThread.Start();
			
			tempPen.Dispose();
			tempBrush.Dispose();
		}
*/
		
/*				
CString QueryPoint( int x, int y )
{
	int closestRecord = 0;
	double closestDistance = MAXDOUBLE;
	double testDistance = 0;
	double dx = (double) x;
	double dy = (double) y;

	_mapMetrics->Pixel2World( dx, dy );
	
	for( int a=0; a<_FeatureCount; ++a )
	{
		if( !_features[a].IsInViewport ) continue;
		
		testDistance = _features[a].DistanceFromFeature( new PointD( dx, dy ) );
		
		if( testDistance < closestDistance )
		{
			closestDistance = testDistance;
			closestRecord = a;
		}
	}
	
	_highlightedFeature = closestRecord;
	return _db[ closestRecord ];
}
*/		