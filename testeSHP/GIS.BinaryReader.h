#if !defined(_BINARYREADER_)
#define _BINARYREADER_

#include "stdafx.h"

//GIS
#include "GIS.FileStream.h"
using MapGIS::FileStream;

namespace MapGIS
{
	class BinaryReader : public CObject
	{
	public:
		FileStream *m_pFile;
	public:
		BinaryReader(FileStream *pFile) : m_pFile(NULL)
		{
			setFileStream(pFile);
		}

		~BinaryReader()
		{ 
		}

		BinaryReader() : m_pFile(NULL)
		{ 
		}

		UINT32 ReadUInt32()
		{
			UINT32 i=0;
			m_pFile->Read(&i,sizeof(UINT32));
			return i;
		}
		
		USHORT ReadUInt16()
		{
			USHORT i=0;
			m_pFile->Read(&i,sizeof(USHORT));
			return i;
		}

		SHORT ReadInt16()
		{
			SHORT i=0;
			m_pFile->Read(&i,sizeof(SHORT));
			return i;
		}

		INT32 ReadInt32()
		{
			INT32 i=0;
			m_pFile->Read(&i,sizeof(INT32));
			return i;
		}
		
		BYTE ReadByte()
		{
			byte b;
			m_pFile->Read(&b,sizeof(BYTE));
			return b;
		}

		BYTE* ReadBytes(int size,BYTE* b)
		{
			m_pFile->Read(b,size*sizeof(BYTE));
			return b;
		}

		CHAR* ReadChars(int size,CHAR* b)
		{
			m_pFile->Read(b,size*sizeof(CHAR));
			return b;
		}
		
		CHAR ReadChar()
		{
			CHAR b;
			m_pFile->Read(&b,sizeof(CHAR));
			return b;
		}

		CString ReadString(int size)
		{
			CString s;
			CHAR *b = new CHAR[size];
			m_pFile->Read(b,size*sizeof(CHAR));
			s = b;
			delete[] b;
			return s;
		}

		DOUBLE ReadDouble()
		{
			DOUBLE d=0.0;
			m_pFile->Read(&d,sizeof(DOUBLE));
			return d;
		}

		BOOL setFileStream(FileStream *pFile)
		{ 
			ASSERT(pFile);
			m_pFile = pFile;
			return m_pFile ? TRUE : FALSE;
		}
		
		BOOL Open(FileStream *pFile)
		{
			return setFileStream(pFile);
		}

		BOOL IsOpen()
		{
			return m_pFile ? TRUE : FALSE;
		}

		BOOL Close()
		{
			m_pFile = NULL;
			return TRUE;
		}
	};
}
#endif //_BINARYREADER_