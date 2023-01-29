#include "DxRawVertexBuffer.h"

namespace render = slv::modules::render;

render::DxRawVertexBuffer::DxRawVertexBuffer(
	IDirect3DDevice9 &dev, UINT capacity, DWORD fvf,
	const DxBufParams &params)
	: m_device(dev), m_pVB(nullptr), m_fvf(fvf)
{
	if (capacity == 0u) return;
	m_device.CreateVertexBuffer(capacity, static_cast<DWORD>(params), m_fvf,
		D3DPOOL_DEFAULT, &m_pVB, nullptr);
}

render::DxRawVertexBuffer::~DxRawVertexBuffer()
{
	SLV_SAFE_RELEASE(m_pVB);
}