#include "stdafx.h"

//GIS
#include "GIS.ShapeReaderIndex.h"
#include "GIS.FileStream.h"
#include "GIS.BinaryReader.h"

#include "DASmartPtr.h"

using MapGIS::FileStream;
using MapGIS::BinaryReader;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ShapeIndex::ShapeIndex( string filename, MapMetrics metrics )
{			
	_Filename		= filename;
	_mapMetrics		= metrics;
	_extents		= new RectangleD();
	
	ParseHeader( filename );
	ReadIndex( filename );

	LoadFile();

	SetupLabels();
}

ShapeIndex::~ShapeIndex()
{

}

void LoadFile()
{			
	RefCountPtr<FileStream> fs   = new FileStream( _Filename, FileStream::Open );
	RefCountPtr<BinaryReader> br = new BinaryReader( fs );

	VectorFeature tempFeature;
	int[] segments;
	int segmentPosition;
	int pointsInSegment;
	PointD[] tempPoints;
	PointD[] segmentPoints;
	
	_features = new VectorFeature[ _FeatureCount ];
	
	
	if ( _ShapeType == ShapeType.Point )
	{
		for ( int a=0; a < _FeatureCount; ++a ) 
		{
			// Point types don't have parts (segments) / one point per feature
			tempFeature = new VectorFeature( 1, _mapMetrics );
			tempPoints = new PointD[ 1 ];
			
			fs.Seek( _OffsetOfRecord[ a ], 0 );
			
			br.ReadInt32(); //Record number (not needed)
			br.ReadInt32(); //Content length (not needed)
			br.ReadInt32(); //Shape type (not needed)
			tempPoints[ 0 ] = new PointD( br.ReadDouble(), br.ReadDouble() );
			
			tempFeature.AddSegment( tempPoints );
			_features[ a ] = tempFeature;
		}
	}
	else
	{			
		for ( int a=0; a < _FeatureCount; ++a ) 
		{
			fs.Seek( _OffsetOfRecord[ a ] + 44, 0 );
			
			// Read the number of parts (segments) and create a new VectorFeature
			tempFeature = new VectorFeature( br.ReadInt32(), _mapMetrics ); 
			
			fs.Seek( _OffsetOfRecord[ a ], 0 );
			
			br.ReadInt32(); //Record number (not needed)
			br.ReadInt32(); //Content length (not needed)
			br.ReadInt32(); //Shape type (not needed)
			tempFeature.Extents.Xmin = br.ReadDouble();
			tempFeature.Extents.Ymin = br.ReadDouble();
			tempFeature.Extents.Xmax = br.ReadDouble();
			tempFeature.Extents.Ymax = br.ReadDouble();
			br.ReadInt32(); // Number of parts (segments) gotten earlier
			tempPoints = new PointD[ br.ReadInt32() ]; // Number of points
			
			segments = new int[ tempFeature.SegmentCount + 1 ];
			
			//Read in the segment indexes
			for ( int b=0; b<tempFeature.SegmentCount; ++b )
			{
				segments[ b ] = br.ReadInt32();
			}
			
			//Read in *ALL* of the points in the feature
			for ( int c=0; c<tempPoints.Length; ++c )
			{
				tempPoints[ c ] = new PointD( br.ReadDouble(), br.ReadDouble() );
			}
			
			//Add in an ending point for the inner loop that follows (e) 
			segments[ tempFeature.SegmentCount ] = tempPoints.Length;
			
			//Watch your step...
			for ( int d=0; d<tempFeature.SegmentCount; ++d )
			{
				pointsInSegment = segments[ d+1 ] - segments[ d ];
				segmentPoints = new PointD[ pointsInSegment ];
				segmentPosition = 0;
				
				for ( int e=segments[ d ]; e<segments[ d+1 ]; ++e )
				{
					segmentPoints[ segmentPosition ] = tempPoints[ e ];
					++segmentPosition;
				}
				
				tempFeature.AddSegment( segmentPoints );
			}
			
			_features[ a ] = tempFeature;
		}
	}
	
	_OffsetOfRecord	= null;  //don't need the info anymore
	_LengthOfRecord	= null;
	
	GC.Collect();
	
	br.Close();
	fs.Close();
}
		
void ShapeIndex::ParseHeader( CString filename )
{
	filename.Delete(filename.GetLength() - 4, 4);
	filename += ".shx";
	
	RefCountPtr<FileStream> fs = new FileStream( filename, FileStream::Open );
	RefCountPtr<BinaryReader> br = new BinaryReader( fs );
	
	int naousado=0;

	naousado		= br->ReadInt32();
	naousado		= br->ReadInt32();
	naousado		= br->ReadInt32();
	naousado		= br->ReadInt32();
	naousado		= br->ReadInt32();
	naousado		= br->ReadInt32();
	_IndexFileSize 	= 2* SwitchByteOrder(br->ReadDouble());
	_Version 		= br->ReadDouble();
	_ShapeType 		= (ShapeType)br->ReadDouble();
	_extents.Xmin 	= br->ReadInt32();
	_extents.Ymin 	= br->ReadInt32();
	_extents.Xmax 	= br->ReadInt32();
	_extents.Ymax 	= br->ReadInt32();
	
	br->Close();
	fs->Close();
}
		
void ShapeIndex::ReadIndex( CString filename )
{
	filename.Delete(filename.GetLength() - 4, 4);
	filename += ".shx";
	
	RefCountPtr<FileStream> fs = new FileStream( filename, FileStream::Open );
	RefCountPtr<BinaryReader> br = new BinaryReader( fs );

	int ibuffer;
	
	_FeatureCount		= ( _IndexFileSize - 100 ) / 8;
	
	_OffsetOfRecord		= new int[ _FeatureCount ];
	_LengthOfRecord		= new int[ _FeatureCount ];
	
	fs->Seek( 100, 0 );
	
	for ( int x=0; x < _FeatureCount; ++x ) 
	{
		ibuffer = br->ReadInt32(); 		
		_OffsetOfRecord[ x ] = 2* SwitchByteOrder( ibuffer );
		
		ibuffer = br->ReadInt32();				
		_LengthOfRecord[ x ] = ( 2* SwitchByteOrder( ibuffer ) ) + 8;
	}
	
	br->Close();
	fs->Close();
}

private void SetupLabels()
{
	double minx;
	double maxx;
	double miny;
	double maxy;
	PointD[] tmpPoints;
	int midPoint;
	int endPoint;
	int segToDraw =0;
	int maxSegPoints =0;
	
	if ( _ShapeType == ShapeType.Polygon ||
		_ShapeType == ShapeType.Multipoint )
	{
		for ( int a=0; a< _features.Length; ++a )
		{
			// Optimal situation
			if ( _features[a].SegmentCount == 1 )
			{
				_features[a].LabelAnchor = new PointD( 
					_features[a].Extents.CenterX, 
					_features[a].Extents.CenterY );
				
				continue;
			} 
			else 
			{
				for ( int b=0; b<_features[a].SegmentCount; ++b )
				{
					tmpPoints = _features[a].GetPoints(b);
					
					if ( tmpPoints.Length > maxSegPoints )
					{
						maxSegPoints = tmpPoints.Length;
						segToDraw = b;
					}
				}
				tmpPoints = _features[ a ].GetPoints( segToDraw );
				
				PointD p1 = tmpPoints[ 0 ];
				PointD p2 = tmpPoints[ (int)tmpPoints.Length /2 ];
				
				if ( p1.X < p2.X )
				{
					minx = p1.X;
					maxx = p2.X;
				} 
				else 
				{
					minx = p2.X;
					maxx = p1.X;
				}
				
				
				if ( p1.Y < p2.Y )
				{
					miny = p1.Y;
					maxy = p2.Y;
				} 
				else 
				{
					miny = p2.Y;
					maxy = p1.Y;
				}
				
				_features[a].LabelAnchor = new PointD( minx + (( maxx - minx)/2), miny + (( maxy - miny)/2) );
				segToDraw =0;
				maxSegPoints =0;
			}
		}
	}
	
	if ( _ShapeType == ShapeType.Line )
	{
		double dx, dy;
		double tmpx, tmpy;
		double angle = 0.0;
		
		for ( int a=0; a< _features.Length; ++a )
		{										
			tmpPoints = _features[a].GetPoints(0);
			endPoint = tmpPoints.Length - 1;
			
			// Figure out which point to use as an anchor for the label					
			midPoint = ( tmpPoints.Length -1 ) / 2;
			dx = tmpPoints[midPoint +1].X - tmpPoints[midPoint].X;
			dy = tmpPoints[midPoint +1].Y - tmpPoints[midPoint].Y;
			
			tmpx = tmpPoints[midPoint].X + ( dx / 2 );
			tmpy = tmpPoints[midPoint].Y + ( dy / 2 );
			
			_features[a].LabelAnchor = new PointD( tmpx, tmpy );
			
			// calculate angle of line					
			angle = -Math.Atan(dy / dx) + Math.PI / 2d;
			angle *= (180d / Math.PI); // convert radians to degrees
			
			_features[a].FeatureAngle = (float) angle - 90; // -90 text orientation
		}
	}
}

int ShapeIndex::SwitchByteOrder (int i) 
{
	int reverso;

	byte k1[4] = {0};
	byte k2[4] = {0};

	memcpy(k1,&i,sizeof(int));
	
	for (int j=0;j<4;j++)
		k2[j] = k1[3-j];

	memcpy(&reverso, k2,4);

	return reverso;
}
