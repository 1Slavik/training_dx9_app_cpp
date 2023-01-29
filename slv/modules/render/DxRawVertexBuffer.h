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
			class DxRawVertexBuffer
			{
				IDirect3DDevice9 &m_device;
				IDirect3DVertexBuffer9 *m_pVB;
				DWORD m_fvf;
			public:
				DxRawVertexBuffer(IDirect3DDevice9&, UINT capacity, DWORD fvf, const DxBufParams&);
				DxRawVertexBuffer(const DxRawVertexBuffer&) = delete; // temporary delete
				DxRawVertexBuffer(DxRawVertexBuffer&&) = delete; // temporary delete
				~DxRawVertexBuffer();

				DxRawVertexBuffer& operator=(const DxRawVertexBuffer&) = delete; // temporary delete
				DxRawVertexBuffer& operator=(DxRawVertexBuffer&&) = delete; // temporary delete

				inline IDirect3DVertexBuffer9* Buf() const { return m_pVB; }
				inline operator bool() const { return m_pVB != nullptr; }



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
			int DxRawVertexBuffer::Serialize(
				ostream& os, const std::string &objName, const Padding & padding) const
			{
				os << padding << objName.c_str() << " (DxRawVertexBuffer) { pVB = " << m_pVB << " }";
				return 0;
			}

#endif SLV_DEBUG_LOGGING
		}
	}
}