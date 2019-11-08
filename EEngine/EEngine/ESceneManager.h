#pragma once
#include "ECommon.h"
#include "EMesh.h"
#include "ELight.h"
#include "ECore.h"
#include "ECamera.h"
#include <map>

namespace E3D
{
	class ESceneManager
	{
		typedef std::map<EString, EMesh*>::iterator MeshIter;
	public:
		ESceneManager();
		~ESceneManager();

		//��������
		void drawString(const EString &str, EInt x, EInt y, const EColor &c = EColor(255, 255, 255));
		//����һ��ģ��ʵ��
		EMesh* createMesh(const EString &name, const EString &meshName);
		//��ȡָ��ģ��
		EMesh* getMesh(const EString &name);
		//����ָ����ģ��ʵ��
		void destroyMesh(const EString &name);

		void clearMesh();

		//�����رյƹ�
		void enableLights(EBool enable);
		EBool isLightEnable() const { return mLightEnable; }

		//����һյ��
		ELight* createLight(LIGHT_TYPE lightType);
		//���һյ�� ����ƹⲻ���� �򷵻�NULL
		ELight* getLight(EInt id);

		//��ȡ���ָ��
		ECamera* getCamera() { return mCamera; }

		//��ȡ�����������
		EInt getTotalObjectNumber() const { return mMeshs.size(); }
		//��ȡ�ɼ��������
		EInt getVisibleObjectNumber() const { return mVisibleObjectNumber; }
		//��ȡ�ɼ�����εĸ���
		EInt getVisiblePolyonNumber() const { return mVisiblePolyonNumber; }
		//��ȡ���ж���θ���
		EInt getTotalPolyonNumber() const { return mTotalPolyonNumber; }

		//��������
		EMesh* createTerrain(const EString &rawFileName, const EString &heightMapFileName, EFloat uTitle = 1, EFloat vTitle = 1, EFloat blockSize = 5.0f);
		EMesh* getTerrain() { return mTerrainMesh; }

		//����
		void update();

	protected:
		std::map<EString, EMesh*> mMeshs;
		std::map<EString, EMesh*>mUsedMesh;
		ERenderList4D *mRenderList;
		EMesh *mTerrainMesh;
		ECamera *mCamera;
		EBool mLightEnable;
		EInt mVisibleObjectNumber;
		EInt mVisiblePolyonNumber;
		EInt mTotalPolyonNumber;
	};
}