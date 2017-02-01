#include "stdafx.h"

//GIS
#include "GIS.DbaseReader.h"
#include "GIS.BinaryReader.h"
#include "GIS.FileStream.h"

using MapGIS::BinaryReader;
using MapGIS::FileStream;
using MapGIS::DbaseReader;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


DbaseReader::DbaseReader(const CString& filename)
{
	ReadHeader(filename);
}

DbaseReader::~DbaseReader()
{
}

CString DbaseReader::operator [](int index)
{
	return GetRecord(index,"");
}

CString DbaseReader::GetRecord(int index, CString PipeFields)
{
	CString sReturn;
	FileStream *fs		= NULL;
	BinaryReader *br	= NULL;
	
	TRY
	{
		fs = new FileStream(m_sFilename, FileStream::Open);
		br = new BinaryReader(fs);
		
		if ( index < (int)m_uiRecordCount )
			fs->Seek( m_uiRecordStart + ( index * m_usRecordLength ), 0 );
		else
			return sReturn;
		
		br->ReadByte();  // excluido
		
		for ( unsigned int a=0; a < m_uiFieldCount; ++a )
		{
			switch(m_pftFieldTypes[a])
			{
			case FieldType::C: //Caracter
			case FieldType::c:
				
			case FieldType::D: //Data
			case FieldType::d:
				
			case FieldType::F: //Ponto Flutuante
			case FieldType::f:
				
			case FieldType::L: //Lógico
			case FieldType::l:
				
			case FieldType::N: //Numérico
			case FieldType::n:
				
			default:
				{
					char *chars = new char[m_pbyFieldLengths[ a ]+1];
					memset(chars,0,m_pbyFieldLengths[ a ]);
					
					for(int i = 0; i < m_pbyFieldLengths[ a ]; i++) 
						chars[i] = (char)br->ReadByte();
					
					chars[m_pbyFieldLengths[ a ]] = 0;
					
					if(PipeFields.GetLength() == 0 ||
						PipeFields.Find(m_psFieldNames[a]) != -1)
					{
						sReturn += chars;
						sReturn += _T("|");
					}
					
					delete[] chars;
				}
				break;
			}
		}
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
	}
	AND_CATCH(CMemoryException, pEx)
	{
		pEx->ReportError();
	}
	END_CATCH

	if(fs)
		delete fs;
	if(br)
		delete br;
		
	return sReturn;
}
