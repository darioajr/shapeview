#if !defined(_POINTD_)
#define _POINTD_

#include "stdafx.h"

namespace MapGIS
{
	class PointD : public CObject
	{
	public:
		double X;
		double Y;
		
	public:
		PointD()
		{
			X = 0.0;
			Y = 0.0;
		}

		void Reset()
		{
			X = 0.0;
			Y = 0.0;
		}
		
		PointD( const double X, const double Y )
		{
			this->X = X;
			this->Y = Y;
		}
		
		PointD( const PointD &P )
		{
			X = P.X;
			Y = P.Y;
		}
		
		BOOL operator > ( const PointD& value )
		{
			return (X >= value.X && Y <= value.Y) ? true : false;
		}
		
		BOOL operator < ( const PointD &value)
		{
			return (X <= value.X && Y >= value.Y) ? true : false;
		}
		
		PointD& operator = ( const PointD& value)
		{
			X = value.X;
			Y = value.Y;
			return *this;
		}

		BOOL operator == (const PointD& value)
		{
			return (X == value.X && Y >= value.Y) ? true : false;
		}
	};
}

#endif //_POINTD_