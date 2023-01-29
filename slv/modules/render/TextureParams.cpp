#include "TextureParams.h"

using TexParams = slv::modules::render::TextureParams;

TexParams::TextureParams()
	: minFilter(FilterMode::Point),
	magFilter(FilterMode::Point),
	addrU(AddressMode::Wrap),
	addrV(AddressMode::Wrap)
{}

void TexParams::Apply(IDirect3DDevice9 &device, DWORD sampler)
{
	device.SetSamplerState(sampler, D3DSAMP_MINFILTER, static_cast<DWORD>(minFilter));
	device.SetSamplerState(sampler, D3DSAMP_MAGFILTER, static_cast<DWORD>(magFilter));

	device.SetSamplerState(sampler, D3DSAMP_ADDRESSU, static_cast<DWORD>(addrU));
	device.SetSamplerState(sampler, D3DSAMP_ADDRESSV, static_cast<DWORD>(addrV));
}