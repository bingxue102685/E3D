#pragma once
#include "ECommon.h"
#include "EGraphics.h"
#include <map>

namespace E3D
{
	struct EMaterial
	{
		EString name;
		//������
		EColor ambient;
		//������
		EColor diffuse;
		//���淴��
		EColor specular;

		EBitmap *bitmap;

		EMaterial();
		~EMaterial();
		inline bool useTexture() { return bitmap && bitmap->isValid(); }
		EColor getPixel(EFloat u, EFloat v);
	};

	typedef std::map<EString, EMaterial*>::iterator MaterialIter;
	extern std::map<EString, EMaterial*> *GMaterials;

	extern EMaterial* GetMaterial(const EString &name);
	extern bool SetMaterial(const EString &name, EMaterial *material);
	extern bool DestoryMaterial(const EString &name);
	extern void DestoryAllMaterials();

	//��������
	extern void ParseMaterialScript(const EString &scriptName);
}