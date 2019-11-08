#pragma once

#include "ECommon.h"

namespace E3D
{
	enum RenderMode
	{
		RENDER_WIRE,	//�߿���Ⱦ
		RENDER_SOILD,	//ʵ����Ⱦ
	};

	//������ܣ��߿���Ⱦ��ʵ����Ⱦ�����Զ���ü������ã��������������ϵ�е�λ�ã��ƶ�����ת�ȡ�
	class ECamera
	{
	public:
		ECamera();
		~ECamera();

		void update();
		//���ý��������
		void setZNear(EFloat znear);
		//����Զ�������
		void setZFar(EFloat zfar);

		//�����������������
		void setPosition(const EVector3D &pos);
		//��������۲�� looktartget Ϊtrue �����۲��
		void setTarget(const EVector3D &target, EBool lockTarget = false);
		void releaseTarget() { mLockTarget = false; }

		//������������ϵ�ƶ�
		void move(const EVector3D &move);
		//�����������������ϵ�ƶ�
		void moveRelative(const EVector3D &move);

		//��y����ת
		void yaw(EFloat yDegree);
		//��x����ת
		void pitch(EFloat pDegree);
		//������Ⱦģʽ �߿��ʵ��
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