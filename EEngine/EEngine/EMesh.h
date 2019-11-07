#pragma once

#include "ECommon.h"
#include "ECore.h"
#include "EMaterial.h"
#include "EObject.h"

namespace E3D
{
	enum COLLSION_TYPE
	{
		COLLISION_COLLSION,
		COLLISION_IGNORE,
	};

	class EMesh : public EObject
	{
	public:
		~EMesh();

		void setScale(const EVector3D &scale);
		void setPosition(const EVector3D &pos);
		EVector3D getPosition()const;
		void move(const EVector3D &move);
		//�����Ƿ���Mesh�İ�Χ���У��Ƿ���true ���Ƿ���false
		EBool intersect(const EVector3D &point);

		void setVisible(EBool visible) { mIsVisible = visible; };
		EBool isVisible() const { return isVisible; };

		//�Ƿ�Ҫ�����޳�����
		void setCullFlag(EBool needCull) { mObject->needCull = needCull; };

		//��y����ת
		void yaw(EFloat yDegree);
		//��z����ת
		void roll(EFloat rDegree);
		//��x����ת
		void pitch(EFloat pDegree);

		void setCollsionType(COLLSION_TYPE type) { mCollsionType = type; };
		COLLSION_TYPE getCollsionType() const { return mCollsionType; };

		EObject4D* getObject() const { return mObject; };
		EInt getPolyonNumber() const { return mPolyonNumber; };
		EInt getVertexNumber() const { return mVertexNumber; };
		EInt getSubmeshNumber() const { return mSubmeshNumber; };

		EMaterial* getMaterial(EInt index);
		void setMatrial(EInt index, EString matName);

		//��ȡ�������ת���� 
		EVector3D getRotateVec() const { return mRotate; };
		//�Դ�ģ�ͽ���clone �����ٴ�io
		EMesh* clone();
	protected:
		friend class ESceneManager;

		EMesh();
		EMesh(const EString &name, const EString &meshName);

		EObject4D *mObject;
		EInt mPolyonNumber;
		EInt mVertexNumber;
		EInt mSubmeshNumber;

		EVector3D mRotate;
		EBool mIsVisible;
		COLLSION_TYPE mCollsionType;
	};
}
