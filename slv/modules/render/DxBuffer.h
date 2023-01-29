//#pragma once
//#include "DxRawBuffer.h"
//
//#if SLV_DEBUG_LOGGING
//#include "../../utils/debug/Padding.h"
//#endif
//
//namespace slv
//{
//	namespace modules
//	{
//		namespace render
//		{
//			template<class DxRawBuffer, class TBufferData>
//			struct DxBufferDescription
//			{
//				using BufferData = TBufferData;
//				using RawBuffer = DxRawBuffer;
//				using DataType = typename BufferData::DataType;
//				using DataFormat = typename BufferData::Format;
//			};
//
//			template <class Data>
//			using DxVertexBufferDescription = DxBufferDescription<DxRawVertexBuffer, Data>;
//
//			template <class Data>
//			using DxIndexBufferDescription = DxBufferDescription<DxRawIndexBuffer, Data>;
//
//
//			
//
//			template<class BufferDescription, template<class Buffer>class UsingPolicy>
//			class DxBuffer
//			{
//			public:
//				using Description = BufferDescription;
//				using Data = typename Description::BufferData;
//			private:
//				typename Description::RawBuffer m_rawBuffer;
//				UINT m_verticesCount;
//
//			public:
//				DxBuffer() = delete;
//				DxBuffer(IDirect3DDevice9 &, UINT verticesCount, const DxBufParams&);
//				~DxBuffer();
//
//				int Write(const Data *pVertices, UINT destStartIndex,
//					UINT count, const LockParams&);
//
//				int Use(IDirect3DDevice9&, UINT streamNumber, UINT startIndex);
//
//				inline operator bool() const { return (bool)m_rawBuffer; }
//				inline const typename Description::RawBuffer& RawBuf() { return m_rawBuffer; }
//
//
//#pragma region debug
//#if SLV_DEBUG_LOGGING
//				using Padding = slv::utils::debug::Padding;
//				template <class ostream>
//				int Serialize(ostream&, const std::string& objName = "", const Padding& padding = Padding(0)) const;
//#endif
//#pragma endregion debug
//			};
//
//
//			template<class BufferDescription, template<class Buffer>class UsingPolicy>
//			DxBuffer<BufferDescription, UsingPolicy>::DxBuffer(IDirect3DDevice9 &device,
//				UINT verticesCount, const DxBufParams& params)
//				: m_rawBuffer(device, verticesCount * sizeof(Data), Data::format, params),
//				m_verticesCount(m_rawBuffer ? verticesCount : 0)
//			{}
//			template<class BufferDescription, template<class Buffer>class UsingPolicy>
//			DxBuffer<BufferDescription, UsingPolicy>::~DxBuffer()
//			{
//#if SLV_DEBUG_LOGGING
//				std::cout << "~DxBuffer()\n";
//#endif
//			}
//
//			template<class BufferDescription, template<class Buffer>class UsingPolicy>
//			int DxBuffer<BufferDescription, UsingPolicy>::Write(const Data *pVertices, UINT destStartIndex,
//				UINT count, const LockParams &params)
//			{
//				if (!m_rawBuffer) return 1;
//
//				auto dxBuf = m_rawBuffer.Buf();
//				const auto sz = sizeof(Data);
//				auto dataSize = count * sz;
//
//				Data *pBufferData;
//				if (FAILED(dxBuf->Lock(destStartIndex * sz, dataSize,
//					(void**)&pBufferData, static_cast<DWORD>(params))))
//				{
//					return 2;
//				}
//				memcpy(pBufferData, pVertices, dataSize);
//
//				dxBuf->Unlock();
//				return 0;
//			}
//
//			template<class BufferDescription, template<class Buffer>class UsingPolicy>
//			int DxBuffer<BufferDescription, UsingPolicy>::Use(IDirect3DDevice9 &device, UINT streamNumber, UINT startIndex)
//			{
//				using Policy = UsingPolicy<DxBuffer<BufferDescription, UsingPolicy>>;
//				return Policy::Use(device, *this, streamNumber, startIndex);
//			}
//
//
//#if SLV_DEBUG_LOGGING
//
//			template<class BufferDescription, template<class Buffer>class UsingPolicy>
//			template <class ostream>
//			int DxBuffer<BufferDescription, UsingPolicy>::Serialize(
//				ostream& os, const std::string &objName, const Padding & padding) const
//			{
//				os << padding << objName.c_str() << " (DxBuffer)\n";
//				os << padding << "{\n";
//				m_rawBuffer.Serialize(os, "rawBuffer", padding + Padding::defLength);
//				os << '\n' << padding << '}';
//
//				/*Vertex *bufVertices;
//				pVB->Lock(0, 0, (void**)&bufVertices, D3DLOCK_READONLY);
//
//				VB->Unlock();
//				bufVertices = nullptr;*/
//				return 0;
//			}
//
//#endif SLV_DEBUG_LOGGING
//
//
//			template <class Buffer>
//			struct VertexBufUsing
//			{
//				static int Use(IDirect3DDevice9 &device, Buffer &buffer, UINT streamNumber, UINT startIndex)
//				{
//					const auto sz = sizeof(typename Buffer::Data);
//					device.SetStreamSource(streamNumber, buffer.RawBuf().Buf(), startIndex * sz, sz);
//					device.SetFVF(static_cast<DWORD>(Buffer::Data::format));
//					return 0;
//				}
//			};
//
//			template <class Buffer>
//			struct IndexBufUsing
//			{
//				static int Use(IDirect3DDevice9 &device, Buffer &buffer, UINT streamNumber, UINT startIndex)
//				{
//					const auto sz = sizeof(typename Buffer::Data);
//					device.SetIndices(buffer.RawBuf().Buf());
//					return 0;
//				}
//			};
//
//			
//			template <class Data>
//			using DxVertexBuffer = DxBuffer<DxVertexBufferDescription<Data>, VertexBufUsing>;
//
//			template <class Data>
//			using DxIndexBuffer = DxBuffer<DxIndexBufferDescription<Data>, IndexBufUsing>;
//			
//		}
//	}
//}