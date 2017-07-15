//Vector.h
//Created 15/07/17
//Created By Daniel Bowler
//
//My simple vector math library

#pragma once

#include <math.h> //Sqrt()

namespace EngineAPI
{
	namespace Math
	{
		class Vector3D
		{
		public:
			//Public components
			float x, y, z;

		public:
			//Default constructor (returns zero vector) & destructor
			Vector3D();
			~Vector3D();

			//Other constructors
			Vector3D(float x, float y, float z);
			Vector3D(Vector3D& other);

			//Assignment operator
			Vector3D& operator=(Vector3D& other);

			//Overload the [] operator. Returns the ith component
			float& operator[](int i);
			const float& operator[](int i) const;

			//Overloaded operators:
			Vector3D& operator*=(Vector3D& v);
			Vector3D& operator*=(float s);

			Vector3D& operator/=(Vector3D& v);
			Vector3D& operator/=(float s);

			Vector3D& operator+=(Vector3D& v);
			Vector3D& operator+=(float s);

			Vector3D& operator-=(Vector3D& v);
			Vector3D& operator-=(float s);
		};

		//Global operators:
		inline Vector3D operator *(const Vector3D& v, float s)
		{
			return (Vector3D(v.x * s, v.y * s, v.z * s));
		}

		inline Vector3D operator *(const Vector3D& v, const Vector3D& v2)
		{
			return (Vector3D(v.x * v2.x, v.y * v2.y, v.z * v2.z));
		}

		inline Vector3D operator /(const Vector3D& v, float s)
		{
			s = 1.0f / s;
			return (Vector3D(v.x * s, v.y * s, v.z * s));
		}

		inline Vector3D operator /(const Vector3D& v, const Vector3D& v2)
		{
			return (Vector3D(v.x / v2.x, v.y / v2.y, v.z / v2.z));
		}

		inline Vector3D operator +(const Vector3D& a, const Vector3D& b)
		{
			return (Vector3D(a.x + b.x, a.y + b.y, a.z + a.z));
		}

		inline Vector3D operator -(const Vector3D& a, const Vector3D& b)
		{
			return (Vector3D(a.x - b.x, a.y - b.y, a.z - a.z));
		}

		inline Vector3D operator -(const Vector3D& v)
		{
			return (Vector3D(-v.x, -v.y, -v.z)); //Negates the vector
		}

		//Global vector functions
		inline float Length(const Vector3D& v)
		{
			return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
		}

		inline float Magnitude(const Vector3D& v)
		{
			return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
		}

		inline Vector3D Normalize(const Vector3D& v)
		{
			return (v / Magnitude(v));
		}
	};
};