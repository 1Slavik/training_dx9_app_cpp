#pragma once
#include <functional>
#include <d3d9.h>
#include <d3dx9.h>

#include "Screen.h"
#include "WinWindow.h"
#include "Console.h"

namespace slv
{
	// framework
	namespace frwk
	{
		// application
		namespace appl
		{
			// directX 9 application
			// this is a static class
			class Dx9App
			{
#pragma region events
				static std::function<void()> s_evtRender;
				static std::function<void()> s_evtUpdate;
				static std::function<void()> s_evtQuit;
#pragma endregion events

#pragma region windows
				static HINSTANCE s_hAppInstance;
				static WinWindow *s_pWindow;
#pragma endregion windows
				
#pragma region directx
				static LPDIRECT3D9 s_pD3d;
				static LPDIRECT3DDEVICE9 s_pD3dDevice;
				static D3DCAPS9 s_deviceCapabilities;
#pragma endregion directx

#pragma region flags
				static bool s_consoleExists;
#pragma endregion flags

				static int CreateConsole(int16_t minLength);
				static void ReleaseConsole();
				static HRESULT InitDirect3d();
				static void CleanupDirect3d();
				static void FillPresentationParams(D3DPRESENT_PARAMETERS*, const D3DDISPLAYMODE&);
				static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			public:
				static int Init(HINSTANCE hinstance, WCHAR *lpCmdLine, int nCmdShow);
				static void Cleanup();

				static int EnterGameLoop();
				static void LeaveGameLoop();
				static inline void CallbackRender(std::function<void()> cb) { s_evtRender = cb; }
				static inline void CallbackUpdate(std::function<void()> cb) { s_evtUpdate = cb; }
				static inline void CallbackQuit(std::function<void()> cb) { s_evtQuit = cb; }

				static inline LPDIRECT3D9 Direct3d() { return s_pD3d; }
				static inline LPDIRECT3DDEVICE9 Direct3dDevice() { return s_pD3dDevice; }
				static inline WinWindow *Window() { return s_pWindow; }

				static inline long VertexProcessingType()
				{
					return (s_deviceCapabilities.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
						? D3DCREATE_HARDWARE_VERTEXPROCESSING
						: D3DCREATE_SOFTWARE_VERTEXPROCESSING;
				}
			private:
				static inline void NotifyRender() { if (s_evtRender) s_evtRender(); }
				static inline void NotifyUpdate() { if (s_evtUpdate) s_evtUpdate(); }
				static inline void NotifyQuit() { if (s_evtQuit) s_evtQuit(); }

				static inline void FillDeviceCaps()
				{ s_pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &s_deviceCapabilities); }
			};
		}
	}
}