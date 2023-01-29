#include <iostream>
#include "D3dFormat.h"

slv::D3dFormat slv::D3dFormat::s_formats[]
{
	slv::D3dFormat(D3DFMT_A1R5G5B5, "A1R5G5B5"),
	slv::D3dFormat(D3DFMT_A2R10G10B10, "A2R10G10B10"),
	slv::D3dFormat(D3DFMT_A8R8G8B8, "A8R8G8B8"),
	slv::D3dFormat(D3DFMT_R5G6B5, "R5G6B5"),
	slv::D3dFormat(D3DFMT_X1R5G5B5, "X1R5G5B5"),
	slv::D3dFormat(D3DFMT_X8R8G8B8, "X8R8G8B8"),
};
const int slv::D3dFormat::s_formatsCount = sizeof(s_formats) / sizeof(s_formats[0]);

void slv::D3dFormat::PrintAvailableFormats(LPDIRECT3D9 pDirect3d)
{
	for (int i = 0; i < s_formatsCount; ++i)
	{
		std::cout << "Format: " << s_formats[i].Name() << std::endl;
		UINT modesCount = pDirect3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, s_formats[i]);
		if (modesCount == 0) std::cout << "\tNo modes supported on this format\n";
		D3DDISPLAYMODE mode;
		HRESULT res;
		for (int j = 0; j < modesCount; ++j)
		{
			res = pDirect3d->EnumAdapterModes(D3DADAPTER_DEFAULT, s_formats[i], j, &mode);
			std::cout << "\tmodes[" << j << "] : ";
			switch (res)
			{
			case D3DERR_INVALIDCALL: std::cout << "D3DERR_INVALIDCALL"; break;
			case D3DERR_NOTAVAILABLE: std::cout << "D3DERR_NOTAVAILABLE"; break;
			case D3D_OK:
				std::cout
					<< mode.Width << " x "
					<< mode.Height << " | "
					<< mode.RefreshRate << " Hz";
				break;
			}
			std::cout << std::endl;
		}
	}
};