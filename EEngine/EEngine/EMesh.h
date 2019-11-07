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
		//顶点是否在Mesh的包围盒中，是返回true 不是返回false
		EBool intersect(const EVector3D &point);

		void setVisible(EBool visible) { mIsVisible = visible; };
		EBool isVisible() const { return isVisible; };

		//是否要进行剔除操作
		void setCullFlag(EBool needCull) { mObject->needCull = needCull; };

		//绕y轴旋转
		void yaw(EFloat yDegree);
		//绕z轴旋转
		void roll(EFloat rDegree);
		//绕x轴旋转
		void pitch(EFloat pDegree);

		void setCollsionType(COLLSION_TYPE type) { mCollsionType = type; };
		COLLSION_TYPE getCollsionType() const { return mCollsionType; };

		EObject4D* getObject() const { return mObject; };
		EInt getPolyonNumber() const { return mPolyonNumber; };
		EInt getVertexNumber() const { return mVertexNumber; };
		EInt getSubmeshNumber() const { return mSubmeshNumber; };

		EMaterial* getMaterial(EInt index);
		void setMatrial(EInt index, EString matName);

		//获取各轴的旋转分量 
		EVector3D getRotateVec() const { return mRotate; };
		//对此模型进行clone 避免再次io
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
