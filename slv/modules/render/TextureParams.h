#pragma once
#include <d3d9.h>
#include <d3dx9.h>

namespace slv
{
	namespace modules
	{
		namespace render
		{
			struct TextureParams
			{
#pragma region wrappers
				enum class FilterMode
				{
					Point = D3DTEXF_POINT,
					Linear = D3DTEXF_LINEAR,
					Anisotropic = D3DTEXF_ANISOTROPIC
				};
				enum class AddressMode
				{
					Wrap = D3DTADDRESS_WRAP,
					Border = D3DTADDRESS_BORDER,
					Clamp = D3DTADDRESS_CLAMP,
					Mirror = D3DTADDRESS_MIRROR
				};
#pragma endregion wrappers

				FilterMode minFilter, magFilter;
				AddressMode addrU, addrV;

				TextureParams();

				void Apply(IDirect3DDevice9 &device, DWORD sampler = 0);
			};
		}
	}
}