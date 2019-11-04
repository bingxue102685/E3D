#pragma

#include "ECommon.h"
#include "EVector.h"
#include <list>
#include <vector>

namespace E3D
{
#define EPOLY_ATTR_2SIDE					0x0001		//˫��
#define EPOLY_ATTR_TRANSPARENT				0x0002		//͸��

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

#define EPOLY_STATE_ACTIVE					0x0100		//��Ч״̬
#define EPOLY_STATE_CLIPPED					0x0200		//���ü�״̬
#define EPOLY_STATE_BACKFACE				0x0400		//����״̬

#define EOBJECT_STATE_ACTIVE				0x0001
#define	EOBJECT_STATE_VISIBLE				0x0002
#define EOBJECT_STATE_CULLED				0x0004

	//�����޳�����
	enum CULL_TYPE
	{
		CULL_BOX = 0,		//�����Χ��AABB�޳�
		CULL_SPHERE = 1,	//����������޳�
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
		//����һ��
		EVector4D point;
		//��ķ���
		EVector4D normal;

		EPlane3D() {}
		EPlane3D(const EVector4D &p, const EVector4D &nor);
		EPlane3D& operator = (const EPlane3D &right);
	};

	//�˶���νṹ�ǻ��ڶ��������ķ�ʽ�����ݽṹ
	struct EPolyon4D
	{
		EInt state;
		EInt attribute;
		EInt color;
		//��ָtransfromList
		EVertex4DArray *verList;
		EInt verIndex[3];

		EPolyon4D();
	};


	struct EPolyonF4D
	{
		EInt state;			//״̬��Ϣ
		EInt attribute;		//��������
		EInt color;			//����ǿ����ɫ

		EVertex4D localList[3];		//����ֲ�����
		EVertex4D transformList[3];		//�ֲ�����任�������
	
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

	//���ڶ����б�Ͷ�����б������
	struct EObject4D
	{
		EString		name;		//��������
		EInt		state;		//����״̬
		EInt		attribute;	//��������
		EFloat		avgRadius;	//�����ƽ�ְ뾶��������ײ���
		EFloat		maxRadius;	//�������뾶

		EBool		needCull;	//�Ƿ�ִ���޳�
		EString		materialName;	//�����������

		EVector4D	minBoundingBox;		//��Χ����С��
		EVector4D	maxBoundingBox;		//��Χ������

		EVector4D	worldPostion;		//��������
		EVector4D	scale;				//����ֵ

		EVector4D	direction;			//��¼���������ת�Ƕ�
		EInt		vertexNumber;		//�������

		EVector4DArray localList;		//����任Ǯ������ֲ���������
		EVector4DArray transformList;	//����任�������

		EInt polyonNumber;				//�����������Ķ���ε�����
		EPolyon4DArray polyonList;		//�洢���������
		//���������Ϊ�˴���һ�������mesh�ж��submesh����������ǵ�
		EObject4D *nextObject;

		EObject4D();
	};

	//��Ⱦ�б�
	struct ERenderList4D
	{
		EInt  state;		//����״̬
		EInt	attribute;	//��������
		//����Ķ�������ݣ�ʹ��˫�˶��� ɾ������Ч
		std::list<EPolyonF4D> polyData;

		typedef std::list<EPolyonF4D>::iterator Itr;
	};

	//����任���Ʊ��
	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,	//ֻ�Ծֲ������б���б任
		TRANSFORM_TRANS = 1,	//ֻ�Ա任��Ķ����б���б任
		TRANSFORM_LOCAL_TO_TRANS = 2,	//ֻ�Ծֲ������б���б任������������ڱ任��Ķ����б�����
	};

	//��EObject���б任����λ�ƺ���ת��
	//transformType ָ���˶��ĸ������б���б任
	//transformBasis ָ�����Ƿ�Գ����������б任
	void Transform_Object4D(EObject4D *obj, const EMatrix44 &mat, OBJ_TRANSFORM_TYPE transformType);

	//��ERenderList4D���б任  ���Խ���λ�Ʊ任����ת�任
	//transformType ָ���˶���һ�������б���б任
	void Transform_Renderlist4D(ERenderList4D *renderList, const EMatrix44 &mat, OBJ_TRANSFORM_TYPE transformType);
	
	//��Object����RenderList,����LocalList�洢����ԭʼ���꣬TransformList�洢���Ǿ���ƽ�����ź������
	void Insert_Object4D_To_RenderList4D(ERenderList4D *renderList, EObject4D *obj, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS, bool transformScaleAndPosition = false);

	void Local_To_World_Object4D(EObject4D *obj, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	void Local_To_World_RenderList4D(ERenderList4D *reanderList, const EVector4D &pos, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	//�޳�
	bool Cull_Object4D(EObject4D *object, EFrustum *camera, CULL_TYPE cullType = CULL_SPHERE);

	//�������棬���������㷨�����߻��ӵ��ǻ�����������ģ�������Ҫ������任�����������ǰ ���б�������
	bool Remove_Backface(EPolyon4D *poly, EFrustum *camera);
	bool Remove_Backface(EPolyonF4D *poly, EFrustum *camera);
	void Remove_Backface_Obejct4D(EObject4D *object, EFrustum *camera);
	void Remove_Backface_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	//light �Ƿ�Զ���ν��й��մ���
	//���ռ���
	//���ռ���ֻ������������ϵ��������֮�󣬻������������ϵ�н���
	void Light_PolyonF4D(EPolyonF4D *poly, EFrustum *camera);
	void Light_RenderList4D(ERenderList4D *renderLIst, EFrustum *camera);

	//��������תΪ�������
	void World_To_Camera_Object4D(EObject4D *obj, EFrustum *camera);
	void World_To_Camera_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	//��RenderList�Ķ���ν����������
	void Sort_RenderList4D(ERenderList4D *renderList);

	//�������ϵת��Ϊ͸������ϵ
	void Camera_To_Perspective_Object4D(EObject4D *obj, EFrustum *camera);
	void Camera_To_Perspective_RenderList(ERenderList4D *renderList, EFrustum *camera);

	//͸������ת��Ļ����
	void Perspective_To_Screen_Object4D(EObject4D *obj, EFrustum *camera);
	void Perspective_To_Screen_RenderList(ERenderList4D *renderList, EFrustum *camera);

	//��RenderList4D ���вü����� �ü��������������Χ�ڵĶ����  �����������ϵ�н���
	void Clip_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	void World_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	//��Ⱦ�߿�ģ��
	void Draw_Object4D_Wire(EObject4D *obj);
	void Draw_RenderList4D_Wire(ERenderList4D *renderList);

	//��Ⱦʵ��ģ��
	void Draw_Object4D_Solid(EObject4D *object);
	void Draw_RenderList4D_Solid(ERenderList4D *renderList);

	//���ƴ������renderlist
	void Draw_RenderList4D_Texture_Solid(ERenderList4D *renderList);
}
