#pragma

#include "ECommon.h"
#include "EVector.h"
#include <list>
#include <vector>

namespace E3D
{
#define EPOLY_ATTR_2SIDE					0x0001		//双面
#define EPOLY_ATTR_TRANSPARENT				0x0002		//透明

#define EPOLY_ATTR_SHADE_MODE_PURE			0x0004
#define EPOLY_ATTR_SHADE_MODE_CONSTANT		0x0004
#define EPOLY_ATTR_SHADE_MODE_FLAT			0x0008
#define EPOLY_ATTR_SHADE_MODE_GOURAUD		0x0010
#define EPOLY_ATTR_SHADE_MODE_PHONG			0x0020
#define EPOLY_ATTR_SHADE_MODE_FASTPHONE		0x0020
#define EPOLY_ATTR_SHADE_MODE_TEXTURE		0x0040

#define EPOLY_ATTR_VERTEX_POSITION			0x1000
#define EPOLY_ATTR_VERTEX_NORMAL			0x2000
#define EPOLY_ATTR_VERTEX_UV				0x4000

#define EPOLY_STATE_ACTIVE					0x0100		//有效状态
#define EPOLY_STATE_CLIPPED					0x0200		//被裁剪状态
#define EPOLY_STATE_BACKFACE				0x0400		//背面状态

#define EOBJECT_STATE_ACTIVE				0x0001
#define	EOBJECT_STATE_VISIBLE				0x0002
#define EOBJECT_STATE_CULLED				0x0004

	//物体剔除类型
	enum CULL_TYPE
	{
		CULL_BOX = 0,		//物体包围盒AABB剔除
		CULL_SPHERE = 1,	//物体球体积剔除
	};

	typedef std::vector<EVertex4D> EVertex4DArray;
	typedef std::vector<EVertex4D> EVector4DArray;
	typedef std::vector<EPolyon4D> EPolyon4DArray;

	struct EVertex4D
	{
		EFloat x, y, z, w;
		EFloat u, v;

		EVertex4D();
		EVertex4D(const EVertex4D &v);
		EVertex4D(EFloat xf, EFloat yf, EFloat zf);
		EVertex4D operator *(const EVector4D &right);
		EVertex4D operator +(const EVertex4D &right);
		EVertex4D operator +(const EVector4D &right);
		EVertex4D& operator =(const EVertex4D &right);
		EVertex4D operator -(const EVertex4D &right);
		EVertex4D operator /(EFloat factor);

		EVector4D toVector4D() const;
	};

	struct EPlane3D
	{
		//面上一点
		EVector4D point;
		//面的法线
		EVector4D normal;

		EPlane3D() {}
		EPlane3D(const EVector4D &p, const EVector4D &nor);
		EPlane3D& operator = (const EPlane3D &right);
	};

	//此多边形结构是基于顶点索引的方式的数据结构
	struct EPolyon4D
	{
		EInt state;
		EInt attribute;
		EInt color;
		//是指transfromList
		EVertex4DArray *verList;
		EInt verIndex[3];

		EPolyon4D();
	};


	struct EPolyonF4D
	{
		EInt state;			//状态信息
		EInt attribute;		//物理属性
		EInt color;			//光照强度颜色

		EVertex4D localList[3];		//物体局部坐标
		EVertex4D transformList[3];		//局部坐标变换后的坐标
	
		struct EMaterial *material;

		EPolyonF4D();
	};

	struct CompEPolyonF4D
	{
		EBool operator()(const EPolyonF4D &polyA, const EPolyonF4D &polyB)
		{
			EFloat zA = max(polyA.transformList[0].z, max(polyA.transformList[1].z, polyA.transformList[2].z));
			EFloat zB = max(polyB.transformList[0].z, max(polyB.transformList[1].z, polyB.transformList[2].z));

			if (Abs(zA - zB) < 0.005f)
			{
				zA = (polyA.transformList[0].z + polyA.transformList[1].z + polyA.transformList[2].z) / 3.0f;
				zB = (polyB.transformList[0].z + polyB.transformList[1].z + polyB.transformList[2].z) / 3.0f;
				if (zA <= zB)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			else if (zA < zB)
			{
				return false;
			}
			else 
			{
				return true;
			}
		}
	};

	//基于顶点列表和多边形列表的物体
	struct EObject4D
	{
		EString		name;		//物体名字
		EInt		state;		//物体状态
		EInt		attribute;	//物体属性
		EFloat		avgRadius;	//物体的平局半径，用于碰撞检测
		EFloat		maxRadius;	//物体最大半径

		EBool		needCull;	//是否执行剔除
		EString		materialName;	//物体材质名称

		EVector4D	minBoundingBox;		//包围盒最小点
		EVector4D	maxBoundingBox;		//包围盒最大点

		EVector4D	worldPostion;		//世界坐标
		EVector4D	scale;				//缩放值

		EVector4D	direction;			//记录各个轴的旋转角度
		EInt		vertexNumber;		//顶点个数

		EVector4DArray localList;		//物体变换钱的物体局部坐标数组
		EVector4DArray transformList;	//物体变换后的坐标

		EInt polyonNumber;				//物体所包含的多边形的数组
		EPolyon4DArray polyonList;		//存储多边形数组
		//这个变量是为了处理一个导入的mesh有多个submesh的情况来考虑的
		EObject4D *nextObject;

		EObject4D();
	};

	//渲染列表
	struct ERenderList4D
	{
		EInt  state;		//物体状态
		EInt	attribute;	//物体属性
		//具体的多边形数据，使用双端队列 删除更有效
		std::list<EPolyonF4D> polyData;

		typedef std::list<EPolyonF4D>::iterator Itr;
	};

	//顶点变换控制标记
	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,	//只对局部顶点列表进行变换
		TRANSFORM_TRANS = 1,	//只对变换后的顶点列表进行变换
		TRANSFORM_LOCAL_TO_TRANS = 2,	//只对局部顶点列表进行变换，并将结果存在变换后的顶点列表里面
	};

	//对EObject进行变换，可位移和旋转、
	//transformType 指定了对哪个顶点列表进行变换
	//transformBasis 指定了是否对朝向向量进行变换
	void Transform_Object4D(EObject4D *obj, const EMatrix44 &mat, OBJ_TRANSFORM_TYPE transformType);

	//对ERenderList4D进行变换  可以进行位移变换和旋转变换
	//transformType 指定了对哪一个顶点列表进行变换
	void Transform_Renderlist4D(ERenderList4D *renderList, const EMatrix44 &mat, OBJ_TRANSFORM_TYPE transformType);
	
	//将Object插入RenderList,这里LocalList存储的是原始坐标，TransformList存储的是经过平移缩放后的坐标
	void Insert_Object4D_To_RenderList4D(ERenderList4D *renderList, EObject4D *obj, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS, bool transformScaleAndPosition = false);

	void Local_To_World_Object4D(EObject4D *obj, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	void Local_To_World_RenderList4D(ERenderList4D *reanderList, const EVector4D &pos, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	//剔除
	bool Cull_Object4D(EObject4D *object, EFrustum *camera, CULL_TYPE cullType = CULL_SPHERE);

	//消除背面，背面消除算法的射线机视点是基于世界坐标的，所有需要将顶点变换成摄像机坐标前 进行背面消除
	bool Remove_Backface(EPolyon4D *poly, EFrustum *camera);
	bool Remove_Backface(EPolyonF4D *poly, EFrustum *camera);
	void Remove_Backface_Obejct4D(EObject4D *object, EFrustum *camera);
	void Remove_Backface_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	//light 是否对多边形进行光照处理
	//光照计算
	//光照计算只能在世界坐标系消除背面之后，或者在相机坐标系中进行
	void Light_PolyonF4D(EPolyonF4D *poly, EFrustum *camera);
	void Light_RenderList4D(ERenderList4D *renderLIst, EFrustum *camera);

	//世界坐标转为相机坐标
	void World_To_Camera_Object4D(EObject4D *obj, EFrustum *camera);
	void World_To_Camera_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	//对RenderList的多边形进行深度排序
	void Sort_RenderList4D(ERenderList4D *renderList);

	//相机坐标系转换为透视坐标系
	void Camera_To_Perspective_Object4D(EObject4D *obj, EFrustum *camera);
	void Camera_To_Perspective_RenderList(ERenderList4D *renderList, EFrustum *camera);

	//透视坐标转屏幕坐标
	void Perspective_To_Screen_Object4D(EObject4D *obj, EFrustum *camera);
	void Perspective_To_Screen_RenderList(ERenderList4D *renderList, EFrustum *camera);

	//对RenderList4D 进行裁剪坐标 裁减掉超出摄像机范围内的多边形  需在相机坐标系中进行
	void Clip_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	void World_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	//渲染线框模型
	void Draw_Object4D_Wire(EObject4D *obj);
	void Draw_RenderList4D_Wire(ERenderList4D *renderList);

	//渲染实体模型
	void Draw_Object4D_Solid(EObject4D *object);
	void Draw_RenderList4D_Solid(ERenderList4D *renderList);

	//绘制带纹理的renderlist
	void Draw_RenderList4D_Texture_Solid(ERenderList4D *renderList);
}
