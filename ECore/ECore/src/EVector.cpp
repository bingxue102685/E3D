#include "EVector.h"


namespace E3D
{
	EVector2D EVector2D::ZERO = EVector2D(0, 0);
	EVector2D EVector2D::UNIT_X = EVector2D(1.0f, 0);
	EVector2D EVector2D::UNIT_Y = EVector2D(0, 1.0f);

	EVector3D EVector3D::ZERO = EVector3D(0, 0, 0);
	EVector3D EVector3D::UNIT_X = EVector3D(1.0f, 0, 0);
	EVector3D EVector3D::UNIT_Y = EVector3D(0, 1.0f, 0);
	EVector3D EVector3D::UNIT_Z = EVector3D(0, 0, 1.0f);

	EVector4D EVector4D::ZERO = EVector4D(0, 0, 0);
	EVector4D EVector4D::UNIT_X = EVector4D(1.0f, 0, 0);
	EVector4D EVector4D::UNIT_Y = EVector4D(0, 1.0f, 0);
	EVector4D EVector4D::UNIT_Z = EVector4D(0, 0, 1.0f);
}
