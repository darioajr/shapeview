#if !defined(_DBASEREADER_)
#define _DBASEREADER_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GIS.DbaseReaderIndex.h"

using MapGIS::DbaseReaderIndex;

namespace MapGIS
{
	
	class DbaseReader : public DbaseReaderIndex  
	{
	public:
		DbaseReader(const CString& filename);
		virtual ~DbaseReader();
		
		CString operator [](int index);
		
		CString GetRecord(int index, CString PipeFields);
	};
}

#endif // !defined(_DBASEREADER_)
