#include "EMath.h"
#include "EVector.h"
#include "EMatrix.h"
#include "ECore.h"

namespace E3D
{
	void GetTranslateMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z)
	{
		mat.m[0][0] = 1, mat.m[0][1] = 0, mat.m[0][2] = 0, mat.m[0][3] = 0;
		mat.m[1][0] = 0, mat.m[1][1] = 1, mat.m[1][2] = 0, mat.m[1][3] = 0;
		mat.m[2][0] = 0, mat.m[2][1] = 0, mat.m[2][2] = 1, mat.m[2][3] = 0;
		mat.m[3][0] = x, mat.m[3][1] = y, mat.m[3][2] = z, mat.m[3][3] = 1;
	}

	void GetScaleMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z)
	{
		mat.m[0][0] = x, mat.m[0][1] = 0, mat.m[0][2] = 0, mat.m[0][3] = 0;
		mat.m[1][0] = 0, mat.m[1][1] = y, mat.m[1][2] = 0, mat.m[1][3] = 0;
		mat.m[2][0] = 0, mat.m[2][1] = 0, mat.m[2][2] = z, mat.m[2][3] = 0;
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}
	//沿X轴旋转
	void GetRotateMatrix44X(EMatrix44 &mat, EFloat degree)
	{
		EFloat sinA = sin(Degree2Radian(degree));
		EFloat cosA = cos(Degree2Radian(degree));

		mat.m[0][0] = 1, mat.m[0][1] = 0,		mat.m[0][2] = 0,	mat.m[0][3] = 0;
		mat.m[1][0] = 0, mat.m[1][1] = cosA,	mat.m[1][2] = sinA,	mat.m[1][3] = 0;
		mat.m[2][0] = 0, mat.m[2][1] = -sinA,	mat.m[2][2] = cosA,mat.m[2][3] = 0;
		mat.m[3][0] = 0, mat.m[3][1] = 0,		mat.m[3][2] = 0,	mat.m[3][3] = 1;
	}
	//沿Y轴旋转
	void GetRotateMatrix44Y(EMatrix44 &mat, EFloat degree)
	{
		EFloat sinA = sin(Degree2Radian(degree));
		EFloat cosA = cos(Degree2Radian(degree));

		mat.m[0][0] = cosA, mat.m[0][1] = 0, mat.m[0][2] = sinA, mat.m[0][3] = 0;
		mat.m[1][0] = 0, mat.m[1][1] = 1, mat.m[1][2] = 0, mat.m[1][3] = 0;
		mat.m[2][0] = sinA, mat.m[2][1] = 0, mat.m[2][2] = cosA, mat.m[2][3] = 0;
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}
	//沿Z轴旋转
	void GetRotateMatrix44Z(EMatrix44 &mat, EFloat degree)
	{
		EFloat sinA = sin(Degree2Radian(degree));
		EFloat cosA = cos(Degree2Radian(degree));

		mat.m[0][0] = cosA, mat.m[0][1] = sinA, mat.m[0][2] = 0, mat.m[0][3] = 0;
		mat.m[1][0] = -sinA, mat.m[1][1] = cosA, mat.m[1][2] = 0, mat.m[1][3] = 0;
		mat.m[2][0] = 0, mat.m[2][1] = 0, mat.m[2][2] = 1, mat.m[2][3] = 0;
		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}
	//沿任意周旋转
	void GetRotateMatrix44(EMatrix44 &mat, const EVector4D &normal, EFloat degree)
	{
		EFloat sinA = sin(Degree2Radian(degree));
		EFloat cosA = cos(Degree2Radian(degree));
		EFloat oneCosA = 1 - cosA;


		mat.m[0][0] = normal.x * normal.x * oneCosA + cosA;
		mat.m[0][1] = normal.x * normal.y * oneCosA + normal.x * sinA;
		mat.m[0][2] = normal.x * normal.z * oneCosA - normal.y * sinA;
		mat.m[0][3] = 0;

		mat.m[1][0] = normal.x * normal.y * oneCosA - normal.z * sinA;
		mat.m[1][1] = normal.y * normal.y * oneCosA + cosA;
		mat.m[1][2] = normal.y * normal.z * oneCosA + normal.x * sinA;
		mat.m[1][3] = 0;

		mat.m[2][0] = normal.x * normal.z * oneCosA + normal.y * sinA;
		mat.m[2][1] = normal.y * normal.z * oneCosA - normal.x * sinA;
		mat.m[2][2] = normal.z * normal.z * oneCosA + cosA;
		mat.m[2][3] = 0;

		mat.m[3][0] = 0, mat.m[3][1] = 0, mat.m[3][2] = 0, mat.m[3][3] = 1;
	}

	void GetVertex4DAddVector4D(const EVertex4D &v, const EVector4D &p, EVertex4D &result)
	{
		result.x = v.x + p.x;
		result.y = v.y + p.y;
		result.z = v.z + p.z;
		result.w = 1;
	}

	//3D向量 X 44矩阵
	void GetVector3DMulMatrix44(const EVector3D &vect, const EMatrix44 &mat, EVector3D &result)
	{
		result.x = vect.x * mat.m[0][0] + vect.y * mat.m[1][0] + vect.z * mat.m[2][0];
		result.y = vect.x * mat.m[0][1] + vect.y * mat.m[1][1] + vect.z * mat.m[2][1];
		result.z = vect.x * mat.m[0][2] + vect.y * mat.m[1][2] + vect.z * mat.m[2][2];
	}

	//4D向量 X 44矩阵
	void GetVector4DMulMatrix44(const EVector4D &vect, const EMatrix44 &mat, EVector4D &result)
	{
		result.x = vect.x * mat.m[0][0] + vect.y * mat.m[1][0] + vect.z * mat.m[2][0];
		result.y = vect.x * mat.m[0][1] + vect.y * mat.m[1][1] + vect.z * mat.m[2][1];
		result.z = vect.x * mat.m[0][2] + vect.y * mat.m[1][2] + vect.z * mat.m[2][2];
		result.w = 1;
	}

	void GetVertex4DMulMatrix44(const EVertex4D &ver, const EMatrix44 &mat, EVertex4D &result)
	{
		result.x = ver.x * mat.m[0][0] + ver.y * mat.m[1][0] + ver.z * mat.m[2][0];
		result.y = ver.x * mat.m[0][1] + ver.y * mat.m[1][1] + ver.z * mat.m[2][1];
		result.z = ver.x * mat.m[0][2] + ver.y * mat.m[1][2] + ver.z * mat.m[2][2];
		result.w = 1;
	}
}