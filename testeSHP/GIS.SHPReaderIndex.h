#if !defined(_SHAPEINDEX_)
#define _SHAPEINDEX_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ShapeIndex : public CObject  
{
public:
	enum ShapeType 
	{
		Null		= 0,
		Point		= 1,
		Line		= 3,
		Polygon		= 5,
		Multipoint	= 8 
	};

	ShapeIndex();
	virtual ~ShapeIndex();

protected:
	int 			_Version;
	ShapeType 		_ShapeType;
	CString 		_Filename;
	int 			_IndexFileSize;
	RectangleD		_extents;

	int*			_OffsetOfRecord;
	int*			_LengthOfRecord;
	int				_FeatureCount;
		
	VectorFeature*	_features;
	MapMetrics		_mapMetrics;

private:
	void ParseHeader( CString filename );
	void ReadIndex( CString filename );
	int SwitchByteOrder (int i);
	void SetupLabels();

};

#endif // !defined(_SHAPEINDEX_)
