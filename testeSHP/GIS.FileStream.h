#if !defined(_FILESTREAM_)
#define _FILESTREAM_

#include "stdafx.h"

namespace MapGIS
{
	class FileStream : public CFile
	{
	public:
		typedef enum{ Open = CFile::modeRead|CFile::shareDenyNone } FileMode;

	public:
		FileStream(const CString &FileName, FileMode Mode)
		{
			CFile::Open(FileName, (UINT)Mode);
		}
			
		~FileStream()
		{ 
			CFile::Close();
		}
	};
}
#endif //_FILESTREAM_