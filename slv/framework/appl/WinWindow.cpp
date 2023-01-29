#include "WinWindow.h"

namespace app = slv::frwk::appl;
using Window = app::WinWindow;


Window::WinWindow(HINSTANCE hInstance, int w, int h, int nCmdShow, WNDPROC windowProcedure)
{
	m_screenWidth = w;
	m_screenHeight = h;
	m_aspectRatio = (float)w / h;
	
	LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, s_maxLoadstring);
	LoadStringW(hInstance, IDC_DX, m_szWindowClass, s_maxLoadstring);
	MyRegisterClass(hInstance, windowProcedure);
	m_hwnd = CreateWindowW(m_szWindowClass, m_szTitle,
						   WS_OVERLAPPEDWINDOW,
						   CW_USEDEFAULT, CW_USEDEFAULT,
						   w, h,
						   nullptr, nullptr,
						   hInstance, nullptr);
	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);
}

Window::~WinWindow()
{
}

ATOM Window::MyRegisterClass(HINSTANCE hInstance, WNDPROC windowProcedure)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = windowProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DX);
	wcex.lpszClassName = m_szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	return RegisterClassExW(&wcex);
}