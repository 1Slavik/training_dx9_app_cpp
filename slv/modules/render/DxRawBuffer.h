//#pragma once
//
//#include "DxRawBufferDescription.h"
//#include <slv_dx_common.h>
//#if SLV_DEBUG_LOGGING
//#include "../../utils/debug/Padding.h"
//#endif
//
//
//namespace slv
//{
//	namespace modules
//	{
//		namespace render
//		{
//			using VertexFormat = DWORD;
//
//			struct DxBufParams
//			{
//				enum Type { Static = 0, Dynamic = D3DUSAGE_DYNAMIC };
//				enum Clipping {Clip = 0, DontClip = D3DUSAGE_DONOTCLIP };
//				enum OptimizedPrimitives
//				{
//					None = 0,
//					NPatches = D3DUSAGE_NPATCHES,
//					Points = D3DUSAGE_POINTS,
//					RTPatches = D3DUSAGE_RTPATCHES
//				};
//				enum VertexProcessing
//				{
//					Hardware = 0,
//					Software = D3DUSAGE_SOFTWAREPROCESSING
//				};
//				enum Other
//				{
//					NonSecure = D3DUSAGE_NONSECURE,
//					TextApi = D3DUSAGE_TEXTAPI,
//					WriteOnly = D3DUSAGE_WRITEONLY
//				};
//
//				Type bufferType;
//				Clipping clipping;
//				OptimizedPrimitives optimizedPrimitives;
//				Other otherFlags;
//				VertexProcessing vertexProcessing;
//
//				operator DWORD() const
//				{
//					return static_cast<DWORD>(bufferType | clipping
//						| optimizedPrimitives | vertexProcessing | otherFlags);
//				}
//			};
//			enum LockParams
//			{
//				None = 0,
//				Discard = D3DLOCK_DISCARD,
//				NoDirtyUpdate = D3DLOCK_NO_DIRTY_UPDATE,
//				NoSysLock = D3DLOCK_NOSYSLOCK,
//				// ReadOnly = D3DLOCK_READONLY,
//				NoOverwrite = D3DLOCK_NOOVERWRITE
//			};
//
//			template<template<class BufParams> class RawBufDescr>
//			class DxRawBuffer
//			{
//			public:
//				using BufDescr = RawBufDescr<DxBufParams>;
//				using DxBuffer = typename BufDescr::Buffer;
//				using DataFormat = typename BufDescr::DataFormat;
//			private:
//				typename BufDescr::Buffer *m_pBuffer;
//			public:
//				DxRawBuffer(IDirect3DDevice9&, UINT capacity,
//					DataFormat, const DxBufParams&);
//				DxRawBuffer(const DxRawBuffer&) = delete; // temporary delete
//				DxRawBuffer(DxRawBuffer&&) = delete; // temporary delete
//				~DxRawBuffer();
//
//				DxRawBuffer& operator=(const DxRawBuffer&) = delete; // temporary delete
//				DxRawBuffer& operator=(DxRawBuffer&&) = delete; // temporary delete
//
//				inline DxBuffer* Buf() const { return m_pBuffer; }
//				inline operator bool() const { return m_pBuffer != nullptr; }
//
//
//#pragma region debug
//#if SLV_DEBUG_LOGGING
//				using Padding = slv::utils::debug::Padding;
//				template <class ostream>
//				int Serialize(ostream&, const std::string &objName = "", const Padding & padding = Padding(0)) const;
//#endif
//#pragma endregion debug
//			};
//
//
//			template<template<class BufParams> class RawBufDescr>
//			DxRawBuffer<RawBufDescr>::DxRawBuffer(
//				IDirect3DDevice9 &dev, UINT capacity,
//				DxRawBuffer<RawBufDescr>::DataFormat fvf,
//				const DxBufParams &params)
//				: m_pBuffer(nullptr)
//			{
//				if (capacity == 0) return;
//				// dev.CreateVertexBuffer(capacity, static_cast<DWORD>(params), fvf,
//					// D3DPOOL_DEFAULT, &m_pBuffer, nullptr);
//
//				BufDescr::Create(dev, capacity, params,
//					fvf, D3DPOOL_DEFAULT, &m_pBuffer);
//			}
//
//			template<template<class BufParams> class RawBufDescr>
//			DxRawBuffer<RawBufDescr>::~DxRawBuffer()
//			{
//				SLV_SAFE_RELEASE(m_pBuffer);
//			}
//
//
//
//#if SLV_DEBUG_LOGGING
//
//			template<template<class BufParams> class RawBufDescr>
//			template <class ostream>
//			int DxRawBuffer<RawBufDescr>::Serialize(
//				ostream& os, const std::string &objName, const Padding & padding) const
//			{
//				os << padding << objName.c_str() << " (DxRawBuffer) { pBuffer = " << m_pBuffer << " }";
//				return 0;
//			}
//
//#endif SLV_DEBUG_LOGGING
//
//
//			using DxRawVertexBuffer = DxRawBuffer<DxRawVertexBufferDescription>;
//			using DxRawIndexBuffer = DxRawBuffer<DxRawIndexBufferDescription>;
//		}
//	}
//}