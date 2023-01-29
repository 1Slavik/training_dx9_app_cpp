#include "DxApp.h"

using Dx9App = slv::frwk::appl::Dx9App;


#pragma region events
std::function<void()> Dx9App::s_evtUpdate;
std::function<void()> Dx9App::s_evtRender;
std::function<void()> Dx9App::s_evtQuit;
#pragma endregion events

#pragma region windows
HINSTANCE Dx9App::s_hAppInstance;
slv::frwk::appl::WinWindow *Dx9App::s_pWindow;
#pragma endregion windows

#pragma region directx
LPDIRECT3D9 Dx9App::s_pD3d;
LPDIRECT3DDEVICE9 Dx9App::s_pD3dDevice;
D3DCAPS9 Dx9App::s_deviceCapabilities;
#pragma endregion directx

#pragma region flags
bool Dx9App::s_consoleExists;
#pragma endregion flags

int Dx9App::Init(HINSTANCE hinstance, WCHAR *lpCmdLine, int nCmdShow)
{
	CreateConsole(1024);
	s_hAppInstance = hinstance;
	s_pWindow = new WinWindow(hinstance, Screen::WidthPx(), Screen::HeightPx(), nCmdShow, WndProc);
	HACCEL hAccelTable = LoadAccelerators(hinstance, MAKEINTRESOURCE(IDC_DX));

	InitDirect3d();

	return 0;
}
void Dx9App::Cleanup()
{
	CleanupDirect3d();
	delete s_pWindow;
	ReleaseConsole();
}

int Dx9App::CreateConsole(int16_t minLength)
{
	s_consoleExists = WinConsole::CreateNewConsole(minLength);
	return !s_consoleExists;
}
void Dx9App::ReleaseConsole()
{
	if (s_consoleExists) WinConsole::ReleaseConsole();
}

HRESULT Dx9App::InitDirect3d()
{
	HWND hwnd = s_pWindow->Handle();

	if (NULL == (s_pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE displayMode;
	if (FAILED(s_pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
	{
		return E_FAIL;
	}

	FillDeviceCaps();
	auto vertexProcessingType = VertexProcessingType();

	D3DPRESENT_PARAMETERS presentationParams;
	FillPresentationParams(&presentationParams, displayMode);

	if (FAILED(s_pD3d->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vertexProcessingType,
		&presentationParams, &s_pD3dDevice)))
	{
		return E_FAIL;
	}
	s_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	s_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	s_pD3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	float s = 5.0f;
	s_pD3dDevice->SetRenderState(D3DRS_POINTSIZE, *(DWORD *)(&s));

	return S_OK;
}
void Dx9App::CleanupDirect3d()
{
	if (s_pD3dDevice != nullptr) s_pD3dDevice->Release();
	if (s_pD3d != nullptr) s_pD3d->Release();
}

void Dx9App::FillPresentationParams(
	D3DPRESENT_PARAMETERS *pd3dpp, const D3DDISPLAYMODE &displayMode)
{
	ZeroMemory(pd3dpp, sizeof(*pd3dpp));
	pd3dpp->BackBufferWidth = Screen::WidthPx();
	pd3dpp->BackBufferHeight = Screen::HeightPx();
	pd3dpp->BackBufferFormat = displayMode.Format;
	pd3dpp->BackBufferCount = 1;
	pd3dpp->MultiSampleType = D3DMULTISAMPLE_NONE;
	pd3dpp->MultiSampleQuality = 0;
	pd3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
	pd3dpp->hDeviceWindow = s_pWindow->Handle();
	pd3dpp->Windowed = true;
	pd3dpp->EnableAutoDepthStencil = true;
	pd3dpp->AutoDepthStencilFormat = D3DFMT_D24S8;
	pd3dpp->Flags = 0;
	pd3dpp->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pd3dpp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

int Dx9App::EnterGameLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	HWND hwnd = s_pWindow->Handle();
	while (1)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			NotifyUpdate();
			NotifyRender();
		}
	}
	NotifyQuit();
	Cleanup();
	return 0;
}
void Dx9App::LeaveGameLoop()
{
	PostQuitMessage(0);
}

LRESULT CALLBACK Dx9App::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		NotifyRender();
		ValidateRect(hWnd, NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}