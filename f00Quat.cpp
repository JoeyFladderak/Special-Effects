#include <windows.h>
#include <math.h>
#include <assert.h>

#include "f00Quat.h"


f00Quat::f00Quat()
{
	reset();
}


f00Quat::f00Quat( const f00Quat& copyFrom)
{
	set( copyFrom );
}


f00Quat::f00Quat( GLfloat x, GLfloat y, GLfloat z )
{
	set( x, y, z );
}


f00Quat::f00Quat( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
	set( angle, x, y, z );
}


f00Quat& f00Quat::reset()
{
	// The multiplication 'identity' quaternion.  Why not?  :-)
	d_val[ 0 ] = 1.0;
	d_val[ 1 ] = 0.0;
	d_val[ 2 ] = 0.0;
	d_val[ 3 ] = 0.0;

	return *this;
}


f00Quat& f00Quat::set( const f00Quat& copyFrom )
{
	d_val[ 0 ] = copyFrom.d_val[ 0 ];
	d_val[ 1 ] = copyFrom.d_val[ 1 ];
	d_val[ 2 ] = copyFrom.d_val[ 2 ];
	d_val[ 3 ] = copyFrom.d_val[ 3 ];
	return *this;
}


f00Quat& f00Quat::set( GLfloat x, GLfloat y, GLfloat z )
{
	f00Quat xQ( x, 1.0, 0.0, 0.0 );
	f00Quat yQ( y, 0.0, 1.0, 0.0 );
	f00Quat zQ( z, 0.0, 0.0, 1.0 );

	set( xQ );
	postMult( yQ );
	postMult( zQ );
	return *this;
}


f00Quat& f00Quat::set( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
	// Convert to radians.
	angle *= (GLfloat)( M_PI / 180.0 );

	// Normalize the axis.
	GLfloat factor = x * x + y * y + z * z;
	assert( factor != 0 );

	GLfloat scaleBy((GLfloat)( 1.0 / sqrt( factor ) ));
	x = x * scaleBy;
	y = y * scaleBy;
	z = z * scaleBy;

	// Build a quaternion!
	d_val[ 0 ] = (GLfloat)cos( angle / 2.0 );

	GLfloat sinHalfAngle( (GLfloat)(sin( angle / 2.0 ) ));
	d_val[ 1 ] = x * sinHalfAngle;
	d_val[ 2 ] = y * sinHalfAngle;
	d_val[ 3 ] = z * sinHalfAngle;

	return *this;
}


f00Quat& f00Quat::postMult( const f00Quat& quat )
{
	f00Quat tempQ( *this );
	multAndSet( tempQ, quat );
	return *this;
}


f00Quat& f00Quat::multAndSet( const f00Quat& quat1, const f00Quat& quat2 )
{
	d_val[ 0 ] =	  quat2.d_val[ 0 ] * quat1.d_val[ 0 ]
						- quat2.d_val[ 1 ] * quat1.d_val[ 1 ]
						- quat2.d_val[ 2 ] * quat1.d_val[ 2 ]
						- quat2.d_val[ 3 ] * quat1.d_val[ 3 ];

	d_val[ 1 ] =	  quat2.d_val[ 0 ] * quat1.d_val[ 1 ]
						+ quat2.d_val[ 1 ] * quat1.d_val[ 0 ]
						+ quat2.d_val[ 2 ] * quat1.d_val[ 3 ]
						- quat2.d_val[ 3 ] * quat1.d_val[ 2 ];

	d_val[ 2 ] =	  quat2.d_val[ 0 ] * quat1.d_val[ 2 ]
						- quat2.d_val[ 1 ] * quat1.d_val[ 3 ]
						+ quat2.d_val[ 2 ] * quat1.d_val[ 0 ]
						+ quat2.d_val[ 3 ] * quat1.d_val[ 1 ];

	d_val[ 3 ] =	  quat2.d_val[ 0 ] * quat1.d_val[ 3 ]
						+ quat2.d_val[ 1 ] * quat1.d_val[ 2 ]
						- quat2.d_val[ 2 ] * quat1.d_val[ 1 ]
						+ quat2.d_val[ 3 ] * quat1.d_val[ 0 ];

	return *this;
}


f00Quat& f00Quat::normalize()
{
	GLfloat factor =	d_val[ 0 ] * d_val[ 0 ] +
							d_val[ 1 ] * d_val[ 1 ] +
							d_val[ 2 ] * d_val[ 2 ] +
							d_val[ 3 ] * d_val[ 3 ];
	assert( factor != 0 );

	GLfloat scaleBy((GLfloat)( 1.0 / sqrt( factor ) ));

	d_val[ 0 ] = d_val[ 0 ] * scaleBy;
	d_val[ 1 ] = d_val[ 1 ] * scaleBy;
	d_val[ 2 ] = d_val[ 2 ] * scaleBy;
	d_val[ 3 ] = d_val[ 3 ] * scaleBy;

	return *this;
}


void f00Quat::getMatrix( GLfloat matrix[ 16 ] )
{
	normalize();

	GLfloat w = d_val[ 0 ];
	GLfloat x = d_val[ 1 ];
	GLfloat y = d_val[ 2 ];
	GLfloat z = d_val[ 3 ];

	GLfloat xx = x * x;
	GLfloat yy = y * y;
	GLfloat zz = z * z;

	#define M( x, y )  matrix[ x + y * 4 ]

	M( 0, 0 ) = (GLfloat)(1.0 - 2.0 * ( yy + zz ));
	M( 1, 0 ) = (GLfloat)(2.0 * ( x * y + w * z ));
	M( 2, 0 ) = (GLfloat)(2.0 * ( x * z - w * y ));
	M( 3, 0 ) = (GLfloat)(0.0);

	M( 0, 1 ) = (GLfloat)(2.0 * ( x * y - w * z ));
	M( 1, 1 ) = (GLfloat)(1.0 - 2.0 * ( xx + zz ));
	M( 2, 1 ) = (GLfloat)(2.0 * ( y * z + w * x ));
	M( 3, 1 ) = (GLfloat)(0.0);

	M( 0, 2 ) = (GLfloat)(2.0 * ( x * z + w * y ));
	M( 1, 2 ) = (GLfloat)(2.0 * ( y * z - w * x ));
	M( 2, 2 ) = (GLfloat)(1.0 - 2.0 * ( xx + yy ));
	M( 3, 2 ) = (GLfloat)(0.0);

	M( 0, 3 ) = (GLfloat)(0.0);
	M( 1, 3 ) = (GLfloat)(0.0);
	M( 2, 3 ) = (GLfloat)(0.0);
	M( 3, 3 ) = (GLfloat)(1.0);

	#undef M
}


void f00Quat::getAxisAngle( GLfloat& axisX, GLfloat& axisY, GLfloat& axisZ, GLfloat& rotAngle )
{
	#define DEF_EPSILON			0.0001
	GLfloat lenOfVector2( d_val[ 1 ] * d_val[ 1 ] + d_val[ 2 ] * d_val[ 2 ] + d_val[ 3 ] * d_val[ 3 ] );
	if( lenOfVector2 < DEF_EPSILON )
	{
		axisX = 1.0;
		axisY = 0.0;
		axisZ = 0.0;
		rotAngle = 0.0;
	}
	else
	{
		GLfloat invLen((GLfloat)( 1.0 / sqrt( lenOfVector2 ) ));
		axisX = d_val[ 1 ] * invLen;
		axisY = d_val[ 2 ] * invLen;
		axisZ = d_val[ 3 ] * invLen;
		rotAngle = (GLfloat)(2.0 * acos( d_val[ 0 ] ));
	}
}


#include <stdio.h>
void f00Quat::getLatLong( GLfloat& latAngle, GLfloat& longAngle )
{
	GLfloat dirX;
	GLfloat dirY;
	GLfloat dirZ;
	getDirectionVector( dirX, dirY, dirZ );

	#define DEF_EPSILON 0.0001
	latAngle = (GLfloat)(asin( dirY ) * ( 180.0 / M_PI ));
	if( fabs( dirY ) > ( 1.0 - DEF_EPSILON ) )
	{
		// Pointing straight up or down, the longitude makes no difference.
		longAngle = 0.0;
	}
	else
	{
		if( fabs( dirZ ) < DEF_EPSILON )
			longAngle = (GLfloat)(( dirX > 0.0 ? 90.0 : 270.0 ));
		else
		{
			if( dirZ >= 0.0 )
				longAngle = (GLfloat)(atan( dirX / dirZ ) * ( 180.0 / M_PI ));
			else
				longAngle = (GLfloat)(180.0 + atan( dirX / dirZ ) * ( 180.0 / M_PI ));

			if( longAngle < 0.0 )
				longAngle += 360.0;
		}
	}
}


void f00Quat::getDirectionVector( GLfloat& dirX, GLfloat& dirY, GLfloat& dirZ )
{
	normalize();

	GLfloat w = d_val[ 0 ];
	GLfloat x = d_val[ 1 ];
	GLfloat y = d_val[ 2 ];
	GLfloat z = d_val[ 3 ];

	// Optimized out of the 'getMatrix' function, above.
	dirX = (GLfloat)(2.0 * ( x * z - w * y ));
	dirY = (GLfloat)(2.0 * ( y * z + w * x ));
	dirZ = (GLfloat)(1.0 - 2.0 * ( x * x + y * y ));
}


#define DELTA 1e-6     // error tolerance

void f00Quat::setInterpolated_SLERP(const f00Quat& qFrom, const f00Quat& qTo, GLfloat t)
{
    GLfloat           to1[4];
    GLdouble          omega, cosom, sinom;
    GLdouble          scale0, scale1;

    // calc cosine
    cosom = qFrom.d_val[1] * qTo.d_val[1] + qFrom.d_val[2] * qTo.d_val[2] + qFrom.d_val[3] * qTo.d_val[3] + qFrom.d_val[0] * qTo.d_val[0];

    // adjust signs (if necessary)
    if ( cosom < 0.0 )
	{
		cosom = -cosom;

		to1[0] = - qTo.d_val[1];
		to1[1] = - qTo.d_val[2];
		to1[2] = - qTo.d_val[3];
		to1[3] = - qTo.d_val[0];

    } 
	else  
	{
		to1[0] = qTo.d_val[1];
		to1[1] = qTo.d_val[2];
		to1[2] = qTo.d_val[3];
		to1[3] = qTo.d_val[0];
    }

    // calculate coefficients

    if ( (1.0 - cosom) > DELTA ) 
	{
        // standard case (slerp)
        omega = acos(cosom);
        sinom = sin(omega);
        scale0 = sin((1.0 - t) * omega) / sinom;
        scale1 = sin(t * omega) / sinom;

    } else {        
		// "qFrom" and "to" quaternions are very close 
		//  ... so we can do a linear interpolation

        scale0 = 1.0 - t;
        scale1 = t;
    }

	// calculate final values
	d_val[1] = (GLfloat) (scale0 * qFrom.d_val[1] + scale1 * to1[0]);
	d_val[2] = (GLfloat) (scale0 * qFrom.d_val[2] + scale1 * to1[1]);
	d_val[3] = (GLfloat) (scale0 * qFrom.d_val[3] + scale1 * to1[2]);
	d_val[0] = (GLfloat) (scale0 * qFrom.d_val[0] + scale1 * to1[3]);
}


void f00Quat::setInterpolated_NAIVE(const f00Quat& qFrom, const f00Quat& qTo, GLfloat t)
{
	for(int i=0;i<4;i++)
	{
		d_val[i] = qFrom.d_val[i] + (qTo.d_val[i] - qFrom.d_val[i])*t;
	}
}
