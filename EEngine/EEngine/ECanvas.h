#pragma once
#include "EGraphics.h"

namespace E3D
{
	//绘图接口
	class ECanvas
	{
	public:
		virtual ~ECanvas(){}

		//只负责绘制
		virtual void onPaint() = 0;

		//更新场景
		virtual void update() = 0;
	};
}