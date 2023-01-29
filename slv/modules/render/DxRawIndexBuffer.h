#pragma once

#include <d3d9.h>

#include <slv_dx_common.h>
#include "DxBufferParams.h"

#if SLV_DEBUG_LOGGING
#include "../../utils/debug/Padding.h"
#endif


namespace slv
{
	namespace modules
	{
		namespace render
		{
			class DxRawIndexBuffer
			{
				IDirect3DIndexBuffer9 *m_pIB;
			public:
				DxRawIndexBuffer(IDirect3DDevice9&, UINT capacity, D3DFORMAT indexFormat, const DxBufParams&);
				DxRawIndexBuffer(const DxRawIndexBuffer&) = delete; // temporary delete
				DxRawIndexBuffer(DxRawIndexBuffer&&) = delete; // temporary delete
				~DxRawIndexBuffer();

				DxRawIndexBuffer& operator=(const DxRawIndexBuffer&) = delete; // temporary delete
				DxRawIndexBuffer& operator=(DxRawIndexBuffer&&) = delete; // temporary delete

				inline IDirect3DIndexBuffer9* Buf() const { return m_pIB; }
				inline operator bool() const { return m_pIB != nullptr; }


#pragma region debug
#if SLV_DEBUG_LOGGING
				using Padding = slv::utils::debug::Padding;
				template <class ostream>
				int Serialize(ostream&, const std::string &objName = "", const Padding & padding = Padding(0)) const;
#endif
#pragma endregion debug
			};

#if SLV_DEBUG_LOGGING

			template <class ostream>
			int DxRawIndexBuffer::Serialize(
				ostream& os, const std::string &objName, const Padding & padding) const
			{
				os << padding << objName.c_str() << " (DxRawIndexBuffer) { pIB = " << m_pIB << " }";
				return 0;
			}

#endif SLV_DEBUG_LOGGING
		}
	}
}