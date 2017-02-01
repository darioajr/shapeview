#if !defined(_DBASEREADERINDEX_)
#define _DBASEREADERINDEX_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace MapGIS
{
	class DbaseReaderIndex : public CObject  
	{
	public:
		DbaseReaderIndex();
		virtual ~DbaseReaderIndex();
		
		enum FieldType
		{
			C = 0x43, D = 0x44, F = 0x46, N = 0x4E, L = 0x4C,
			c = 0x63, d = 0x64, f = 0x66, n = 0x6E, l = 0x6C,
		};
		
		BOOL ReadHeader(const CString& filename);
		
	protected:
		CString* m_psFieldNames;
		FieldType* m_pftFieldTypes;
		byte* m_pbyFieldLengths;
		unsigned int m_uiRecordCount;
		unsigned short m_usRecordLength;
		unsigned int m_uiRecordStart;
		unsigned int m_uiFieldCount; 
		CString m_sFilename; 
		
	private:
		byte m_byDbType;
		byte m_byUpdateYear;
		byte m_byUpdateMonth;
		byte m_byUpdateDay;
		unsigned short m_usHeaderLength;
	};
}

#endif // !defined(_DBASEREADERINDEX_)
