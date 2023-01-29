#include "DxMathEx.h"

D3DXVECTOR3 operator*(const D3DXQUATERNION &q, const D3DXVECTOR3 &v)
{
	D3DXQUATERNION vQuat(v.x, v.y, v.z, 0.f);
	vQuat = vQuat * q;
	return { vQuat.x, vQuat.y, vQuat.z };
}

D3DXVECTOR3 operator*(const D3DXVECTOR3 &v, const D3DXQUATERNION &q)
{
	D3DXQUATERNION vQuat(v.x, v.y, v.z, 0.f);
	vQuat = vQuat * q;
	return { vQuat.x, vQuat.y, vQuat.z };
}