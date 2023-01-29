#include "Texture2d.h"

using Texture = slv::modules::render::Texture2d;

Texture::DbgStatus Texture::s_statuses[] =
{
	{ S_OK, "OK", "" },
	{ D3DXERR_INVALIDDATA, "invalid data", "" },
	{ D3DERR_NOTAVAILABLE, "not available", "" },
	{ D3DERR_OUTOFVIDEOMEMORY, "out of video memory", "" },
	{ D3DERR_INVALIDCALL, "invalid call", "" },
	{ E_OUTOFMEMORY, "out of memory", "" },
};
Texture::DbgStatusList Texture::s_statusList = Texture::DbgStatusList(s_statuses, sizeof(s_statuses) / sizeof(s_statuses[0]));

Texture::Texture2d() 
	: m_pTexData(nullptr),
	m_params(TextureParams())
{}
Texture::~Texture2d()
{
	if (m_pTexData) m_pTexData->Release();
}

void Texture::LoadFromFile(const wchar_t *file)
{
	HRESULT res;
	if (FAILED(res = D3DXCreateTextureFromFile(App::Direct3dDevice(), file, &m_pTexData)))
	{
		Debug::Log(std::cout, s_statusList.GetStatus(res).strCode, __FILE__, __LINE__);
	}
}

void Texture::Use()
{
	auto &device = *App::Direct3dDevice();
	const DWORD sampler = 0;
	
	device.SetTexture(sampler, m_pTexData);
	m_params.Apply(device, 0);
}