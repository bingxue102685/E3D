#pragma once

#include "ECommon.h"

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
		//设置远裁面距离
		void setZFar(EFloat zfar);

		//设置相机的世界坐标
		void setPosition(const EVector3D &pos);
		//设置相机观察点 looktartget 为true 锁定观察点
		void setTarget(const EVector3D &target, EBool lockTarget = false);
		void releaseTarget() { mLockTarget = false; }

		//基于世界坐标系移动
		void move(const EVector3D &move);
		//基于摄像机自身坐标系移动
		void moveRelative(const EVector3D &move);

		//绕y轴旋转
		void yaw(EFloat yDegree);
		//绕x轴旋转
		void pitch(EFloat pDegree);
		//设置渲染模式 线框或实体
		void setRenderMode(RenderMode mode) { mRenderMode = mode; }
		RenderMode getRenderMode() const { return mRenderMode; };
		EFrustum *getFrustum() const { return mFrustum; }
	protected:
		friend class ESceneMananger;
		EFrustum *mFrustum;
		EBool mLockTarget;
		EBool mNeedUpdate;
		RenderMode mRenderMode;
	};
}