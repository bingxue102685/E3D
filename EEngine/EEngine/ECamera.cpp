#include "ECamera.h"
#include "EFrustum.h"
#include "EMath.h"

namespace E3D
{
	ECamera::ECamera() :mLockTarget(false), mNeedUpdate(true), mRenderMode(RENDER_SOILD)
	{
		mFrustum = new EFrustum(CAMERA_MODEL_UVN, EVector4D::ZERO, EVector4D::ZERO, EVector4D::ZERO, 10, 1000, 75, SCREEN_WIDTH, SCREEN_HEIGHT);
		update();
	}

	ECamera::~ECamera() 
	{
		SafeDelete(mFrustum);
	}

	void ECamera::setZFar(EFloat zfar)
	{
		mFrustum->clip_z_far = zfar;
		mNeedUpdate = true;
	}

	void ECamera::setZNear(EFloat znear)
	{
		mFrustum->clip_z_near = znear;
		mNeedUpdate = true;
	}

	void ECamera::setPosition(const EVector3D &pos)
	{
		mFrustum->position.x = pos.x;
		mFrustum->position.y = pos.y;
		mFrustum->position.z = pos.z;
		mNeedUpdate = true;
	}

	void ECamera::setTarget(const EVector3D &target, EBool lockTarget)
	{
		mFrustum->camTarget.x = target.x;
		mFrustum->camTarget.y = target.y;
		mFrustum->camTarget.z = target.z;

		mLockTarget = lockTarget;
		mNeedUpdate = true;
	}

	//基于世界坐标系移动
	void ECamera::move(const EVector3D &mov)
	{
		mFrustum->position.x += mov.x;
		mFrustum->position.y += mov.y;
		mFrustum->position.z += mov.z;
		if (!mLockTarget)
		{
			mFrustum->camTarget.x += mov.x;
			mFrustum->camTarget.y += mov.y;
			mFrustum->camTarget.z += mov.z;
		}
		mNeedUpdate = true;
	}
	//基于相机自身坐标系移动
	void ECamera::moveRelative(const EVector3D &mov)
	{
		EVector4D offset = mFrustum->camRight * mov.x + mFrustum->camUp * mov.y + mFrustum->camLook * mov.z;
		mFrustum->position = mFrustum->position + offset;
		if (!mLockTarget)
		{
			mFrustum->camTarget = mFrustum->camTarget + offset;
		}
		mNeedUpdate = true;
	}
	//绕y轴
	void ECamera::yaw(EFloat yDegree)
	{
		EMatrix44 roMat;
		EVector4D distance = mFrustum->camTarget - mFrustum->position;
		GetRotateMatrix44(roMat, mFrustum->camUp, yDegree);
		GetVector4DMulMatrix44(distance, roMat, distance);
		//旋转后的目标点
		mFrustum->camTarget = distance + mFrustum->position;
		mNeedUpdate = true;
	}

	//绕x轴旋转
	void ECamera::pitch(EFloat pDegree)
	{
		EMatrix44 roMat;
		EVector4D distance = mFrustum->camTarget - mFrustum->position;
		GetRotateMatrix44(roMat, mFrustum->camRight, pDegree);
		GetVector4DMulMatrix44(distance, roMat, distance);
		//旋转后的目标点
		mFrustum->camTarget = distance + mFrustum->position;
		mNeedUpdate = true;
	}

	void ECamera::update()
	{
		if (mNeedUpdate)
		{
			EMatrix44 mt_inv;
			GetTranslateMatrix44(mt_inv, -mFrustum->position.x, -mFrustum->position.y, -mFrustum->position.z);
			mFrustum->camLook = mFrustum->camTarget - mFrustum->position;
			mFrustum->camUp = EVector4D::UNIT_Y;
			mFrustum->camRight = mFrustum->camUp.cross(mFrustum->camLook);
			mFrustum->camUp = mFrustum->camLook.cross(mFrustum->camRight);

			mFrustum->camLook.normalize();
			mFrustum->camUp.normalize();
			mFrustum->camRight.normalize();

			EMatrix44 mt_uvn(
				mFrustum->camRight.x, mFrustum->camUp.x, mFrustum->camRight.x, 0,
				mFrustum->camRight.y, mFrustum->camUp.y, mFrustum->camRight.y, 0, 
				mFrustum->camRight.z, mFrustum->camUp.z, mFrustum->camRight.z, 0,
				0, 0, 0, 1);

			mFrustum->mWorldToCamera = mt_inv * mt_uvn;
			mNeedUpdate = false;
		}
	}
}