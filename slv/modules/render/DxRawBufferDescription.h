//#pragma once
//#include <d3d9.h>
//
//namespace slv
//{
//	namespace modules
//	{
//		namespace render
//		{
//			template<class BufferParams>
//			struct DxRawVertexBufferDescription
//			{
//				using Buffer = IDirect3DVertexBuffer9;
//				using DataFormat = DWORD;
//
//				inline static HRESULT Create(IDirect3DDevice9 &dev, UINT length, const BufferParams &params,
//					DataFormat format, D3DPOOL pool, Buffer **ppVertexBuffer)
//				{
//					return dev.CreateVertexBuffer(length, static_cast<DWORD>(params),
//						format, pool, ppVertexBuffer, nullptr);
//				}
//			};
//
//			template<class BufferParams>
//			struct DxRawIndexBufferDescription
//			{
//				using Buffer = IDirect3DIndexBuffer9;
//				using DataFormat = D3DFORMAT;
//
//				static inline HRESULT Create(IDirect3DDevice9 &dev, UINT length, const BufferParams &params,
//					DataFormat format, D3DPOOL pool, Buffer **ppIndexBuffer)
//				{
//					return dev.CreateIndexBuffer(length, static_cast<DWORD>(params),
//						format, pool, ppIndexBuffer, nullptr);
//				}
//			};
//		}
//	}
//}