#if !defined(_RETANGLED_)
#define _RETANGLED_

#include "stdafx.h"

#include "GIS.PointD.h"

namespace MapGIS
{
	class RectangleD : public CObject
	{
	public:
		PointD UL;
		PointD LR;
		
	public:
		RectangleD()
		{
			UL = PointD( 0.0, 0.0 );
			LR = PointD( 0.0, 0.0 );
		}
		
		RectangleD( const double ul_x, const double ul_y, const double lr_x, const double lr_y )
		{
			UL = PointD( ul_x, ul_y );
			LR = PointD( lr_x, lr_y );
		}

		RectangleD( const PointD ul, const PointD lr )
		{
			UL = ul;
			LR = lr;
		}

		RectangleD( const RectangleD &rect )
		{
			UL = rect.UL;
			LR = rect.LR;
		}

		void Reset()
		{
			UL.Reset();
			LR.Reset();
		}

		double CenterX()
		{
			return Xmin() + ( DeltaX() / 2 );
		}

		double CenterY()
		{
			return Ymin() + ( DeltaY() / 2 );
		}

		bool Contains( const double X, const double Y )
		{
			return Contains(PointD( X, Y ));
		}
				
		bool Contains( PointD &point )
		{
			return ( point > UL && point < LR ) ? true : false;
		}

		bool Intersects( RectangleD &rect )
		{
			if ( Contains( rect.UL ) )
				return true;

			if ( Contains( rect.LR ) )
				return true;
			
			if ( rect.Xmin() <= Xmax() &&
				 rect.Xmax() >= Xmax() )
			{
				if ( Ymax() >= rect.Ymax() &&
					Ymin() <= rect.Ymax())
				{
					return true;
				}

				if ( Ymax() >= rect.Ymin() &&
					Ymin() <= rect.Ymin() )
				{
					return true;
				}
			}

			if ( rect.Xmin() <= Xmin() &&
				rect.Xmax() >= Xmin() )
			{
				if ( Ymax() >= rect.Ymax()&&
					Ymin() <= rect.Ymax())
				{
					return true;
				}

				if ( Ymax() >= rect.Ymin() &&
					Ymin() <= rect.Ymin() )
				{
					return true;
				}
			}

			if ( rect.Ymin() <= Ymax()&&
				rect.Ymax() >= Ymax())
			{
				if ( Xmax() >= rect.Xmax()&&
					Xmin() <= rect.Xmax())
				{
					return true;
				}

				if ( Xmax() >= rect.Xmin() &&
					Xmin() <= rect.Xmin() )
				{
					return true;
				}
			}

			if ( rect.Ymin() <= Ymin() &&
				rect.Ymax() >= Ymin() )
			{
				if ( Xmax() >= rect.Xmax()&&
					Xmin() <= rect.Xmax())
				{
					return true;
				}

				if ( Xmax() >= rect.Xmin() &&
					Xmin() <= rect.Xmin() )
				{
					return true;
				}
			}
			
			if ( rect.UL < UL &&
				rect.LR > LR )
			{
				return true;
			}

			return false;
		}

		void FixGeoAspect()
		{
			if ( DeltaX() > DeltaY() )
			{
				double tmpCenter = CenterY();
				setYmin(tmpCenter - ( ( DeltaX() ) / 2 ));
				setYmax(tmpCenter + ( ( DeltaX() ) / 2 ));
			}
			else
			{
				double tmpCenter = CenterX();
				setXmin(tmpCenter - ( ( DeltaY() ) / 2 ));
				setXmax(tmpCenter + ( ( DeltaY() ) / 2 ));
			}
		}

		void setXmin(const double value)
		{
			UL.X = value;
		}
		double Xmin()
		{
			return UL.X;
		}
		
		double Ymin()
		{
			return LR.Y;
		}
		void setYmin(const double value)
		{
			LR.Y = value;
		}

		double Xmax()
		{
			return LR.X;
		}
		void setXmax(const double value)
		{
			LR.X = value;
		}
		
		double Ymax()
		{
			return UL.Y;
		}
		void setYmax(const double value)
		{
			UL.Y = value;
		}

		double DeltaX()
		{
			return LR.X - UL.X;
		}

		double DeltaY()
		{
			return UL.Y - LR.Y;
		}
		
		RectangleD& operator = ( const RectangleD& value)
		{
			UL = value.UL;
			LR = value.LR;
			return *this;
		}
	};
}

#endif //_RETANGLED_