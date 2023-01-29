#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include "../../framework/appl/Screen.h"

namespace slv
{
	namespace modules
	{
		namespace render
		{
			class Camera
			{
#pragma region types aliases
				using vec3 = D3DXVECTOR3;
				using mat4 = D3DXMATRIX;
				using Screen = slv::frwk::appl::Screen;
#pragma endregion types aliases

#pragma region space
				vec3 m_forward;
				vec3 m_right;
				vec3 m_up;
#pragma endregion space

				vec3 m_pos;


			public:
				Camera() {};
				~Camera() {};
				
				void ViewMatrix(mat4 *pOut) const {};
				void ProjMatrix(mat4 *pOut) const {};
			};
		}
	}
}