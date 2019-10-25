#pragma once

#include "ECommon.h"
#include "EVector.h"
#include "EMatrix.h"

namespace E3D
{
	enum CAMERA_TYPE
	{
		CAMERA_MODEL_ELUA = 0x0001,		//ŷ�����ģ��
		CAMERA_MODEL_UVN = 0x0002,		//UVN���ģ��
	};

	struct EFrustum
	{
		//���״̬
		EInt state;
		//�������
		EInt attribute;
		//�������
		CAMERA_TYPE camMode;
		//λ��
		EVector4D position;
		//ע�ӷ���
		EVector4D dirction;
		//UVN���
		EVector4D camUp;
		EVector4D camRight;
		EVector4D camLook;
		EVector4D camTarget;

		//�ӵ㵽ͶӰ��ľ���
		EFloat view_dist;
		//ˮƽ����ʹ�ֱ�������Ұ
		EFloat fov;
		//��ƽ��
		EFloat clip_z_near;
		//Զƽ��
		EFloat clip_z_far;
		//�Ҳ���
		EPlane3D clip_plane_R;
		//�����
		EPlane3D clip_plane_L;
		//�ϲ���
		EPlane3D clip_plane_T;
		//�²���
		EPlane3D clip_plane_B;

		//ͶӰ���
		EFloat viewplane_width;
		//ͶӰ���
		EFloat viewplane_height;

		//�ӿڿ�
		EFloat viewport_width;
		//�ӿڸ�
		EFloat viewport_height;
		//��Ļ��������
		EFloat viewport_center_X;
		//��Ļ��������
		EFloat viewport_center_Y;

		//��Ļ�Ŀ�߱�
		EFloat aspect_ratio;

		//�������굽�������任����
		EMatrix44 mWorldToCamera;
		//������굽͸������任����
		EMatrix44 mCameraToPerspective;
		//͸�����굽��Ļ����任����
		EMatrix44 mPerspectiveToScreen;

		EFrustum(CAMERA_TYPE mode, const EVector4D &pos, const EVector4D &dir, const EVector4D &target, EFloat nearZ, EFloat farZ,
			EFloat fov, EFloat viewportWidth, EFloat viewportHeight);
	};
}