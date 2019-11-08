#include "ESceneManager.h"

namespace E3D
{
	ESceneManager::ESceneManager() :mLightEnable(true), mVisibleObjectNumber(0), mVisiblePolyonNumber(0),
		mTotalPolyonNumber(0), mTerrainMesh(NULL)
	{
		mCamera = new ECamera();
		mRenderList = new ERenderList4D();
	}

	ESceneManager::~ESceneManager()
	{
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); iter++)
		{
			SafeDelete(iter->second);
		}

		for (MeshIter iter = mUsedMesh.begin(); iter != mUsedMesh.end(); iter++)
		{
			SafeDelete(iter->second);
		}

		mMeshs.clear();
		DestroyAllLights();
		SafeDelete(mCamera);
		SafeDelete(mRenderList);
	}

	void ESceneManager::drawString(const EString &str, EInt x, EInt y, const EColor &c = EColor(255, 255, 255))
	{
		EGraphics::drawString(str, x, y, c);
	}

	//����һ��ʵ��ģ��
	EMesh* ESceneManager::createMesh(const EString &name, const EString &meshName)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter == mMeshs.end())
		{
			EMesh * mesh = NULL;
			MeshIter meshIter = mUsedMesh.find(meshName);
			if (meshIter != mUsedMesh.end())
			{
				mesh = meshIter->second->clone();
				mesh->mName = name;
			}
			else
			{
				mesh = new EMesh(name, meshName);
				mUsedMesh.insert(std::make_pair(meshName, mesh->clone()));
			}
			mMeshs.insert(std::make_pair(name, mesh));
			return mesh;
		}
		return iter->second;
	}

	EMesh* ESceneManager::getMesh(const EString &name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter != mMeshs.end())
		{
			return iter->second;
		}
		return NULL;
	}

	//����ָ����ģ��ʵ��
	void ESceneManager::destroyMesh(const EString &name)
	{
		MeshIter iter = mMeshs.find(name);
		if (iter != mMeshs.end())
		{
			SafeDelete(iter->second);
			mMeshs.erase(name);
		}	
	}

	void ESceneManager::clearMesh()
	{
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); iter++)
		{
			SafeDelete(iter->second);
		}
		mMeshs.clear();
	}
	//���ݸ߶�ͼ���ɵ���
	EMesh* ESceneManager::createTerrain(const EString &rawFileName, const EString &heightMapFileName, EFloat uTitle = 1, EFloat vTitle = 1, EFloat blockSize = 5.0f)
	{
		if (mTerrainMesh)
			SafeDelete(mTerrainMesh);
		EBitmap *heightMap = new EBitmap(rawFileName);

		if (!heightMap->isValid() || heightMap->getWidth() % 2 == 0 || heightMap->getHeight() % 2 == 0)
		{
			SafeDelete(heightMap);
			return NULL;
		}

		EMaterial *mat = new EMaterial();
		mat->bitmap = new EBitmap(heightMapFileName);
		SetMaterial("Terrain", mat);

		EInt row = heightMap->getHeight();
		EInt col = heightMap->getWidth();

		mTerrainMesh = new EMesh();
		EObject4D *obj = new EObject4D();
		mTerrainMesh->mObject = obj;

		obj->name = "Terrain";
		obj->materialName = "Terrain";
		obj->state = EOBJECT_STATE_ACTIVE | EOBJECT_STATE_VISIBLE;
		obj->scale = EVector4D(1, 1, 1);
		obj->worldPostion = EVector4D::ZERO;
		obj->vertexNumber = row * col;
		obj->polyonNumber = (row - 1) * (col - 1) * 2;

		mTerrainMesh->mVertexNumber = obj->vertexNumber;
		mTerrainMesh->mPolyonNumber = obj->polyonNumber;

		//�����б�
		obj->localList.reserve(obj->vertexNumber);
		obj->transformList.reserve(obj->vertexNumber);
		//������б�
		obj->polyonList.reserve(obj->polyonNumber);

		EFloat offsetX = (col - 1) * blockSize / 2;
		EFloat offsetZ = (row - 1) * blockSize / 2;

		EFloat minY = 0, maxY = 0;
		EFloat u = uTitle / (col - 1);
		EFloat v = vTitle / (row - 1);

		for (EInt r = 0; r < row; r++)
		{
			for (EInt c = 0; c < col; c++)
			{
				EVertex4D vex;
				EColor color = heightMap->getPixel(c, r);
				vex.x = c * blockSize - offsetX;
				vex.y = 0.1f * (0.299f * color.r + 0.587f * color.g + 0.144f * color.b);
				vex.z = r * blockSize - offsetZ;

				if (vex.y < minY)
				{
					minY = vex.y;
				}

				if (vex.y > maxY)
				{
					maxY = vex.y;
				}
				//����Uv
				vex.u = c * u;
				vex.v = r * v;
				obj->localList.push_back(vex);
				obj->transformList.push_back(vex);

				if (r < row - 1 && c < col - 1)
				{
					EPolyon4D poly;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.attribute = EPOLY_ATTR_VERTEX_POSITION | EPOLY_ATTR_VERTEX_UV;
					poly.verList = &obj->transformList;
					poly.verIndex[0] = r * col + c;
					poly.verIndex[1] = (r + 1) * col + c;
					poly.verIndex[2] = r * col + c + 1;
					obj->polyonList.push_back(poly);

					poly.verIndex[0] = r * col + c + 1;
					poly.verIndex[1] = (r + 1) * col + c;
					poly.verIndex[2] = (r + 1) * col + c + 1;
					obj->polyonList.push_back(poly);
				}
			}
		}
		obj->maxBoundingBox = EVector4D(offsetX, maxY, offsetZ);
		obj->minBoundingBox = EVector4D(-offsetX, minY, -offsetZ);
		obj->maxRadius = std::sqrt(offsetX * offsetX + offsetZ * offsetZ);

		SafeDelete(heightMap);
		mMeshs.insert(std::make_pair("Terrain", mTerrainMesh));

		//���޳�
		mTerrainMesh->setCullFlag(false);

		return mTerrainMesh;
	}
	//����ѭ���߼�
	void ESceneManager::update()
	{
		mVisibleObjectNumber = 0;
		mTotalPolyonNumber = 0;

		//�������
		mCamera->update();

		//������ģ����ӵ���Ⱦ�б�
		for (MeshIter iter = mMeshs.begin(); iter != mMeshs.end(); iter++)
		{
			mTotalPolyonNumber += iter->second->getPolyonNumber();

			//ֻ����Ұ�ڵ�������뵽��Ⱦ�б���
			if (iter->second->isVisible() && !Cull_Object4D(iter->second->getObject(), mCamera->getFrustum(), CULL_BOX))
			{
				mVisibleObjectNumber++;
				Insert_Object4D_To_RenderList4D(mRenderList, iter->second->getObject(), TRANSFORM_TRANS, true);
			}
		}

		//��Ⱦ�б����3d��ˮ�ߴ���
		//�ֲ�����ϵ����������ϵ
		Local_To_World_RenderList4D(mRenderList, EVector4D::ZERO, TRANSFORM_TRANS);
		//��������ϵת��Ļ����ϵ
		//��������ϵ->�����޳�->�������ϵװ��->�ü�->���ռ���->͸�ӱ任->��Ļ�任
		World_To_Screen_RenderList4D(mRenderList, mCamera->getFrustum());

		//��ǰ�ɼ�����θ���
		mVisiblePolyonNumber = mRenderList->polyData.size();
		//ִ����Ⱦ
		if (mCamera->getRenderMode() == RENDER_WIRE)
		{
			Draw_RenderList4D_Wire(mRenderList);
		}
		else
		{
			Draw_RenderList4D_Texture_Solid(mRenderList);
		}
	}

	//����һ�ƹ�
	ELight* ESceneManager::createLight(LIGHT_TYPE lightType)
	{
		EInt light = CreateLight(lightType);
		return getLight(light);
	}
	//��ȡ�ƹ�
	ELight *ESceneManager::getLight(EInt id)
	{
		return GetLight(id);
	}
	//�������رյƹ⣨ȫ�֣�
	void ESceneManager::enableLights(EBool enable)
	{
		mLightEnable = enable;
		for (EInt i = 0; i < GetLightSize(); i++)
		{
			getLight(i)->lightOn = enable;
		}
	}
}
