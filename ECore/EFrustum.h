#pragma once

#include "ECommon.h"
#include "EVector.h"
#include "EMatrix.h"

namespace E3D
{
	enum CAMERA_TYPE
	{
		CAMERA_MODEL_ELUA = 0x0001,		//欧拉相机模型
		CAMERA_MODEL_UVN = 0x0002,		//UVN相机模型
	};

	struct EFrustum
	{
		//相机状态
		EInt state;
		//相机属性
		EInt attribute;
		//相机类型
		CAMERA_TYPE camMode;
		//位置
		EVector4D position;
		//注视方向
		EVector4D dirction;
		//UVN相机
		EVector4D camUp;
		EVector4D camRight;
		EVector4D camLook;
		EVector4D camTarget;

		//视点到投影面的距离
		EFloat view_dist;
		//水平方向和垂直方向的视野
		EFloat fov;
		//近平面
		EFloat clip_z_near;
		//远平面
		EFloat clip_z_far;
		//右裁面
		EPlane3D clip_plane_R;
		//左裁面
		EPlane3D clip_plane_L;
		//上裁面
		EPlane3D clip_plane_T;
		//下裁面
		EPlane3D clip_plane_B;

		//投影面宽
		EFloat viewplane_width;
		//投影面高
		EFloat viewplane_height;

		//视口宽
		EFloat viewport_width;
		//视口高
		EFloat viewport_height;
		//屏幕中心坐标
		EFloat viewport_center_X;
		//屏幕中心坐标
		EFloat viewport_center_Y;

		//屏幕的宽高比
		EFloat aspect_ratio;

		//世界坐标到相机坐标变换矩阵
		EMatrix44 mWorldToCamera;
		//相机坐标到透视坐标变换矩阵
		EMatrix44 mCameraToPerspective;
		//透视坐标到屏幕坐标变换矩阵
		EMatrix44 mPerspectiveToScreen;

		EFrustum(CAMERA_TYPE mode, const EVector4D &pos, const EVector4D &dir, const EVector4D &target, EFloat nearZ, EFloat farZ,
			EFloat fov, EFloat viewportWidth, EFloat viewportHeight);
	};
}