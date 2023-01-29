#pragma once

#include "DxRawIndexBuffer.h"

#if SLV_DEBUG_LOGGING
#include <string>
#include "../../utils/debug/Padding.h"
#endif

namespace slv
{
	namespace modules
	{
		namespace render
		{
			template <class VertexIndex>
			class IndexBuffer
			{
				DxRawIndexBuffer m_rawBuffer;
				UINT m_indicesCount;

			public:
				IndexBuffer() = delete;
				IndexBuffer(IDirect3DDevice9&, UINT indicesCount, const DxBufParams&);
				~IndexBuffer();

				int Write(const VertexIndex *pIndices, UINT destinationStartIndex,
					UINT count, const LockParams&);

				int Use(IDirect3DDevice9&);

				inline operator bool() const { return (bool)m_rawBuffer; }
				inline const DxRawIndexBuffer& RawBuf() const { return m_rawBuffer; }


#pragma region debug
#if SLV_DEBUG_LOGGING
				using Padding = slv::utils::debug::Padding;
				template <class ostream>
				int Serialize(ostream&, const std::string& objName = "",
					const Padding& padding = Padding(0),
					UINT startIndex = 0, UINT count = 0) const;

				template <class ostream>
				int SerializeBufferData(ostream&, const std::string& objName = "",
					const Padding& padding = Padding(0),
					UINT startIndex = 0, UINT count = 0) const;
#endif
#pragma endregion debug
			};


			template <class VertexIndex>
			IndexBuffer<VertexIndex>::IndexBuffer(IDirect3DDevice9 &device,
				UINT indicesCount, const DxBufParams& params)
				: m_rawBuffer(device, indicesCount * sizeof(VertexIndex), VertexIndex::format, params),
				m_indicesCount(m_rawBuffer ? indicesCount : 0)
			{}

			template<class VertexIndex>
			IndexBuffer<VertexIndex>::~IndexBuffer()
			{
#if SLV_DEBUG_LOGGING
				std::cout << "~IndexBuffer()\n";
#endif
			}

			template <class VertexIndex>
			int IndexBuffer<VertexIndex>::Write(const VertexIndex *pIndices, UINT destStartIndex,
				UINT count, const LockParams &params)
			{
				if (!m_rawBuffer) return 1;

				auto dxBuf = m_rawBuffer.Buf();
				const auto sz = sizeof(VertexIndex);
				auto dataSize = count * sz;

				VertexIndex *pBufferData;
				if (FAILED(dxBuf->Lock(destStartIndex * sz, dataSize,
					(void**)&pBufferData, static_cast<DWORD>(params))))
				{
					return 2;
				}
				memcpy(pBufferData, pIndices, dataSize);

				dxBuf->Unlock();
				return 0;
			}

			template<class VertexIndex>
			int IndexBuffer<VertexIndex>::Use(IDirect3DDevice9 &device)
			{
				device.SetIndices(m_rawBuffer.Buf());
				return 0;
			}


#if SLV_DEBUG_LOGGING

			template <class VertexIndex>
			template <class ostream>
			int IndexBuffer<VertexIndex>::Serialize(
				ostream& os, const std::string &objName, const Padding & padding,
				UINT startIndex, UINT count) const
			{
				os << padding << objName.c_str() << " (IndexBuffer)\n";
				os << padding << "{\n";
				m_rawBuffer.Serialize(os, "rawBuffer", padding + Padding::defLength);
				os << '\n';
				SerializeBufferData(os, "", padding + Padding::defLength, startIndex, count);
				os << '\n' << padding << '}';
				return 0;
			}

			template <class VertexIndex>
			template <class ostream>
			int IndexBuffer<VertexIndex>::SerializeBufferData(
				ostream& os, const std::string &objName, const Padding & padding,
				UINT startIndex, UINT count) const
			{
				os << padding;
				if (objName.length() != 0) os << objName.c_str() << ' ';
				os << "index buffer data:\n";
				auto *pIB = m_rawBuffer.Buf();
				Padding dataPadding = padding + 2u;
				if(!pIB)
				{
					os << dataPadding << "ERROR: rawBuffer.Buf() == nullptr";
					return 1;
				}
				VertexIndex *bufIndices = nullptr;
				const auto sz = sizeof(VertexIndex);
				if (FAILED(pIB->Lock(startIndex * sz, count * sz, (void**)&bufIndices, D3DLOCK_READONLY)))
				{
					os << dataPadding << "ERROR: failed to lock the buffer";
					return 2;
				}
				os << padding << "{\n";
				for (unsigned int i = 0; i < count; ++i)
				{
					os << dataPadding << "indices[" << i << "] = " << bufIndices[i] << '\n';
				}
				pIB->Unlock();
				os << padding << '}';
				return 0;
			}

#endif SLV_DEBUG_LOGGING
		
		}
	}
}