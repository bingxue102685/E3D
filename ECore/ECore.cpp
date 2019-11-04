#include "ECore.h"
#include "EMath.h"
#include "ELight.h"
#include "EMaterial.h"
#include "EFrustum.h"
#include "EVector.h"

namespace E3D
{

	EVertex4D::EVertex4D() : x(0.0f), y(0.0f), z(0.0f), w(1.0f), u(0.0f), v(0.0f) {}

	EVertex4D::EVertex4D(const EVertex4D &v) : x(v.x), y(v.y), z(v.z), w(v.w), u(v.u), v(v.v) {}

	EVertex4D::EVertex4D(EFloat xf, EFloat yf, EFloat zf) : x(xf), y(yf), z(zf), u(0), v(v) {}

	EVertex4D EVertex4D::operator*(const EVector4D &right)
	{
		EVertex4D temp = *this;
		temp.x *= right.x;
		temp.y *= right.y;
		temp.z *= right.z;
		return temp;
	}

	EVertex4D EVertex4D::operator+(const EVertex4D &right)
	{
		EVertex4D temp = *this;
		temp.x += right.x;
		temp.y += right.y;
		temp.z += right.z;
		return temp;
	}

	EVertex4D  EVertex4D::operator +(const EVector4D &right)
	{
		EVertex4D temp = *this;
		temp.x += right.x;
		temp.y += right.y;
		temp.z += right.z;
		return temp;
	}

	EVertex4D EVertex4D::operator-(const EVertex4D &right)
	{
		EVertex4D temp = *this;
		temp.x -= right.x;
		temp.y -= right.y;
		temp.z -= right.z;
		return temp;
	}

	EVertex4D EVertex4D::operator /(EFloat factor)
	{
		EVertex4D temp = *this;
		temp.x /= factor;
		temp.y /= factor;
		temp.z /= factor;
		return temp;
	}

	EVertex4D& EVertex4D::operator =(const EVertex4D &right)
	{
		x = right.x;	y = right.y;	z = right.z;	w = right.w;
		u = right.u;	v = right.v;
		return *this;
	}

	EVector4D EVertex4D::toVector4D() const
	{
		return EVector4D(x, y, z, w);
	}


	EPlane3D::EPlane3D(const EVector4D &p, const EVector4D &nor)
	{
		point = p;
		normal = nor;
		normal.normalize();
	}

	EPlane3D& EPlane3D::operator=(const EPlane3D &right)
	{
		point = right.point;
		normal = right.normal;
		return *this;
	}

	EPolyon4D::EPolyon4D() :state(0), attribute(0), color(0xffffff), verList(NULL) {}

	EPolyonF4D::EPolyonF4D(): state(0), attribute(0), color(0xffffff) {}

	EObject4D::EObject4D():nextObject(NULL), materialName(DEFAULT_NAME), scale(1, 1, 1), direction(EVector4D::UNIT_X), needCull(true) {}

	void Transform_Object4D(EObject4D *object, const EMatrix44 &mat, OBJ_TRANSFORM_TYPE transformType)
	{
		EObject4D *obj = object;
		switch (transformType)
		{
		case E3D::TRANSFORM_LOCAL:
		{
			while (obj)
			{
				//只对局部坐标进行变换
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DMulMatrix44(obj->localList[i], mat, obj->localList[i]);
				}
				obj = obj->nextObject;
			}
		}
		break;
		case E3D::TRANSFORM_TRANS:
		{
			while (obj)
			{
				//对变换后的坐标进行变换
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DMulMatrix44(obj->transformList[i], mat, obj->transformList[i]);
				}
			}
		}
		break;
		case E3D::TRANSFORM_LOCAL_TO_TRANS:
		{
			while (obj)
			{
				//局部坐标变换后存储值变换后的坐标顶点中
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DMulMatrix44(obj->localList[i], mat, obj->transformList[i]);
				}
			}
		}
		break;
		default:
			break;
		}
	}

	void Transform_Renderlist4D(ERenderList4D *renderList, const EMatrix44 &mat, OBJ_TRANSFORM_TYPE transformType)
	{
		EPolyonF4D *poly = NULL;
		switch (transformType)
		{
		case E3D::TRANSFORM_LOCAL:
		{
			for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
			{
				poly = &(*iter);

				GetVertex4DMulMatrix44(poly->localList[0], mat, poly->localList[0]);
				GetVertex4DMulMatrix44(poly->localList[1], mat, poly->localList[1]);
				GetVertex4DMulMatrix44(poly->localList[2], mat, poly->localList[2]);
			}
		}
			break;
		case E3D::TRANSFORM_TRANS:
		{
			for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
			{
				poly = &(*iter);
				GetVertex4DMulMatrix44(poly->transformList[0], mat, poly->transformList[0]);
				GetVertex4DMulMatrix44(poly->transformList[1], mat, poly->transformList[1]);
				GetVertex4DMulMatrix44(poly->transformList[2], mat, poly->transformList[2]);
			}
		}
			break;
		case E3D::TRANSFORM_LOCAL_TO_TRANS:
		{
			for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
			{
				poly = &(*iter);
				GetVertex4DMulMatrix44(poly->localList[0], mat, poly->transformList[0]);
				GetVertex4DMulMatrix44(poly->localList[1], mat, poly->transformList[1]);
				GetVertex4DMulMatrix44(poly->localList[2], mat, poly->transformList[2]);
			}
		}
			break;
		default:
			break;
		}
	}

	void Insert_Object4D_To_RenderList4D(ERenderList4D *renderList, EObject4D *object, OBJ_TRANSFORM_TYPE transformType, bool transformScaleAndPosition)
	{
		if (!object)
		{
			return;
		}

		EVector4D scale(1, 1, 1);
		EVector4D pos(0, 0, 0);

		if (transformScaleAndPosition)
		{
			scale = object->scale;
			pos = object->worldPostion;
		}

		//备份灯光数据
		ELight *light = NULL;
		for (LightIter iter = GLights->begin(); iter != GLights->end(); iter++)
		{
			light = *iter;
			light->transPostion = light->position;
			light->transDirection = light->direction;
		}

		EObject4D *obj = object;
		EPolyon4D *base;
		EPolyonF4D poly;

		switch (transformType)
		{
		case E3D::TRANSFORM_LOCAL:
			while (obj)
			{
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];
					poly.attribute = base->attribute;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.color = base->color;
					poly.localList[0] = obj->localList[base->verIndex[0]];
					poly.localList[1] = obj->localList[base->verIndex[1]];
					poly.localList[2] = obj->localList[base->verIndex[2]];
					//设置材质
					poly.material = GetMaterial(obj->materialName);

					renderList->polyData.push_back(poly);
				}

				obj = obj->nextObject;
			}
			break;
		case E3D::TRANSFORM_TRANS:
			while (obj)
			{
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute = base->attribute;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.transformList[0] = obj->transformList[base->verIndex[0]] * scale + pos;
					poly.transformList[1] = obj->transformList[base->verIndex[1]] * scale + pos;
					poly.transformList[2] = obj->transformList[base->verIndex[2]] * scale + pos;

					poly.material = GetMaterial(obj->materialName);

					renderList->polyData.push_back(poly);
				}
			}
			break;
		case E3D::TRANSFORM_LOCAL_TO_TRANS:
			while (obj)
			{
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					base = &obj->polyonList[i];

					poly.attribute = base->attribute;
					poly.state = EPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.transformList[0] = obj->localList[base->verIndex[0]] * scale + pos;
					poly.transformList[1] = obj->localList[base->verIndex[1]] * scale + pos;
					poly.transformList[2] = obj->localList[base->verIndex[2]] * scale + pos;

					poly.material = GetMaterial(obj->materialName);

					renderList->polyData.push_back(poly);
				}
			}
			break;
		default:
			break;
		}
	}

	void Local_To_World_Object4D(EObject4D *object, OBJ_TRANSFORM_TYPE transformType)
	{
		if (!object)
		{
			return;
		}

		EObject4D *obj = object;
		EVector4D pos = obj->worldPostion;
		switch (transformType)
		{
		case E3D::TRANSFORM_LOCAL:
			break;
		case E3D::TRANSFORM_TRANS:
			while (obj)
			{
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DAddVector4D(obj->transformList[i], pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
			break;
		case E3D::TRANSFORM_LOCAL_TO_TRANS:
			while (obj)
			{
				for (EInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4DAddVector4D(obj->localList[i] * obj->scale, pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
			break;
		default:
			break;
		}
	}

	void Local_To_World_RenderList4D(ERenderList4D *renderList, const EVector4D &pos, OBJ_TRANSFORM_TYPE transformType)
	{
		EPolyonF4D *poly = NULL;
		switch (transformType)
		{
		case E3D::TRANSFORM_LOCAL:
		{
			for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
			{
				poly = &(*iter);
				GetVertex4DAddVector4D(poly->localList[0], pos, poly->localList[0]);
				GetVertex4DAddVector4D(poly->localList[1], pos, poly->localList[1]);
				GetVertex4DAddVector4D(poly->localList[2], pos, poly->localList[2]);
			}
		}
			break;
		case E3D::TRANSFORM_TRANS:
		{
			for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
			{
				poly = &(*iter);
				GetVertex4DAddVector4D(poly->transformList[0], pos, poly->transformList[0]);
				GetVertex4DAddVector4D(poly->transformList[1], pos, poly->transformList[1]);
				GetVertex4DAddVector4D(poly->transformList[2], pos, poly->transformList[2]);
			}
		}
			break;
		case E3D::TRANSFORM_LOCAL_TO_TRANS:
		{
			for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
			{
				poly = &(*iter);
				GetVertex4DAddVector4D(poly->localList[0], pos, poly->transformList[0]);
				GetVertex4DAddVector4D(poly->localList[1], pos, poly->transformList[1]);
				GetVertex4DAddVector4D(poly->localList[2], pos, poly->transformList[2]);
			}
		}
			break;
		default:
			break;
		}
	}

	bool Cull_Object4D(EObject4D *object, EFrustum *camera, CULL_TYPE cullType)
	{
		if (!object)
		{
			return true;
		}

		if (!object->needCull)
		{
			return false;
		}

		//先把当前物体的坐标转为相机坐标
		EVector4D centerPos;
		GetVector4DMulMatrix44(object->worldPostion, camera->mWorldToCamera, centerPos);

		//首先把半径进行缩放
		EFloat scaleRadiusZ;
		EFloat scaleRadiusX;
		EFloat scaleRadiusY;

		if (cullType == CULL_SPHERE)
		{
			scaleRadiusZ = object->maxRadius * object->scale.z;
			scaleRadiusY = object->maxRadius * object->scale.y;
			scaleRadiusX = object->maxRadius * object->scale.x;
		}
		else if (cullType == CULL_BOX)
		{
			scaleRadiusZ = object->maxBoundingBox.z * object->scale.z;
			scaleRadiusY = object->maxBoundingBox.y * object->scale.y;
			scaleRadiusX = object->maxBoundingBox.x * object->scale.x;
		}
		else
		{
			return true;
		}

		//远近面剔除
		if (centerPos.z + scaleRadiusZ < camera->clip_z_near || centerPos.z - scaleRadiusZ > camera->clip_z_far)
		{
			return true;
		}

		//左右面剔除
		EFloat x_test = 0.5f * camera->viewplane_width * centerPos.z / camera->view_dist;
		if (centerPos.x - scaleRadiusX > x_test || centerPos.x + scaleRadiusX < -x_test)
		{
			return true;
		}
		
		//上下面剔除
		EFloat y_test = 0.5f * camera->viewplane_height * centerPos.z / camera->view_dist;
		if (centerPos.y - scaleRadiusY > y_test || centerPos.y + scaleRadiusY < -y_test)
		{
			return true;
		}

		return false;
	}

	void World_To_Camera_Object4D(EObject4D *obj, EFrustum *camera)
	{
		EObject4D *object = obj;
		while (object)
		{
			for (EInt i = 0; i < object->vertexNumber; i++)
			{
				GetVertex4DMulMatrix44(object->transformList[i], camera->mWorldToCamera, object->transformList[i]);
			}
			object = obj->nextObject;
		}
	}

	void World_To_Camera_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		//这里对灯光的位置进行变换，将其变换到相机坐标系下面
		EMatrix44 mat = camera->mWorldToCamera;
		//只保留旋转 缩放 ，不进行平移变换，否则会出现灯光跟随相机移动的效果
		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0;

		ELight *light = NULL;
		for (LightIter iter = GLights->begin(); iter != GLights->end(); iter++)
		{
			light = *iter;
			GetVector4DMulMatrix44(light->position, mat, light->transPostion);
			GetVector4DMulMatrix44(light->direction, mat, light->transDirection);
		}

		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
		{
			poly = &(*iter);
			for (EInt i = 0; i < 3; i++)
			{
				GetVertex4DMulMatrix44(poly->transformList[i], camera->mWorldToCamera, poly->transformList[i]);
			}
		}
	}

	void Sort_RenderList4D(ERenderList4D *renderList)
	{
		renderList->polyData.sort(CompEPolyonF4D());
	}

	//背面剔除
	bool Remove_Backface(EPolyon4D *poly, EFrustum *camera)
	{
		EInt index0 = poly->verIndex[0];
		EInt index1 = poly->verIndex[1];
		EInt index2 = poly->verIndex[2];

		EVector4D u = (poly->verList->at(index0) - poly->verList->at(index1)).toVector4D();
		EVector4D v = (poly->verList->at(index0) - poly->verList->at(index2)).toVector4D();

		//多边形法线
		EVector4D n = u.cross(v);
		EVector4D view = camera->position - poly->verList->at(index0).toVector4D();

		//点积 小于0 则是背向相机的
		EFloat dot = n.dot(view);
		if (dot < 0.0f)
		{
			poly->state |= EPOLY_STATE_BACKFACE;
			return true;
		}
		return false;
	}
	bool Remove_Backface(EPolyonF4D *poly, EFrustum *camera)
	{
		if (!(poly->state & EPOLY_STATE_ACTIVE) || (poly->state & EPOLY_STATE_BACKFACE) || (poly->state & EPOLY_STATE_CLIPPED))
		{
			return true;
		}

		EVector4D u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		EVector4D v = (poly->transformList[0] - poly->transformList[2]).toVector4D();

		EVector4D normal = u.cross(v);
		EVector4D view = camera->position - poly->transformList[0].toVector4D();

		EFloat dot = normal.dot(view);
		if (dot < 0)
		{
			poly->state |= EPOLY_STATE_BACKFACE;
			return true;
		}
		return false;
	}

	void Remove_Backface_Obejct4D(EObject4D *object, EFrustum *camera)
	{
		EObject4D *obj = object;
		EPolyon4D *poly = NULL;
		while (obj)
		{
			if (!(obj->state & EOBJECT_STATE_CULLED))
			{
				for (EInt i = 0; i < obj->polyonNumber; i++)
				{
					poly = &obj->polyonList[i];
					Remove_Backface(poly, camera);
				}
			}
			obj = obj->nextObject;
		}
	}

	void Remove_Backface_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end();)
		{
			poly = &(*iter);

			//移除不可见的多变形
			//在此操作以后在 RenderList之后的操作中 就不需要 判断多变形的可见性
			if (Remove_Backface(poly, camera)) 
			{
				renderList->polyData.erase(iter++);
			}
			else
			{
				++iter;
			}
		}
	}

	void Light_PolyonF4D(EPolyonF4D *poly, EFrustum *camera)
	{
		EBool lightOn = false;
		EColor baseColor(poly->color);

		//全部颜色总和
		EInt sumColorR = 0, sumColorG = 0, sumColorB = 0;
		
		//顶点按顺时针方向排序
		EVector4D u = (poly->transformList[0] - poly->transformList[1]).toVector4D();
		EVector4D v = (poly->transformList[0] - poly->transformList[2]).toVector4D();
		EVector4D n = u.cross(v);
		//法线的长度
		EFloat length = n.length();

		ELight *light = NULL;
		for (LightIter iter = GLights->begin(); iter != GLights->end(); iter++)
		{
			light = *iter;

			//灯光关闭
			if (!light->lightOn)
			{
				continue;
			}
			lightOn = true;

			//环境光
			if (light->lightType == LIGHT_AMBIENT)
			{
				sumColorR += light->ambient.r * baseColor.r >> 8;
				sumColorG += light->ambient.g * baseColor.g >> 8;
				sumColorB += light->ambient.b * baseColor.b >> 8;
			}
			//平行光
			else if (light->lightType == LIGHT_DIRECTION)
			{
				EFloat dot = n.dot(-light->transDirection);

				if (dot > 0.0f)
				{
					EFloat dirLeng = light->transDirection.length();

					//接收光照的强度，多变形法线与光照方向的夹角越小，接受的强度越大，反之强度越小，越暗
					//其实除以 256 没啥用，light->power 是  0 - 1，
					//dot / length*dirLeng 是 法线 和 光方向的 夹角的cos值，取值范围[-1， 1]，且 只处理cos值大于0，所以temp的取值只能是 0-1
					EFloat temp = light->power * dot / (length*dirLeng * 256);
					sumColorR += temp * light->diffuse.r * baseColor.r;
					sumColorG += temp * light->diffuse.g * baseColor.g;
					sumColorB += temp * light->diffuse.b * baseColor.b;
				}
				else
				{
					//该图元背向光源，原则上不处理，但是为了让颜色协调
					sumColorR += baseColor.r * light->shadowPower;
					sumColorG += baseColor.g * light->shadowPower;
					sumColorB += baseColor.b * light->shadowPower;
				}
			}
			//点光源
			else if (light->lightType == LIGHT_POINT)
			{
				EVector4D dir = light->transDirection - poly->transformList[0].toVector4D();
				EFloat dot = n.dot(dir);
				if (dot > 0.0f)
				{
					EFloat dist = dir.length();
					EFloat atten = light->kc + light->kl * dist + light->kq * dist * dist;
					EFloat temp = dot / (length * dist * atten * 256);

					sumColorR += light->diffuse.r * baseColor.r * temp;
					sumColorG += light->diffuse.g * baseColor.g * temp;
					sumColorB += light->diffuse.b * baseColor.b * temp;
				}
				else
				{
					sumColorR += baseColor.r * light->shadowPower;
					sumColorG += baseColor.g * light->shadowPower;
					sumColorB += baseColor.b * light->shadowPower;
				}
			}
		}
		if (lightOn)
		{
			if (sumColorR > 255) sumColorR = 255;
			if (sumColorG > 255) sumColorG = 255;
			if (sumColorB > 255) sumColorB = 255;

			poly->color = ECOLOR_16BIT(sumColorR, sumColorG, sumColorB);
		}
	}

	void Light_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
		{
			poly = &(*iter);
			Light_PolyonF4D(poly, camera);
		}
	}

	void Camera_To_Perspective_Object4D(EObject4D *object, EFrustum *camera)
	{
		if (!object)
			return;

		EObject4D *obj = object;
		EFloat d = camera->view_dist;
		EFloat temp = 0;

		while (obj)
		{
			for (EInt i = 0; i < obj->vertexNumber; i++)
			{
				temp = d / obj->transformList[i].z;
				//变换x，y。z不变
				obj->transformList[i].x = temp * obj->transformList[i].x;
				obj->transformList[i].y = camera->aspect_ratio * temp * obj->transformList[i].y;
			}
			obj = obj->nextObject;
		}
	}

	void Camera_To_Perspective_RenderList(ERenderList4D *renderList, EFrustum *camera)
	{
		EFloat d = camera->view_dist;
		EFloat temp = 0;
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
		{
			poly = &(*iter);
			for (EInt i = 0; i < 3; i++)
			{
				temp = d / poly->transformList[i].z;
				poly->transformList[i].x = temp * poly->transformList[i].x;
				poly->transformList[i].y = camera->aspect_ratio * temp * poly->transformList[i].y;
			}
		}
	}

	void Perspective_To_Screen_Object4D(EObject4D *object, EFrustum *camera)
	{
		if (!object)
			return;

		EFloat XOffset = 0.5f * (camera->viewport_width - 1);
		EFloat YOffset = 0.5f * (camera->viewport_height - 1);

		EObject4D *obj = object;
		while (obj)
		{
			for (EInt i = 0; i < obj->vertexNumber; i++)
			{
				obj->transformList[i].x = XOffset + XOffset * obj->transformList[i].x;
				obj->transformList[i].y = YOffset - YOffset * obj->transformList[i].y;
			}
			obj = obj->nextObject;
		}
	}
	void Perspective_To_Screen_RenderList(ERenderList4D *renderList, EFrustum *camera)
	{
		EFloat XOffset = 0.5f * (camera->viewport_width - 1);
		EFloat YOffset = 0.5f * (camera->viewport_height - 1);

		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
		{
			for (EInt i = 0; i < 3; i++)
			{
				iter->transformList[i].x = XOffset + XOffset * iter->transformList[i].x;
				iter->transformList[i].y = YOffset - YOffset * iter->transformList[i].y;
			}
		}
	}

	//判断多边形是否在摄像机范围内
	EBool isClipped(EPolyonF4D *poly, const EPlane3D &plane)
	{
		for (EInt i = 0; i < 3; i++)
		{
			EFloat temp = plane.normal.dot(poly->transformList[i].toVector4D() - plane.point);
			if (temp >= 0)
			{
				return false;
			}
		}

		return true;
	}

	EBool isClipped(EPolyonF4D *poly, EFloat znear, EFloat zfar)
	{
		for (EInt i = 0; i < 3; i++)
		{
			if (poly->transformList[i].z <= zfar && poly->transformList[i].z >= znear)
			{
				return false;
			}
		}
		return true;
	}

	EBool isClipped(EPolyonF4D *poly, EFrustum *camera)
	{
		EVertex4D v0, v1, v2;
		GetVertex4DMulMatrix44(poly->transformList[0], camera->mWorldToCamera, v0);
		GetVertex4DMulMatrix44(poly->transformList[1], camera->mWorldToCamera, v1);
		GetVertex4DMulMatrix44(poly->transformList[2], camera->mWorldToCamera, v2);

		if ( (camera->clip_z_near > v0.z && camera->clip_z_near > v1.z && camera->clip_z_near > v2.z) ||
			 (camera->clip_z_far < v0.z && camera->clip_z_far < v1.z && camera->clip_z_far < v2.z) ||
			 (camera->clip_plane_L.normal.dot(v0.toVector4D() - camera->clip_plane_L.point) < 0 && camera->clip_plane_L.normal.dot(v1.toVector4D() - camera->clip_plane_L.point) < 0 && camera->clip_plane_L.normal.dot(v2.toVector4D() - camera->clip_plane_L.point) < 0) ||
			 (camera->clip_plane_R.normal.dot(v0.toVector4D() - camera->clip_plane_R.point) < 0 && camera->clip_plane_R.normal.dot(v1.toVector4D() - camera->clip_plane_R.point) < 0 && camera->clip_plane_R.normal.dot(v2.toVector4D() - camera->clip_plane_R.point) < 0) || 
			 (camera->clip_plane_T.normal.dot(v0.toVector4D() - camera->clip_plane_T.point) < 0 && camera->clip_plane_T.normal.dot(v1.toVector4D() - camera->clip_plane_T.point) < 0 && camera->clip_plane_T.normal.dot(v2.toVector4D() - camera->clip_plane_T.point) < 0) ||
			 (camera->clip_plane_B.normal.dot(v0.toVector4D() - camera->clip_plane_B.point) < 0 && camera->clip_plane_B.normal.dot(v1.toVector4D() - camera->clip_plane_B.point) < 0 && camera->clip_plane_B.normal.dot(v2.toVector4D() - camera->clip_plane_B.point) < 0) )
		{
			return true;
		}

		return false;
	}

	void Clip_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end();)
		{
			poly = &(*iter);
			if (isClipped(poly, camera->clip_plane_L) || isClipped(poly, camera->clip_plane_R) || isClipped(poly, camera->clip_plane_B) || isClipped(poly, camera->clip_plane_T))
			{
				renderList->polyData.erase(iter++);
				continue;
			}
			++iter;
		}
	}

	void World_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera)
	{
		EFloat XOffset = 0.5f * (camera->viewport_width - 1);
		EFloat YOffset = 0.5f * (camera->viewport_height - 1);
		EFloat dist = camera->view_dist;
		EFloat temp = 0;

		//灯光位置变换，转到相机坐标系下面
		//只保留旋转变换，不平移，否则会出现灯光跟随相机移动的效果  也就是 灯光的坐标一直是世界坐标
		EMatrix44 mat = camera->mWorldToCamera;
		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.0f;
		mat.m[3][3] = 1.0f;

		ELight *light = NULL;
		for (LightIter  iter = GLights->begin(); iter != GLights->end() ; iter++)
		{
			light = *iter;
			GetVector4DMulMatrix44(light->position, mat, light->transPostion);
			GetVector4DMulMatrix44(light->direction, mat, light->transDirection);
		}

		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end();)
		{
			poly = &(*iter);

			//首先背面清楚
			if (Remove_Backface(poly, camera))
			{
				renderList->polyData.erase(iter++);
				continue;
			}
			//世界坐标变相机坐标
			for (EInt i = 0; i < 3; i++)
			{
				GetVertex4DMulMatrix44(poly->transformList[i], camera->mWorldToCamera, poly->transformList[i]);
			}
			//进行裁剪
			if (isClipped(poly, camera->clip_z_near, camera->clip_z_far) ||
				isClipped(poly, camera->clip_plane_L) ||
				isClipped(poly, camera->clip_plane_R) ||
				isClipped(poly, camera->clip_plane_B) ||
				isClipped(poly, camera->clip_plane_T))
			{
				renderList->polyData.erase(iter++);
				continue;
			}
			//如果未被裁剪 且不是背面 则进行光照计算
			Light_PolyonF4D(poly, camera);
			//透视变换 屏幕变换
			for (EInt i = 0; i < 3; i++)
			{
				//透视变换
				temp = dist / poly->transformList[i].z;
				poly->transformList[i].x = temp * poly->transformList[i].x;
				poly->transformList[i].y = camera->aspect_ratio * temp * poly->transformList[i].y;
				//屏幕变换
				poly->transformList[i].x = XOffset + XOffset * poly->transformList[i].x;
				//为什么是减呢  推测：逻辑坐标y 上为正   视口坐标 下为正
				poly->transformList[i].y = YOffset - YOffset * poly->transformList[i].y;
			}
			++iter;
		}
		//在相机坐标系中对渲染列表进行深度排序
		//这里基于一个事实，即虽然x,y坐标发生了变换，但是z坐标依旧是之前摄像机坐标系下的z值
		Sort_RenderList4D(renderList);
	}
	//渲染线框模型
	void Draw_Object4D_Wire(EObject4D *object)
	{
		if (!object)
		{
			return;
		}

		EObject4D *obj = object;
		while (obj)
		{
			for (EInt i = 0; i < obj->polyonNumber; i++)
			{
				//激活 且没有别剔除 或 裁剪
				if (obj->polyonList[i].state & EPOLY_STATE_ACTIVE && !(obj->polyonList[i].state & EPOLY_STATE_BACKFACE) && !(obj->polyonList[i].state & EPOLY_STATE_CLIPPED))
				{
					for (EInt j = 0; j < 3; j++)
					{
						EInt nextVerIndex = (j + 1) % 3;
						EVertex4D curVer = obj->transformList[obj->polyonList[i].verIndex[j]];
						EVertex4D nextVer = obj->transformList[obj->polyonList[i].verIndex[nextVerIndex]];
						EGraphics::drawLine(curVer.x, curVer.y, nextVer.x, nextVer.y, EColor(obj->polyonList[i].color));
					}
				}
				//绘制完毕，恢复多边形，清除背面标记 (讲真不懂为什么要这么做)
				obj->polyonList[i].state = EPOLY_STATE_ACTIVE;
			}
			obj->state = EOBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}
	void Draw_RenderList4D_Wire(ERenderList4D *renderList)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
		{
			poly = &(*iter);
			for (EInt i = 0; i < 3; i++)
			{
				EInt nextVerIndex = (i + 1) % 3;
				EVertex4D curVer = poly->transformList[i];
				EVertex4D nextVer = poly->transformList[nextVerIndex];
				EGraphics::drawLine(curVer.x, curVer.y, nextVer.x, nextVer.y, poly->color);
			}
			//绘制完毕
			poly->state = EPOLY_STATE_ACTIVE;
		}
		//哦？然后就清理掉了？？？
		renderList->polyData.clear();
	}

	void Draw_Object4D_Solid(EObject4D *object)
	{
		if (!object)
		{
			return;
		}

		EObject4D *obj = object;
		EPolyon4D *poly = NULL;
		while (obj)
		{
			for (EInt i = 0; i < obj->polyonNumber; i++)
			{
				//激活 且没有别剔除 或 裁剪
				if (obj->polyonList[i].state & EPOLY_STATE_ACTIVE && !(obj->polyonList[i].state & EPOLY_STATE_BACKFACE) && !(obj->polyonList[i].state & EPOLY_STATE_CLIPPED))
				{
					poly = &obj->polyonList[i];
					EVertex4D ver0 = obj->transformList[poly->verIndex[0]];
					EVertex4D ver1 = obj->transformList[poly->verIndex[1]];
					EVertex4D ver2 = obj->transformList[poly->verIndex[2]];

					EGraphics::fillTriangle(ver0.x, ver0.y, ver1.x, ver1.y, ver2.x, ver2.y, EColor(poly->color));
				}
				//绘制完毕，恢复多边形，清除背面标记 (讲真不懂为什么要这么做)
				obj->polyonList[i].state = EPOLY_STATE_ACTIVE;
			}
			obj->state = EOBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}

	void Draw_RenderList4D_Solid(ERenderList4D *renderList)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
		{
			poly = &(*iter);
			
			EGraphics::fillTriangle(poly->transformList[0].x, poly->transformList[0].y,
				poly->transformList[1].x, poly->transformList[1].y,
				poly->transformList[2].x, poly->transformList[2].y, EColor(poly->color));

			//绘制完毕
			poly->state = EPOLY_STATE_ACTIVE;
		}
		//哦？然后就清理掉了？？？
		renderList->polyData.clear();
	}

	void Draw_Top_Trangle(EFloat x0, EFloat y0, EFloat z0,
		EFloat u0, EFloat v0,
		EFloat x1, EFloat y1, EFloat z1,
		EFloat u1, EFloat v1,
		EFloat x2, EFloat y2, EFloat z2,
		EFloat u2, EFloat v2,
		EMaterial *mat, const EColor &color)
	{
		//该三角形已经超出了屏幕
		if (y2 < 0 || y0 > SCREEN_HEIGHT - 1)
		{
			return;
		}

		EFloat dxdl = (x0 - x1) / (y0 - y1);
		EFloat dxdr = (x0 - x2) / (y0 - y2);

		EFloat dzdl = (z0 - z1) / (y0 - y1);
		EFloat dzdr = (z0 - z2) / (y0 - y2);

		EFloat startL = x0;
		EFloat startR = x0;

		EFloat begX = 0;
		EFloat endX = 0;

		//没有纹理 或者 没有纹理贴图 则使用 color着色
		if (!mat || !mat->useTexture())
		{
			if (y0 < 0)
			{
				y0 = 0;
			}

			if (y2 > SCREEN_HEIGHT - 1)
			{
				y2 = SCREEN_HEIGHT - 1;
			}

			for (EFloat y = y0; y < y2; y++)
			{
				begX = startL;
				endX = startR;
				if (endX < begX) std::swap(begX, endX);

				if (begX < 0)
				{
					begX = 0;
				}

				if (endX > SCREEN_WIDTH - 1)
				{
					endX = SCREEN_WIDTH - 1;
				}

				for (EFloat x = begX; x < endX; x++)
				{
					//TODO 难道不是求出对应点的z么？？？为什么都使用z0
					if (EGraphics::checkZ(x, y, z0))
					{
						EGraphics::setPixel(x, y, color);
					}
				}
				//其实是 startL = startL + 1 * dxdl 仔细品品！
				startL += dxdl;
				startR += dxdr;
			}
			return;
		}

		EFloat dudl = (u0 - u1) / (y0 - y1);
		EFloat dvdl = (v0 - v1) / (y0 - y1);
		EFloat dudr = (u0 - u2) / (y0 - y2);
		EFloat dvdr = (v0 - v2) / (y0 - y2);

		EFloat startLU = u0;
		EFloat startLV = v0;
		EFloat startRU = u0;
		EFloat startRV = v0;

		EFloat begU = 0;
		EFloat endU = 0;
		EFloat begV = 0;
		EFloat endV = 0;

		EFloat dx = 0;
		EFloat ui = 0;
		EFloat vi = 0;

		EFloat zl = 0;
		EFloat zr = 0;
		EFloat z = 0;

		for (EFloat y = 0; y < y2; y++)
		{
			begX = (EInt)startL;
			endX = (EInt)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;
			endV = startRV;

			zl = z0 + (y - y0) * dzdl;
			zr = z0 + (y - y0) * dzdr;

			if (endX < begX)
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);
				std::swap(zl, zr);
			}

			//修正X的范围，并修正贴图的U坐标
			if (begX < 0)
			{
				//修正U
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}

			if (endX > SCREEN_WIDTH - 1)
			{
				endU = endU - (endX - SCREEN_WIDTH) * (startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;

			z = zl;
			EFloat zd = (zl - zr) / (begX - endX);
			for (EFloat x = begX; x <= endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT && EGraphics::checkZ(x, y, z))
				{
					const EColor tex = mat->getPixel(begU, begV);
					EGraphics::setPixel(x, y, color * tex);
				}

				begU += ui;
				begV += vi;
				z += zd;
			}

			startL += dxdl;
			startR += dxdr;

			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	void Draw_Bottom_Trangle(EFloat x0, EFloat y0, EFloat z0,
		EFloat u0, EFloat v0,
		EFloat x1, EFloat y1, EFloat z1,
		EFloat u1, EFloat v1,
		EFloat x2, EFloat y2, EFloat z2,
		EFloat u2, EFloat v2,
		EMaterial *mat, const EColor &color)
	{
		if (y2 <= 0 || SCREEN_HEIGHT - 1)
		{
			return;
		}

		EFloat dxdl = (x1 - x2) / (y1 - y2);
		EFloat dxdr = (x0 - x2) / (y0 - y2);

		EFloat dzdl = (z1 - z2) / (y1 - y2);
		EFloat dzdr = (z0 - z2) / (y0 - y2);

		EFloat startL = x1;
		EFloat startR = x0;

		EFloat begX = 0;
		EFloat endX = 0;

		if (!mat || !mat->useTexture())
		{
			if (y0 < 0)
			{
				y0 = 0;
			}

			if (y2 > SCREEN_HEIGHT - 1)
			{
				y2 = SCREEN_HEIGHT - 1;
			}
			//没有纹理或者没有贴图，就是用color
			for (EFloat y = y0; y < y2; y++)
			{
				begX = startL;
				endX = startR;

				if (begX > endX) std::swap(begX, endX);

				if (begX < 0)
				{
					begX = 0;
				}

				if (endX > SCREEN_WIDTH - 1)
				{
					endX = SCREEN_WIDTH - 1;
				}

				for (EFloat x = begX; x < endX; x++)
				{
					if (EGraphics::checkZ(x, y, z0))
					{
						EGraphics::setPixel(x, y, color);
					}
				}

				startL += dxdl;
				startR += dxdr;
			}
			return;
		}
		EFloat dudl = (u1 - u2) / (y1 - y2);
		EFloat dvdl = (v1 - v2) / (y1 - y2);
		EFloat dudr = (u0 - u2) / (y0 - y2);
		EFloat dvdr = (v0 - v2) / (y0 - y2);

		EFloat startLU = u1;
		EFloat startLV = v1;
		EFloat startRU = u0;
		EFloat startRV = v0;

		EFloat begU = 0;
		EFloat endU = 0;
		EFloat begV = 0;
		EFloat endV = 0;

		EFloat dx = 0;
		EFloat ui = 0;
		EFloat vi = 0;

		EFloat zl = 0;
		EFloat zr = 0;
		EFloat z = 0;
		for (EFloat y = y0; y < y2; y++)
		{
			begX = (EInt)startL;
			endX = (EInt)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;

			zl = z1 + (y - y0) * dzdl;
			zr = z0 + (y - y0) * dzdr;

			if (endX < begX)
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);
				std::swap(zl, zr);
			}
			//修正X的范围，并修正贴图的U的坐标
			if (begX < 0)
			{
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}

			if (endX > SCREEN_WIDTH - 1)
			{
				//修正U(为什么不减1)
				endU = endU - (endX - SCREEN_WIDTH) * (startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;
			z = zl;
			EFloat zd = (zl - zr) / (begX - endX);
			for (EFloat x = begX; x < endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT && EGraphics::checkZ(x, y, z))
				{
					const EColor tex = mat->getPixel(begU, begV);
					EGraphics::setPixel(x, y, color * tex);
				}
				begU += ui;
				begV += vi;
				z += zd;
			}

			startL += dxdl;
			startR += dxdr;
			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	void Draw_Trangle(EPolyonF4D *poly)
	{
		EFloat x0 = poly->transformList[0].x, y0 = poly->transformList[0].y, z0 = poly->transformList[0].z;
		EFloat u0 = poly->transformList[0].u, v0 = poly->transformList[0].v;

		EFloat x1 = poly->transformList[1].x, y1 = poly->transformList[1].y, z1 = poly->transformList[1].z;
		EFloat u1 = poly->transformList[1].u, v1 = poly->transformList[1].v;

		EFloat x2 = poly->transformList[2].x, y2 = poly->transformList[2].y, z2 = poly->transformList[2].z;
		EFloat u2 = poly->transformList[2].u, v2 = poly->transformList[2].v;

		//保证y0 < y1 < y2

		if (y1 < y0)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(z0, z1);
			std::swap(u0, u1);
			std::swap(v0, v1);
		}

		if (y2 < y0)
		{
			std::swap(x0, x2);
			std::swap(y0, y2);
			std::swap(z0, z2);
			std::swap(u0, u2);
			std::swap(v0, v2);
		}

		if (y2 < y1)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
			std::swap(z1, z2);
			std::swap(u1, u2);
			std::swap(v1, v2);
		}

		EFloat factor = (y1 - y0) / (y2 - y0);
		EFloat newX = x0 + factor * (x2 - x0);
		EFloat newZ = z0 + factor * (z2 - z0);
		EFloat newU = u0 + factor * (u2 - u0);
		EFloat newV = v0 + factor * (u2 - v0);

		Draw_Top_Trangle(x0, y0, z0, u0, v0, x1, y1, z1, u1, v1, newX, y1, newZ, newU, newV, poly->material, EColor(poly->color));
		Draw_Bottom_Trangle(newX, y1, newZ, newU, newV, x1, y1, z1, u1, v1, x2, y2, z2, u2, v2, poly->material, EColor(poly->color));
	}
	void Draw_RenderList4D_Texture_Solid(ERenderList4D *renderList)
	{
		EPolyonF4D *poly = NULL;
		for (ERenderList4D::Itr iter = renderList->polyData.begin(); iter != renderList->polyData.end(); iter++)
		{
			poly = &(*iter);
			Draw_Trangle(poly);
			poly->state = EPOLY_STATE_ACTIVE;
		}
		renderList->polyData.clear();
	}
}