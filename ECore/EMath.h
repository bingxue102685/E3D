#pragma once

#include "ECommon.h"

namespace E3D
{
	template<class T>
	const T& Min(const T &a, const T &b)
	{
		return (a < b) ? a : b;
	}

	template<class T>
	const T& Max(const T &a, const T &b)
	{
		return (b < a) ? a : b;
	}

	template<typename T>
	const T& Clamp(const T &val, const T &min, const T &max)
	{
		return Min(Max(val, min), max);
	}

	//获取平移变换矩阵
	void GetTranslateMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z);
	//获取缩放变换矩阵
	void GetScaleMatrix44(EMatrix44 &mat, EFloat x, EFloat y, EFloat z);
	
	//获取沿X轴旋转变换矩阵
	void GetRotateMatrix44X(EMatrix44 &mat, EFloat degree);
	//获取沿Y轴旋转变换矩阵
	void GetRotateMatrix44Y(EMatrix44 &mat, EFloat degree);
	//获取沿Z轴旋转变换矩阵
	void GetRotateMatrix44Z(EMatrix44 &mat, EFloat degree);

	//获取沿任意轴旋转变换矩阵
	void GetRotateMatrix44(EMatrix44 &mat, const EVector4D &normal, EFloat degree);
	
	void GetVertex4DAddVector4D(const EVertex4D &v, const EVector4D &p, EVertex4D &result);

	//3D向量 X 44矩阵
	void GetVector3DMulMatrix44(const EVector3D &vect, const EMatrix44 &mat, EVector3D &result);
	//4D向量 X 44矩阵
	void GetVector4DMulMatrix44(const EVector4D &vect, const EMatrix44 &mat, EVector4D &result);

	void GetVertex4DMulMatrix44(const EVertex4D &ver, const EMatrix44 &mat, EVertex4D &result);
}