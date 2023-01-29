#pragma once

#include <d3dx9math.h>

D3DXVECTOR3 operator*(const D3DXQUATERNION&, const D3DXVECTOR3&);
D3DXVECTOR3 operator*(const D3DXVECTOR3&, const D3DXQUATERNION&);