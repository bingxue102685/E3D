#pragma once

#include <vector>
#include "ECommon.h"
#include "EVector.h"

namespace E3D
{
	enum LIGHT_TYPE
	{
		LIGHT_AMBIENT = 0,		//环境光
		LIGHT_DIRECTION = 1,	//平行光
		LIGHT_POINT = 2,		//点光
	};

	struct ELight
	{
		//灯光ID，全局唯一
		EInt id;

		//灯光是否启用
		EBool lightOn;

		//灯光类型
		LIGHT_TYPE lightType;

		//灯光强度（0-1.0f） 默认1
		EFloat power;

		//灯光背面强度 默认0.05
		EFloat shadowPower;

		//灯光的环境色（环境光使用）
		EColor ambient;

		//灯光漫反射颜色
		EColor diffuse;

		//灯光高光反射颜色
		EColor specular;

		//灯光的世界坐标
		EVector4D position;

		//光源朝向（点光源无效）
		EVector4D direction;

		//灯光摄像机坐标系位置
		EVector4D transPostion;

		//摄像机坐标系光源朝向，点光无效
		EVector4D transDirection;

		//衰减因子
		EFloat kc, kl, kq;

		//聚光灯内锥角
		EFloat spot_inner;

		//聚光灯外锥角
		EFloat spot_outer;

		//聚光灯指数因子
		EFloat pf;

		ELight(EInt lightId, LIGHT_TYPE lightType);
	};

#define MAX_LIGHTS 8

	//全局灯光管理
	extern std::vector<ELight*> *GLights;
	typedef std::vector<ELight*>::iterator LightIter;

	//创建灯光 创建成功返回 灯光ID （全局唯一） 创建失败 则返回-1
	extern EInt CreateLight(LIGHT_TYPE lightType);
	//获取灯光，不存在返回NULL
	extern ELight* GetLight(EInt lightID);
	extern EInt GetLightSize();
	//清除所有光源
	extern void DestroyAllLights();
}
