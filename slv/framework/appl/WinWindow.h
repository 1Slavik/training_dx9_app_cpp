#pragma once
#include <Windows.h>
#include "../../../Resource.h"

namespace slv
{
	// framework
	namespace frwk
	{
		// application
		namespace appl
		{
			// Windows window
			class WinWindow
			{
				static const size_t s_maxLoadstring = 100;
				WCHAR m_szTitle[s_maxLoadstring];
				WCHAR m_szWindowClass[s_maxLoadstring];
				int m_screenWidth;
				int m_screenHeight;
				float m_aspectRatio;
				HWND m_hwnd;


				ATOM MyRegisterClass(HINSTANCE hInstance, WNDPROC windowProcedure);
			public:
				WinWindow(HINSTANCE hInstance, int w, int h, int nCmdShow, WNDPROC windowProcedure);
				~WinWindow();

				inline HWND Handle() { return m_hwnd; }
			};
		}
	}
}