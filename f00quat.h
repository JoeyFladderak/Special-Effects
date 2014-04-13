#ifndef f00QuatH
#define f00QuatH

/*
	Quaternion code (C) "f00Dave" (f00Dave@bigfoot.com)
*/


#include <windows.h>
#include <gl/gl.h>
#include "vector3.h"

#ifndef M_PI
	#define M_PI					3.141592653
#endif

class f00Quat
{
	public:

		f00Quat();

		f00Quat( const f00Quat& copyFrom );

		f00Quat( GLfloat x, GLfloat y, GLfloat z );

		f00Quat( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );


		f00Quat&
		reset();

		f00Quat&
		set( const f00Quat& copyFrom );

		f00Quat&
		set( GLfloat x, GLfloat y, GLfloat z );

		f00Quat&
		set( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );


		f00Quat&
		postMult( const f00Quat& quat );

		f00Quat&
		multAndSet( const f00Quat& quat1, const f00Quat& quat2 );


		f00Quat&
		normalize();

		void
		getMatrix( GLfloat matrix[ 16 ] );

		void
		getAxisAngle( GLfloat& axisX, GLfloat& axisY, GLfloat& axisZ, GLfloat& rotAngle );

		void
		getLatLong( GLfloat& latAngle, GLfloat& longAngle );

		void
		getDirectionVector( GLfloat& dirX, GLfloat& dirY, GLfloat& dirZ );

		friend vector3 operator * (const vector3& v, const f00Quat& q)
		{
			vector3 uv, uuv;
			vector3 qvec(-q.d_val[1], -q.d_val[2], -q.d_val[3]);
			uv = qvec.Cross(v);
			uuv = qvec.Cross(uv);
			uv *= (2.0f * q.d_val[0]);
			uuv *= 2.0f;

			return v + uv + uuv;
		}

		inline f00Quat f00Quat::Inverse() const
		{
			return f00Quat(-d_val[1], -d_val[2], -d_val[3]);
		}

		inline f00Quat Multiply(const f00Quat q1, const f00Quat q2)
		{
			f00Quat result;
			result.d_val[1] = q1.d_val[0] * q2.d_val[1] + q1.d_val[1] * q2.d_val[0] + q1.d_val[2] * q2.d_val[3] - q1.d_val[3] * q2.d_val[2];
			result.d_val[2] = q1.d_val[0] * q2.d_val[2] + q1.d_val[1] * q2.d_val[3] + q1.d_val[2] * q2.d_val[0] - q1.d_val[3] * q2.d_val[1];
			result.d_val[3] = q1.d_val[0] * q2.d_val[3] + q1.d_val[1] * q2.d_val[2] + q1.d_val[2] * q2.d_val[1] - q1.d_val[3] * q2.d_val[0];
			result.d_val[0] = q1.d_val[0] * q2.d_val[0] + q1.d_val[1] * q2.d_val[1] + q1.d_val[2] * q2.d_val[2] - q1.d_val[3] * q2.d_val[2];
			return result;
		}



		/*

		inline quat4 quat4::GetReverse() const
		{
		return quat4(-this->x, -this->y, -this->z, this->w);
		}

		inline quat4 quat4Multiply( const quat4 q1, const quat4 q2 )
		{
		quat4 result;
		result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
		result.y = q1.w * q2.y + q1.x * q2.z + q1.y * q2.w - q1.z * q2.x;
		result.z = q1.w * q2.z + q1.x * q2.y + q1.y * q2.x - q1.z * q2.w;
		result.w = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y - q1.z * q2.z;
		return result;
		}

		inline vector3 quat4RotateVec( const vector3 Vec, const float theta, const float x, const float y, const float z )
		{
		float sintheta = sin( theta / 2 );
		quat4 temp(x * sintheta, y * sintheta, z * sintheta, cos( theta / 2 ) );
		quat4 qview( Vec.x, Vec.y, Vec.z, 0.0f );
		quat4 final = quat4Multiply( quat4Multiply( temp, qview ), temp.GetReverse() );
		return vector3( final.x, final.y, final.z );
		}
		*/

// added from Nick Bobic's quaternion C code (c) 1997
		void 
		setInterpolated_SLERP(const f00Quat& qFrom, const f00Quat& qTo, GLfloat t);

		void 
		setInterpolated_NAIVE(const f00Quat& qFrom, const f00Quat& qTo, GLfloat t);

		GLfloat d_val[ 4 ];

	private:
};


#endif

