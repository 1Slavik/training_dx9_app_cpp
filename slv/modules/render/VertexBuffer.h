#pragma once
#include "DxRawVertexBuffer.h"

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
			template <class Vertex>
			class VertexBuffer
			{
				DxRawVertexBuffer m_rawBuffer;
				UINT m_verticesCount;

			public:
				VertexBuffer() = delete;
				VertexBuffer(IDirect3DDevice9&, UINT verticesCount, const DxBufParams&);
				~VertexBuffer();

				int Write(const Vertex *pVertices, UINT destinationStartIndex,
					UINT count, const LockParams&);

				int Use(IDirect3DDevice9&, UINT streamNumber, UINT startIndex);

				inline operator bool() const { return (bool)m_rawBuffer; }
				inline const DxRawVertexBuffer& RawBuf() const { return m_rawBuffer; }
				

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


			template<class Vertex>
			VertexBuffer<Vertex>::VertexBuffer(IDirect3DDevice9 &device,
				UINT verticesCount, const DxBufParams& params)
				: m_rawBuffer(device, verticesCount * sizeof(Vertex), Vertex::fvf, params),
				m_verticesCount(m_rawBuffer ? verticesCount : 0)
			{}

			template<class Vertex>
			VertexBuffer<Vertex>::~VertexBuffer()
			{
#if SLV_DEBUG_LOGGING
				std::cout << "~VertexBuffer()\n";
#endif
			}

			template <class Vertex>
			int VertexBuffer<Vertex>::Write(const Vertex *pVertices, UINT destStartIndex,
				UINT count, const LockParams &params)
			{
				if (!m_rawBuffer) return 1;

				auto dxBuf = m_rawBuffer.Buf();
				const auto sz = sizeof(Vertex);
				auto dataSize = count * sz;

				Vertex *pBufferData;
				if (FAILED(dxBuf->Lock(destStartIndex * sz, dataSize,
					(void**)&pBufferData, static_cast<DWORD>(params))))
				{
					return 2;
				}
				memcpy(pBufferData, pVertices, dataSize);

				dxBuf->Unlock();
				return 0;
			}

			template<class Vertex>
			int VertexBuffer<Vertex>::Use(IDirect3DDevice9 &device, UINT streamNumber, UINT startIndex)
			{
				device.SetFVF(Vertex::fvf);
				const auto sz = sizeof(Vertex);
				device.SetStreamSource(streamNumber, m_rawBuffer.Buf(), startIndex * sz, sz);
				return 0;
			}


#if SLV_DEBUG_LOGGING

			template <class Vertex>
			template <class ostream>
			int VertexBuffer<Vertex>::Serialize(
				ostream& os, const std::string &objName, const Padding & padding,
				UINT startIndex, UINT count) const
			{
				os << padding << objName.c_str() << " (VertexBuffer)\n";
				os << padding << "{\n";
				m_rawBuffer.Serialize(os, "rawBuffer", padding + Padding::defLength);
				os << '\n';
				SerializeBufferData(os, "", padding + Padding::defLength, startIndex, count);
				os << '\n' << padding << '}';
				return 0;
			}

			template <class Vertex>
			template <class ostream>
			int VertexBuffer<Vertex>::SerializeBufferData(
				ostream& os, const std::string &objName, const Padding & padding,
				UINT startIndex, UINT count) const
			{
				os << padding;
				if (objName.length() != 0) os << objName.c_str() << ' ';
				os << "vertex buffer data:\n";
				Padding dataPadding = padding + 2u;
				auto *pVB = m_rawBuffer.Buf();
				if (!pVB)
				{
					os << dataPadding << "ERROR: rawBuffer.Buf() == nullptr";
					return 1;
				}
				Vertex *bufVertices = nullptr;
				const auto sz = sizeof(Vertex);
				if (FAILED(pVB->Lock(startIndex * sz, count * sz, (void**)&bufVertices, D3DLOCK_READONLY)))
				{
					os << dataPadding << "ERROR: failed to lock the buffer";
					return 2;
				}
				os << padding << "{\n";
				for (unsigned int i = 0; i < count; ++i)
				{
					os << dataPadding << "vertices[" << i << "] = " << bufVertices[i] << "\n";
				}
				pVB->Unlock();
				os << padding << '}';
				return 0;
			}

#endif SLV_DEBUG_LOGGING
		}
	}
}