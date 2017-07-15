#include "Vector.h"

using namespace EngineAPI::Math;

Vector3D::Vector3D()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3D::~Vector3D()
{}

Vector3D::Vector3D(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3D::Vector3D(Vector3D& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

Vector3D& Vector3D::operator=(Vector3D& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return (*this);
}

float& Vector3D::operator[](int i)
{
	return ((&x)[i]);
}

const float& Vector3D::operator[](int i) const
{
	return ((&x)[i]);
}

Vector3D& Vector3D::operator*=(Vector3D& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return (*this);
}

Vector3D& Vector3D::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return (*this);
}

Vector3D& Vector3D::operator/=(float s)
{
	s = 1.0f / s;
	x *= s;
	y *= s;
	z *= s;
	return (*this);
}

Vector3D& Vector3D::operator/=(Vector3D& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return (*this);
}

Vector3D& Vector3D::operator+=(Vector3D& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return (*this);
}

Vector3D& Vector3D::operator+=(float s)
{
	x += s;
	y += s;
	z += s;
	return (*this);
}

Vector3D& Vector3D::operator-=(Vector3D& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return (*this);
}

Vector3D& Vector3D::operator-=(float s)
{
	x -= s;
	y -= s;
	z -= s;
	return (*this);
}