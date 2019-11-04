#pragma once

#include <vector>
#include "ECommon.h"
#include "EVector.h"

namespace E3D
{
	enum LIGHT_TYPE
	{
		LIGHT_AMBIENT = 0,		//������
		LIGHT_DIRECTION = 1,	//ƽ�й�
		LIGHT_POINT = 2,		//���
	};

	struct ELight
	{
		//�ƹ�ID��ȫ��Ψһ
		EInt id;

		//�ƹ��Ƿ�����
		EBool lightOn;

		//�ƹ�����
		LIGHT_TYPE lightType;

		//�ƹ�ǿ�ȣ�0-1.0f�� Ĭ��1
		EFloat power;

		//�ƹⱳ��ǿ�� Ĭ��0.05
		EFloat shadowPower;

		//�ƹ�Ļ���ɫ��������ʹ�ã�
		EColor ambient;

		//�ƹ���������ɫ
		EColor diffuse;

		//�ƹ�߹ⷴ����ɫ
		EColor specular;

		//�ƹ����������
		EVector4D position;

		//��Դ���򣨵��Դ��Ч��
		EVector4D direction;

		//�ƹ����������ϵλ��
		EVector4D transPostion;

		//���������ϵ��Դ���򣬵����Ч
		EVector4D transDirection;

		//˥������
		EFloat kc, kl, kq;

		//�۹����׶��
		EFloat spot_inner;

		//�۹����׶��
		EFloat spot_outer;

		//�۹��ָ������
		EFloat pf;

		ELight(EInt lightId, LIGHT_TYPE lightType);
	};

#define MAX_LIGHTS 8

	//ȫ�ֵƹ����
	extern std::vector<ELight*> *GLights;
	typedef std::vector<ELight*>::iterator LightIter;

	//�����ƹ� �����ɹ����� �ƹ�ID ��ȫ��Ψһ�� ����ʧ�� �򷵻�-1
	extern EInt CreateLight(LIGHT_TYPE lightType);
	//��ȡ�ƹ⣬�����ڷ���NULL
	extern ELight* GetLight(EInt lightID);
	extern EInt GetLightSize();
	//������й�Դ
	extern void DestroyAllLights();
}
