#include "EMesh.h"
#include "EMeshUtil.h"
#include "EMatrix.h"
#include "EMath.h"

namespace E3D
{
	EMesh::EMesh() :mObject(NULL), mSubmeshNumber(1), mCollsionType(COLLISION_COLLSION), mPolyonNumber(0), mVertexNumber(0), mRotate(0, 0, 0), mIsVisible(true)
	{

	}

	EMesh::EMesh(const EString &name, const EString &meshName)
	{
		mSubmeshNumber = 0;
		mPolyonNumber = 0;
		mVertexNumber = 0;
		mRotate = EVector3D(0, 0, 0);
		mCollsionType = COLLISION_COLLSION;
		mIsVisible = true;

		mName = name;
		mObject = LoadOgreMesh(meshName);

		if (!mObject)
		{
			EString errorInfo = "Mesh#" + meshName + "Not Found!";
			throw errorInfo;
		}

		EObject4D *obj = mObject;
		while (obj)
		{
			mSubmeshNumber++;
			mPolyonNumber += obj->polyonNumber;
			mVertexNumber += obj->vertexNumber;
			obj = obj->nextObject;
		}
	}

	EMesh::~EMesh()
	{
		SafeDelete(mObject);
	}

	void EMesh::setScale(const EVector3D &scale)
	{
		mObject->scale = EVector4D(scale.x, scale.y, scale.z);
	}

	void EMesh::setPosition(const EVector3D &pos)
	{
		mObject->worldPostion = EVector4D(pos.x, pos.y, pos.z);
	}

	EVector3D EMesh::getPosition() const
	{
		return EVector3D(mObject->worldPostion.x, mObject->worldPostion.x, mObject->worldPostion.z);
	}

	void EMesh::move(const EVector3D &move)
	{
		mObject->worldPostion = mObject->worldPostion + EVector4D(move.x, move.y, move.z);
	}

	void EMesh::yaw(EFloat yDegree)
	{
		mRotate.y = yDegree;
		EMatrix44 rotMat;
		GetRotateMatrix44Y(rotMat, yDegree);
		Transform_Object4D(mObject, rotMat, TRANSFORM_LOCAL_TO_TRANS);
	}

	void EMesh::roll(EFloat rDegree)
	{
		mRotate.z = rDegree;
		EMatrix44 rotMat;
		GetRotateMatrix44Z(rotMat, rDegree);
		Transform_Object4D(mObject, rotMat, TRANSFORM_LOCAL_TO_TRANS);
	}

	void EMesh::pitch(EFloat pDegree)
	{
		mRotate.x = pDegree;
		EMatrix44 rotMat;
		GetRotateMatrix44X(rotMat, pDegree);
		Transform_Object4D(mObject, rotMat, TRANSFORM_LOCAL_TO_TRANS);
	}

	EMaterial* EMesh::getMaterial(EInt index)
	{
		if (index < 0 || index >= mSubmeshNumber)
			return NULL;

		EObject4D *object = mObject;
		EInt temp = 0;
		while (temp < index)
		{
			temp++;
			object = object->nextObject;
		}
		if (object)
			return GetMaterial(object->materialName);

		return NULL;
	}

	void EMesh::setMatrial(EInt index, EString matName)
	{
		if (index < 0 || index >= mSubmeshNumber)
			return;

		EObject4D *object = mObject;
		EInt temp = 0;

		while (temp < index)
		{
			temp++;
			object = object->nextObject;
		}
		if (object)
			object->materialName = matName;
	}

	EBool EMesh::intersect(const EVector3D &point)
	{
		EFloat x = Abs(mObject->maxBoundingBox.x) * mObject->scale.x / 2.0f;
		EFloat y = Abs(mObject->maxBoundingBox.y) * mObject->scale.y / 2.0f;
		EFloat z = Abs(mObject->maxBoundingBox.z) * mObject->scale.z / 2.0f;

		if (point.x < mObject->worldPostion.x - x || point.x  > mObject->worldPostion.x + x)
		{
			return false;
		}

		if (point.y < mObject->worldPostion.y - y || point.x  > mObject->worldPostion.y + y)
		{
			return false;
		}

		if (point.z < mObject->worldPostion.z - z || point.z  > mObject->worldPostion.z + z)
		{
			return false;
		}

		return true;
	}

	EMesh* EMesh::clone()
	{
		EMesh *mesh = new EMesh();
		{
			mesh->mName = mName + "_Clone";
			mesh->mPolyonNumber = mPolyonNumber;
			mesh->mRotate = mRotate;
			mesh->mSubmeshNumber = mSubmeshNumber;
			mesh->mVertexNumber = mVertexNumber;

			EObject4D *object = new EObject4D();
			{
				object->attribute = mObject->attribute;
				object->avgRadius = mObject->avgRadius;
				object->direction = mObject->direction;
				object->localList = mObject->localList;
				object->transformList = mObject->transformList;
				object->materialName = mObject->materialName;
				object->maxBoundingBox = mObject->maxBoundingBox;
				object->maxRadius = mObject->maxRadius;
				object->minBoundingBox = mObject->minBoundingBox;
				object->needCull = mObject->needCull;
				object->name = mObject->name;
				object->polyonList = mObject->polyonList;
				object->polyonNumber = mObject->polyonNumber;
				object->scale = mObject->scale;
				object->state = mObject->state;
				object->vertexNumber = mObject->vertexNumber;
				object->worldPostion = mObject->worldPostion;
				object->nextObject = NULL;

			}
			mesh->mObject = object;
			mesh->mIsVisible = true;
		}
		return mesh;
	}
}
