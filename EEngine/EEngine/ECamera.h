#pragma once

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

	private:

	};
}