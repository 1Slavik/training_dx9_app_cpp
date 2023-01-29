#pragma once
// #include <d3d9.h>
// #include <d3dx9.h>
#include "../../framework/appl/DxApp.h"
#include "../../utils/debug/StatusList.h"
#include "../../utils/debug/Debug.h"
#include "TextureParams.h"

namespace slv
{
	namespace modules
	{
		namespace render
		{
			class Texture2d
			{
#pragma region types aliases
			private:
				using Debug = slv::utils::debug::Debug;
				using DbgStatus = slv::utils::debug::Status;
				using DbgStatusList = slv::utils::debug::StatusList;
				using App = slv::frwk::appl::Dx9App;
			public:
				using FilterMode = TextureParams::FilterMode;
				using AddressMode = TextureParams::AddressMode;
#pragma endregion types aliases
			private:
#pragma region debug
				static DbgStatus s_statuses[];
				static DbgStatusList s_statusList;
#pragma endregion debug

				IDirect3DTexture9 *m_pTexData;
				TextureParams m_params;
			public:
				Texture2d();
				~Texture2d();

				void LoadFromFile(const wchar_t *file);
				void Use();
				inline void UseWithCurrSamplerParams(DWORD stage = 0) { App::Direct3dDevice()->SetTexture(stage, m_pTexData); }

				inline const IDirect3DTexture9* Data() const { return m_pTexData; }
				inline void MagnificationFilter(FilterMode filter) { m_params.magFilter = filter; }
				inline void MinificationFilter(FilterMode filter) { m_params.minFilter = filter; }
				inline void AddressModeU(AddressMode addr) { m_params.addrU = addr; }
				inline void AddressModeV(AddressMode addr) { m_params.addrV = addr; }
			};
		}
	}
}