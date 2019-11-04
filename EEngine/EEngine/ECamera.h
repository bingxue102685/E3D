#pragma once

namespace E3D
{
	enum RenderMode
	{
		RENDER_WIRE,	//线框渲染
		RENDER_SOILD,	//实体渲染
	};

	//相机功能：线框渲染，实体渲染，相机远近裁剪面设置，相机在世界坐标系中的位置，移动，旋转等。
	class ECamera
	{
	public:
		ECamera();
		~ECamera();

		void update();
		//设置近裁面距离
		void setZNear(EFloat znear);

	private:

	};
}