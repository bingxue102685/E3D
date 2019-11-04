#pragma once

#include "ECommon.h"
#include "EUtil.h"

namespace E3D
{
	//2D����
	struct EVector2D
	{
		static EVector2D ZERO;
		static EVector2D UNIT_X;
		static EVector2D UNIT_Y;

		EFloat x;
		EFloat y;

		EVector2D() :x(0), y(0) {}
		EVector2D(EFloat x, EFloat y) : x(x), y(y) {}
		EVector2D(const EVector2D &v) : x(v.x), y(v.y) {}

		inline EFloat length()const
		{
			return sqrt(x * x + y * y);
		}

		//��һ��
		inline EFloat normalize()
		{
			EFloat len = length();
			if (len > EPSILON_E6)
			{
				EFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
			}
			return len;
		}

		inline EFloat dot(const EVector2D &right) const
		{
			return x * right.x + y * right.y;
		}

		inline EFloat angleBetween(const EVector2D &right)const
		{
			EFloat lenCross = length() * right.length();
			if (lenCross < EPSILON_E6)
			{
				lenCross = EPSILON_E6;
			}

			EFloat f = dot(right) / lenCross;
			f = Clamp(f, -1.0f, 1.0f);

			return acos(f);
		}

		inline bool isZero() const
		{
			return length() < EPSILON_E6;
		}

		inline EFloat squareLength() const
		{
			return x * x + y * y;
		}

		//��������
		inline EVector2D operator* (EFloat right) const
		{
			return EVector2D(x * right, y * right);
		}
		//�������
		inline EVector2D operator* (const EVector2D &right)const
		{
			return EVector2D(x * right.x, y * right.y);
		}
		//�����ӷ�
		inline EVector2D operator+ (const EVector2D &right) const
		{
			return EVector2D(x + right.x, y + right.y);
		}

		//��������
		inline EVector2D operator- (const EVector2D &right) const
		{
			return EVector2D(x - right.x, y - right.y);
		}
		//����ȡ��
		inline EVector2D operator- ()
		{
			x = -x;
			y = -y;
			return *this;
		}

		//�����Ƿ����
		inline bool operator== (const EVector2D &right) const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y);
		}

		//�����ĸ�ֵ
		inline EVector2D operator= (const EVector2D &right)
		{
			x = right.x;
			y = right.y;
			return *this;
		}
	};

	//3D����
	struct EVector3D
	{
		static EVector3D ZERO;
		static EVector3D UNIT_X;
		static EVector3D UNIT_Y;
		static EVector3D UNIT_Z;

		EFloat x;
		EFloat y;
		EFloat z;
		//���캯��
		EVector3D() :x(0), y(0), z(0) {}
		EVector3D(EFloat x, EFloat y, EFloat z) : x(x), y(y), z(z) {}
		EVector3D(const EVector3D &v) : x(v.x), y(v.x), z(v.z) {}

		//���
		inline EFloat dot(const EVector3D &right) const
		{
			return x * right.x + y * right.y + z * right.z;
		}
		//�������
		inline EVector3D cross(const EVector3D &right) const
		{
			return EVector3D(y*right.z - z * right.y, z*right.x - x * right.z, x*right.y - y * right.x);
		}

		inline EFloat length() const
		{
			return (EFloat)sqrt(x * x + y * y + z * z);
		}

		//��һ��
		inline EVector3D normalize()
		{
			EFloat len = length();
			if (len > EPSILON_E6)
			{
				EFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
				z *= invLength;
			}
			return *this;
		}
		//0ֵ�ж�
		inline bool isZero() const
		{
			return length() < EPSILON_E6;
		}
		//�н�(����)
		inline EFloat angleBetween(const EVector3D &right) const
		{
			EFloat lenCross = length() * right.length();
			if (lenCross < EPSILON_E6)
			{
				lenCross = EPSILON_E6;
			}

			EFloat cosValue = dot(right) / lenCross;
			cosValue = Clamp(cosValue, -1.0f, 1.0f);
			return acos(cosValue);
		}
		//���������ƽ��
		inline EFloat squareLength() const
		{
			return x * x + y * y + z * z;
		}
		//��������
		inline EVector3D operator*(EFloat right) const
		{
			return EVector3D(x * right, y * right, z*right);
		}
		//�������������
		inline EVector3D operator*(const EVector3D &right) const
		{
			return EVector3D(x * right.x, y * right.y, z * right.z);
		}
		//�����ӷ�
		inline EVector3D operator+(const EVector3D &right) const
		{
			return EVector3D(x + right.x, y + right.y, z + right.z);
		}
		//��������
		inline EVector3D operator-(const EVector3D &right) const
		{
			return EVector3D(x - right.x, y - right.y, z - right.z);
		}
		//����ȡ��
		inline EVector3D operator-()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}
		//����ж�
		inline bool operator == (const EVector3D &right) const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y) && EqualFloat(z, right.z);
		}
		//������ֵ
		inline EVector3D operator = (const EVector3D &right)
		{
			x = right.x;
			y = right.y;
			z = right.z;
			return *this;
		}
	};

	//4D����
	struct EVector4D
	{
		static EVector4D ZERO;
		static EVector4D UNIT_X;
		static EVector4D UNIT_Y;
		static EVector4D UNIT_Z;

		EFloat x;
		EFloat y;
		EFloat z;
		EFloat w;
		//���캯��
		EVector4D() :x(0), y(0), z(0), w(0) {}
		EVector4D(EFloat x, EFloat y, EFloat z, EFloat w = 1) : x(x), y(y), z(z), w(w) {}
		EVector4D(const EVector4D &v) : x(v.x), y(v.x), z(v.z), w(v.w) {}

		//���
		inline EFloat dot(const EVector4D &right) const
		{
			return x * right.x + y * right.y + z * right.z;
		}
		//�������
		inline EVector4D cross(const EVector4D &right) const
		{
			return EVector4D(y*right.z - z * right.y, z*right.x - x * right.z, x*right.y - y * right.x, 1);
		}

		inline EFloat length() const
		{
			return (EFloat)sqrt(x * x + y * y + z * z);
		}

		//��һ��
		inline EVector4D normalize()
		{
			EFloat len = length();
			if (len > EPSILON_E6)
			{
				EFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
				z *= invLength;
				w = 1;
			}
			return *this;
		}
		//0ֵ�ж�
		inline bool isZero() const
		{
			return length() < EPSILON_E6;
		}
		//�н�(����)
		inline EFloat angleBetween(const EVector4D &right) const
		{
			EFloat lenCross = length() * right.length();
			if (lenCross < EPSILON_E6)
			{
				lenCross = EPSILON_E6;
			}

			EFloat cosValue = dot(right) / lenCross;
			cosValue = Clamp(cosValue, -1.0f, 1.0f);

			return acos(cosValue);
		}
		//���������ƽ��
		inline EFloat squareLength() const
		{
			return x * x + y * y + z * z;
		}
		//��������
		inline EVector4D operator*(EFloat right) const
		{
			return EVector4D(x * right, y * right, z*right, 1);
		}
		//�������������
		inline EVector4D operator*(const EVector4D &right) const
		{
			return EVector4D(x * right.x, y * right.y, z * right.z, 1);
		}
		//�����ӷ�
		inline EVector4D operator+(const EVector4D &right) const
		{
			return EVector4D(x + right.x, y + right.y, z + right.z, 1);
		}
		//��������
		inline EVector4D operator-(const EVector4D &right) const
		{
			return EVector4D(x - right.x, y - right.y, z - right.z, 1);
		}
		//����ȡ��
		inline EVector4D operator-()
		{
			x = -x;
			y = -y;
			z = -z;
			return *this;
		}
		//����ж�
		inline bool operator == (const EVector4D &right) const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y) && EqualFloat(z, right.z) && EqualFloat(w, right.w);
		}
		//������ֵ
		inline EVector4D operator = (const EVector4D &right)
		{
			x = right.x;
			y = right.y;
			z = right.z;
			w = right.w;

			return *this;
		}
	};
}