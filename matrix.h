#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "vector3.h"

#define PI				3.141592653589793238462f

class matrix
{
public:
	enum 
	{ 
		TX=3, 
		TY=7, 
		TZ=11, 
		D0=0, D1=5, D2=10, D3=15, 
		SX=D0, SY=D1, SZ=D2, 
		W=D3 
	};
	matrix() { Identity(); }
	float& operator [] ( int a_N ) { return cell[a_N]; }
	float& GetCell( int i, int j ) { return cell[i+j*4]; }
	void Identity();
	void Rotate( vector3 a_Pos, float a_RX, float a_RY, float a_RZ );
	void RotateX( float a_RX );
	void RotateY( float a_RY );
	void RotateZ( float a_RZ );
	void Translate( vector3 a_Pos ) { cell[TX] += a_Pos.x; cell[TY] += a_Pos.y; cell[TZ] += a_Pos.z; }
	void SetTranslation( vector3 a_Pos ) { cell[TX] = a_Pos.x; cell[TY] = a_Pos.y; cell[TZ] = a_Pos.z; }
	void Normalize();
	void Concatenate( matrix& m2 );
	vector3 Transform( const vector3& v );
	vector3 GetTranslation() { return vector3(cell[TX], cell[TY], cell[TZ]); }
	void Invert();
	float cell[16];
};

#endif