#include "ECore.h"

namespace E3D
{

	EVertex4D::EVertex4D() : x(0.0f), y(0.0f), z(0.0f), w(1.0f), u(0.0f), v(0.0f){}

	EVertex4D::EVertex4D(const EVertex4D &v):x(v.x), y(v.y), z(v.z), w(v.w), u(v.u), v(v.v) {}

	EVertex4D::EVertex4D(EFloat xf, EFloat yf, EFloat zf): x(xf), y(yf), z(zf), u(0), v(v) {}

	EVertex4D EVertex4D::operator*(const EVertex4D &right)
	{
		EVertex4D temp = *this;
		temp.x *= right.x;
		temp.y *= right.y;
		temp.z *= right.z;
		return temp;
	}

	EVertex4D EVertex4D::operator+(const EVertex4D &right)
	{
		EVertex4D temp = *this;
		temp.x += right.x;
		temp.y += right.y;
		temp.z += right.z;
		return temp;
	}

	EVertex4D EVertex4D::operator-(const EVertex4D &right)
	{
		EVertex4D temp = *this;
		temp.x -= right.x;
		temp.y -= right.y;
		temp.z -= right.z;
		return temp;
	}

	EVertex4D EVertex4D::operator /(EFloat factor)
	{
		EVertex4D temp = *this;
		temp.x /= factor;
		temp.y /= factor;
		temp.z /= factor;
		return temp;
	}

	EVertex4D& EVertex4D::operator =(const EVertex4D &right)
	{
		x = right.x;	y = right.y;	z = right.z;	w = right.w;
		u = right.u;	v = right.v;
		return *this;
	}

	EVector4D EVertex4D::toVector4D() const
	{
		return EVector4D(x, y, z, w);
	}


	EPlane3D::EPlane3D(const EVector4D &p, const EVector4D &nor)
	{
		point = p;
		normal = nor;
		normal.normalize();
	}

	EPlane3D& EPlane3D::operator=(const EPlane3D &right)
	{
		point = right.point;
		normal = right.normal;
		return *this;
	}
}