#include "stdafx.h"

//GIS
#include "GIS.DbaseReaderIndex.h"
#include "GIS.FileStream.h"
#include "GIS.BinaryReader.h"

using MapGIS::FileStream;
using MapGIS::BinaryReader;
using MapGIS::DbaseReaderIndex;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

DbaseReaderIndex::DbaseReaderIndex()
{
	m_psFieldNames		= NULL;
	m_pftFieldTypes		= NULL;
	m_pbyFieldLengths	= NULL;
}

DbaseReaderIndex::~DbaseReaderIndex()
{
	if(m_psFieldNames)
		delete[] m_psFieldNames;
	if(m_pftFieldTypes)
		delete[] m_pftFieldTypes;
	if(m_pbyFieldLengths)
		delete[] m_pbyFieldLengths;
}

BOOL DbaseReaderIndex::ReadHeader(const CString& filename)
{
	BOOL bReturn		= TRUE;
	FileStream *fs		= NULL;
	BinaryReader *br	= NULL;

	TRY
	{
		m_sFilename =  filename.Left(filename.GetLength()-4);
		m_sFilename += _T(".dbf");

		fs = new FileStream(m_sFilename, FileStream::Open);
		br = new BinaryReader(fs);
		
		m_byDbType = br->ReadByte();

		if ( m_byDbType == 0x03 )
		{
			m_byUpdateYear		= br->ReadByte();
			m_byUpdateMonth		= br->ReadByte();
			m_byUpdateDay		= br->ReadByte();
			m_uiRecordCount		= br->ReadUInt32();
			m_usHeaderLength	= br->ReadUInt16();
			m_usRecordLength	= br->ReadUInt16();
			
			m_uiFieldCount = (unsigned int) ( ( m_usHeaderLength - 1 ) / 32 ) - 1;
			
			m_psFieldNames		= new CString[m_uiFieldCount];
			m_pftFieldTypes		= new FieldType[m_uiFieldCount];
			m_pbyFieldLengths	= new byte[m_uiFieldCount];
			
			for (unsigned int a=0;a < m_uiFieldCount;++a)
			{
				if(fs->Seek(32 + (32 * a), 0))
				{
					for ( int b=0; b < 11; ++b )
						m_psFieldNames[a] += (char)br->ReadByte();
					
					m_pftFieldTypes[a] = (FieldType)br->ReadByte();
					
					br->ReadByte();
					br->ReadByte();
					br->ReadByte();
					br->ReadByte();

					m_pbyFieldLengths[a] = br->ReadByte();
				}
			}
		}
		
		m_uiRecordStart = ( m_uiFieldCount * 32 ) + 32 + 1;
		
		if(!fs->Seek((m_uiRecordStart-1), 0))
			bReturn = FALSE;

		if(br->ReadByte() != 0x0D)
			bReturn = FALSE;
	}
	CATCH(CFileException, pEx)
	{
		pEx->ReportError();
		bReturn = FALSE;
	}
	AND_CATCH(CMemoryException, pEx)
	{
		pEx->ReportError();
		bReturn = FALSE;
	}
	END_CATCH

	if(fs)
		delete fs;
	if(br)
		delete br;

	return bReturn;
}

		
