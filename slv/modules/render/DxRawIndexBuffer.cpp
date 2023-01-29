#include "DxRawIndexBuffer.h"

using DxRawIndexBuffer = slv::modules::render::DxRawIndexBuffer;
namespace render = slv::modules::render;

render::DxRawIndexBuffer::DxRawIndexBuffer(
	IDirect3DDevice9 &dev, UINT capacity, D3DFORMAT indexFormat,
	const DxBufParams &params)
	: m_pIB(nullptr)
{
	if (capacity == 0u) return;
	dev.CreateIndexBuffer(capacity, static_cast<DWORD>(params), indexFormat,
		D3DPOOL_DEFAULT, &m_pIB, nullptr);
}

DxRawIndexBuffer::~DxRawIndexBuffer()
{
	SLV_SAFE_RELEASE(m_pIB);
}