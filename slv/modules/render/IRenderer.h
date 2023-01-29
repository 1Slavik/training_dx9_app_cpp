#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include <slv_dx_common.h>
#include "../../framework/appl/DxApp.h"
#include "../../utils/debug/Debug.h"

namespace slv
{
	namespace modules
	{
		namespace render
		{
			class IRenderer
			{
#pragma region types aliases
			protected:
				using vec3 = D3DXVECTOR3;
				using mat4 = D3DXMATRIX;

				using App = slv::frwk::appl::Dx9App;
				using Debug = slv::utils::debug::Debug;
#pragma endregion types aliases
			
#pragma region wrappers
			public:
				enum class FillMode
				{
					Point = D3DFILL_POINT,
					Wireframe = D3DFILL_WIREFRAME,
					Solid = D3DFILL_SOLID
				};
#pragma endregion wrappers

			public:
				virtual ~IRenderer() = 0 {};

				virtual void Render(IDirect3DDevice9 &, UINT streamNumber) = 0;
			};
		}
	}
}