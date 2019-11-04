#include "ELight.h"

namespace E3D
{
	ELight::ELight(EInt lightID, LIGHT_TYPE type)
	{
		id = lightID;
		lightType = type;
		lightOn = true;
		ambient = EColor(255, 255, 255);
		diffuse = EColor(255, 255, 255);
		specular = EColor(255, 255, 255);
		position = EVector4D(0, 0, 0);
		direction = EVector4D(0, 0, 1);
		kc = 1.0f;
		kl = 1.0f;
		kq = 0.0f;
		spot_inner = 45.0f;
		spot_outer = 45.0f;
		power = 1.0f;
		shadowPower = 0.5f;
	}

	std::vector<ELight*> *GLights = new std::vector<ELight*>();

	EInt CreateLight(LIGHT_TYPE lightType)
	{
		if ((EInt)GLights->size() > MAX_LIGHTS)
		{
			return -1;
		}

		EInt id = (EInt)GLights->size();
		GLights->push_back(new ELight(id, lightType));

		return id;
	}

	//��ȡһ���ƹ⣬����ƹⲻ���ڣ��򴴽�һ���ƹⷵ��
	ELight* GetLight(EInt lightID)
	{
		if (GLights == NULL || (EInt)GLights->size() > MAX_LIGHTS || lightID < 0 || lightID > (EInt)GLights->size() - 1) 
		{
			return NULL;
		}

		return GLights->at(lightID);
	}
	//��ȡ�ƹ�����
	EInt GetLightSize()
	{
		if (GLights == NULL)
		{
			return 0;
		}
		return GLights->size();
	}

	//�������еĵƹ�
	void DestroyAllLights()
	{
		for (LightIter iter = GLights->begin(); iter != GLights->end(); iter++)
		{
			SafeDelete(*iter);
		}

		GLights->clear();
		SafeDelete(GLights);
	}
}