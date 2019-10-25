#pragma

#include "ECommon.h"
#include "EVector.h"

namespace E3D
{
	struct EVertex4D
	{
		EFloat x, y, z, w;
		EFloat u, v;

		EVertex4D();
		EVertex4D(const EVertex4D &v);
		EVertex4D(EFloat xf, EFloat yf, EFloat zf);
		EVertex4D operator *(const EVertex4D &right);
		EVertex4D operator +(const EVertex4D &right);
		EVertex4D& operator =(const EVertex4D &right);
		EVertex4D operator -(const EVertex4D &right);
		EVertex4D operator /(EFloat factor);

		EVector4D toVector4D() const;
	};

	struct EPlane3D
	{
		//面上一点
		EVector4D point;
		//面的法线
		EVector4D normal;

		EPlane3D() {}
		EPlane3D(const EVector4D &p, const EVector4D &nor);
		EPlane3D& operator = (const EPlane3D &right);
	};
}
