//Matrix3x3
//Created 17/07/17
//Created By Daniel Bowler
//
//3x3 matrix

#pragma once

namespace EngineAPI
{
	namespace Math
	{
		struct Matrix3x3
		{
		private:
			//Private entries - Use the () operator
			//to access components in [row][col]
			float m[3][3];

		public:
			//Inits everything to zero
			Matrix3x3()
			{
				ZeroMemory(this, sizeof(Matrix3x3));
			};

			//Inits everything along the diagonal to diagonal
			Matrix3x3(float diagonal)
			{
				ZeroMemory(this, sizeof(Matrix3x3));
				m[0][0] = diagonal;
				m[1][1] = diagonal;
				m[2][2] = diagonal;
			};

			//Init each component of the matrix
			Matrix3x3(float m00, float m01, float m02,
				float m10, float m11, float m12,
				float m20, float m21, float m22)
			{
				m[0][0] = m00; m[0][1] = m10; m[0][2] = m20;
				m[1][0] = m01; m[1][1] = m11; m[1][2] = m21;
				m[2][0] = m02; m[2][1] = m12; m[2][2] = m22;
			};

			//Copies the vector3D data in to each matrix column
			Matrix3x3(Vector3D col0, Vector3D col1, Vector3D col2)
			{
				m[0][0] = col0.x; m[0][1] = col0.y; m[0][2] = col0.z;
				m[1][0] = col1.x; m[1][1] = col1.y; m[1][2] = col1.z;
				m[2][0] = col2.x; m[2][1] = col2.y; m[2][2] = col2.z;
			};

			Matrix3x3(Matrix3x3&  other)
			{
				memcpy(this, &other, sizeof(Matrix3x3));
			}

			Matrix3x3& operator=(Matrix3x3& other)
			{
				memcpy(this, &other, sizeof(Matrix3x3));
				return (*this);
			};

			//Accessors for the entires data
			float& operator() (int row_i, int col_j)
			{
				return (m[col_j][row_i]);
			};

			const float& operator() (int row_i, int col_j) const
			{
				return (m[col_j][row_i]);
			};

			Vector3D& operator[] (int col_j)
			{
				return (*reinterpret_cast<Vector3D*>(&m[col_j]));
			};

			const Vector3D& operator[] (int col_j) const
			{
				return (*reinterpret_cast<const Vector3D*>(&m[col_j]));
			};
		};
	};
};