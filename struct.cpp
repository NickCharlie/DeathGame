#include "struct.h"

FMatrix FTransform::ToMatrixWithScale() {

	FMatrix m;
	if (IsBadReadPtr((void*)this, 8)) return {};
	if (!this) return {};
	m._41 = Translation.x;
	m._42 = Translation.y;
	m._43 = Translation.z;

	float x2 = Rotation.x + Rotation.x;
	float y2 = Rotation.y + Rotation.y;
	float z2 = Rotation.z + Rotation.z;

	float xx2 = Rotation.x * x2;
	float yy2 = Rotation.y * y2;
	float zz2 = Rotation.z * z2;
	m._11 = (1.0f - (yy2 + zz2)) * Scale3D.x;
	m._22 = (1.0f - (xx2 + zz2)) * Scale3D.y;
	m._33 = (1.0f - (xx2 + yy2)) * Scale3D.z;


	float yz2 = Rotation.y * z2;
	float wx2 = Rotation.w * x2;
	m._32 = (yz2 - wx2) * Scale3D.z;
	m._23 = (yz2 + wx2) * Scale3D.y;


	float xy2 = Rotation.x * y2;
	float wz2 = Rotation.w * z2;
	m._21 = (xy2 - wz2) * Scale3D.y;
	m._12 = (xy2 + wz2) * Scale3D.x;


	float xz2 = Rotation.x * z2;
	float wy2 = Rotation.w * y2;
	m._31 = (xz2 + wy2) * Scale3D.z;
	m._13 = (xz2 - wy2) * Scale3D.x;

	m._14 = 0.0f;
	m._24 = 0.0f;
	m._34 = 0.0f;
	m._44 = 1.0f;

	return m;
}

FMatrix FMatrix::operator*(const FMatrix& other) {

	FMatrix NewMatrix;
	NewMatrix._11 = this->_11 * other._11 + this->_12 * other._21 + this->_13 * other._31 + this->_14 * other._41;
	NewMatrix._12 = this->_11 * other._12 + this->_12 * other._22 + this->_13 * other._32 + this->_14 * other._42;
	NewMatrix._13 = this->_11 * other._13 + this->_12 * other._23 + this->_13 * other._33 + this->_14 * other._43;
	NewMatrix._14 = this->_11 * other._14 + this->_12 * other._24 + this->_13 * other._34 + this->_14 * other._44;
	NewMatrix._21 = this->_21 * other._11 + this->_22 * other._21 + this->_23 * other._31 + this->_24 * other._41;
	NewMatrix._22 = this->_21 * other._12 + this->_22 * other._22 + this->_23 * other._32 + this->_24 * other._42;
	NewMatrix._23 = this->_21 * other._13 + this->_22 * other._23 + this->_23 * other._33 + this->_24 * other._43;
	NewMatrix._24 = this->_21 * other._14 + this->_22 * other._24 + this->_23 * other._34 + this->_24 * other._44;
	NewMatrix._31 = this->_31 * other._11 + this->_32 * other._21 + this->_33 * other._31 + this->_34 * other._41;
	NewMatrix._32 = this->_31 * other._12 + this->_32 * other._22 + this->_33 * other._32 + this->_34 * other._42;
	NewMatrix._33 = this->_31 * other._13 + this->_32 * other._23 + this->_33 * other._33 + this->_34 * other._43;
	NewMatrix._34 = this->_31 * other._14 + this->_32 * other._24 + this->_33 * other._34 + this->_34 * other._44;
	NewMatrix._41 = this->_41 * other._11 + this->_42 * other._21 + this->_43 * other._31 + this->_44 * other._41;
	NewMatrix._42 = this->_41 * other._12 + this->_42 * other._22 + this->_43 * other._32 + this->_44 * other._42;
	NewMatrix._43 = this->_41 * other._13 + this->_42 * other._23 + this->_43 * other._33 + this->_44 * other._43;
	NewMatrix._44 = this->_41 * other._14 + this->_42 * other._24 + this->_43 * other._34 + this->_44 * other._44;

	return NewMatrix;
}
