////
//// dx.cpp : Определяет точку входа для приложения.
////
//
//#include <chrono>
//#include "framework.h"
//#include "dx.h"
//#include <d3d9.h>
//#include <d3dx9.h>
//#include "utils/time/Timer.h"
//#include "slv/framework/appl/Console.h"
//#include "D3dFormat.h"
//#include "Cube.h"
//#include "CubicBezierCurve.h"
//#include "slv/modules/input/Input.h"
//#include "slv/modules/input/KeyCode.h"
//
//#include "Snake.h"
//
//#define MAX_LOADSTRING 100
//
//using vec3 = D3DXVECTOR3;
//using mat4 = D3DXMATRIX;
//
//struct MyVertex
//{
//	static const DWORD c_format = D3DFVF_XYZ | D3DFVF_DIFFUSE;
//	vec3 pos;
//	DWORD color;
//
//	MyVertex() {}
//	MyVertex(const vec3 &p, DWORD c)
//		: pos(p), color(c)
//	{}
//};
//
//
//// Глобальные переменные:
//HINSTANCE g_hInst;                                // текущий экземпляр
//HWND g_hwnd;
//WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
//WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
//const int g_screenWidth = 1280;
//const int g_screenHeight = 720;
//const FLOAT g_aspectRatio = (FLOAT)g_screenWidth / g_screenHeight;
//
//slv::Timer g_timer;
//bool g_bConsole = false;
//
//// dx
//LPDIRECT3D9 g_pD3d = nullptr;
//LPDIRECT3DDEVICE9 g_pD3dDevice = nullptr;
//LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = nullptr;
//
//const UINT g_verticesCount = 4;
//MyVertex g_vertices[g_verticesCount]
//{
//	MyVertex(vec3(0.0f, 0.0f, 0.0f), 0xffffffff),
//	MyVertex(vec3(-0.5f, 0.5f, 0.0f), 0xffffffff),
//	MyVertex(vec3(0.5f, 0.5f, 0.0f), 0xffffffff),
//	MyVertex(vec3(0.5f, -0.5f, 0.0f), 0xffffffff),
//};
//
//
//slv::input::Input g_input;
//
//#pragma region init direct3d
//
//void CreateVertexBuffer();
//void CheckDeviceCaps(int *vertexProcTp);
//void FillPresentationParams(D3DPRESENT_PARAMETERS *pd3dpp, const D3DDISPLAYMODE &displayMode);
//
//HRESULT InitDirect3d(HWND hwnd)
//{
//	if (NULL == (g_pD3d = Direct3DCreate9(D3D_SDK_VERSION)))
//	{
//		return E_FAIL;
//	}
//
//	D3DDISPLAYMODE displayMode;
//	if (FAILED(g_pD3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
//	{
//		return E_FAIL;
//	}
//
//	int vertexProcessingType;
//	CheckDeviceCaps(&vertexProcessingType);
//
//	D3DPRESENT_PARAMETERS presentationParams;
//	FillPresentationParams(&presentationParams, displayMode);
//
//	if (FAILED(g_pD3d->CreateDevice(
//		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
//		hwnd, vertexProcessingType,
//		&presentationParams, &g_pD3dDevice)))
//	{
//		return E_FAIL;
//	}
//	g_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//	g_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
//	g_pD3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//
//	float s = 5.0f;
//	g_pD3dDevice->SetRenderState(D3DRS_POINTSIZE, *(DWORD *)(&s));
//
//	CreateVertexBuffer();
//
//	return S_OK;
//}
//
//
//void CreateVertexBuffer()
//{
//	UINT bufferSize = sizeof(MyVertex) * g_verticesCount;
//	if (FAILED(g_pD3dDevice->CreateVertexBuffer(
//		bufferSize, 0, MyVertex::c_format, D3DPOOL_DEFAULT, &g_pVertexBuffer, nullptr)))
//	{
//		std::cout << "file: " << __FILE__ << std::endl
//			<< "line: " << __LINE__ << std::endl
//			<< "creating vertex buffer failed" << std::endl;
//		return;
//	}
//
//	void *pVertices;
//	if (FAILED(g_pVertexBuffer->Lock(0, 0, &pVertices, 8)))
//	{
//		std::cout << "file: " << __FILE__ << std::endl
//			<< "line: " << __LINE__ << std::endl
//			<< "locking vertex buffer failed" << std::endl;
//		return;
//	}
//	memcpy(pVertices, (void *)g_vertices, bufferSize);
//	g_pVertexBuffer->Unlock();
//}
//
//void CheckDeviceCaps(int *vertexProcessingType)
//{
//	D3DCAPS9 caps;
//	g_pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
//	*vertexProcessingType =
//		(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
//		? D3DCREATE_HARDWARE_VERTEXPROCESSING
//		: D3DCREATE_SOFTWARE_VERTEXPROCESSING;
//}
//
//void FillPresentationParams(D3DPRESENT_PARAMETERS *pd3dpp, const D3DDISPLAYMODE &displayMode)
//{
//	ZeroMemory(pd3dpp, sizeof(*pd3dpp));
//	pd3dpp->BackBufferWidth = g_screenWidth;
//	pd3dpp->BackBufferHeight = g_screenHeight;
//	pd3dpp->BackBufferFormat = displayMode.Format;
//	pd3dpp->BackBufferCount = 1;
//	pd3dpp->MultiSampleType = D3DMULTISAMPLE_NONE;
//	pd3dpp->MultiSampleQuality = 0;
//	pd3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
//	pd3dpp->hDeviceWindow = g_hwnd;
//	pd3dpp->Windowed = true;
//	pd3dpp->EnableAutoDepthStencil = true;
//	pd3dpp->AutoDepthStencilFormat = D3DFMT_D24S8;
//	pd3dpp->Flags = 0;
//	pd3dpp->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
//	pd3dpp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
//}
//
//#pragma endregion init direct3d
//
//#pragma region release direct3d resources
//VOID DeleteDirect3d()
//{
//	if (g_pVertexBuffer != nullptr) g_pVertexBuffer->Release();
//	if (g_pD3dDevice != nullptr) g_pD3dDevice->Release();
//	if (g_pD3d != nullptr) g_pD3d->Release();
//}
//#pragma endregion release direct3d resources
//
//slv::Snake * g_pSnake = nullptr;
//
//#pragma region rendering
//void Matrix();
//
//VOID RenderingDirect3d()
//{
//	if (g_pD3dDevice == NULL) return;
//
//	using Key = slv::input::KeyCode;
//	int ii = g_input.ReadInput();;
//
//
//	Key keyMoveForward = Key::W;
//	Key keyMoveForwardAlt = Key::Up;
//
//	g_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
//						D3DCOLOR_XRGB(200, 170, 40), 1.0f, 0);
//
//	g_pD3dDevice->BeginScene();
//
//	Matrix();
//
//	g_pD3dDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(MyVertex));
//	g_pD3dDevice->SetFVF(MyVertex::c_format);
//
//
//	mat4 matrixWorld;
//	vec3 p;
//	for (int i = 0; i < g_pSnake->PointsCount(); ++i)
//	{
//		p = (*g_pSnake)[i];
//		D3DXMatrixTranslation(&matrixWorld, p.x, p.y, p.z);
//		g_pD3dDevice->SetTransform(D3DTS_WORLD, &matrixWorld);
//
//		g_pD3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, 1);
//	}
//
//	g_pD3dDevice->EndScene();
//	g_pD3dDevice->Present(NULL, NULL, NULL, NULL);
//}
//#pragma endregion rendering
//
//
//vec3 g_turns[50];
//const int g_turnsCount = sizeof(g_turns) / sizeof(g_turns[0]);
//int g_turn = 0;
//float g_turnDeltaTime = 100.0f;
//float g_nextTime = 0.0f;
//
//void Matrix()
//{
//	D3DXMATRIX matrixWorld, matrixTransformation, matrixView, matrixProj;
//
//	static FLOAT prevTime = g_timer.ElapsedMs();
//	FLOAT timeMs = g_timer.ElapsedMs();
//	FLOAT deltaTime = timeMs - prevTime;
//	prevTime = timeMs;
//
//	g_pSnake->MoveForward(deltaTime * 0.001f);
//
//	if (timeMs > g_nextTime)
//	{
//		g_pSnake->HeadDir(g_turns[g_turn]);
//		g_turn = (g_turn + 1) % g_turnsCount;
//		g_nextTime += g_turnDeltaTime;
//	}
//
//	//world matrix
//
//	D3DXMatrixTranslation(&matrixWorld, 0.0f, 0.0f, 0.0f);
//	g_pD3dDevice->SetTransform(D3DTS_WORLD, &matrixWorld);
//
//	static FLOAT camPosZ = -6.0f;
//
//	// view matrix
//	D3DXMatrixLookAtLH(&matrixView,
//					   &vec3(0.0f, 0.0f, camPosZ),
//					   &vec3(0.0f, 0.0f, 0.0f),
//					   &vec3(0.0f, 1.0f, 0.0f));
//	g_pD3dDevice->SetTransform(D3DTS_VIEW, &matrixView);
//
//	// proj matrix
//	FLOAT fovRad = D3DX_PI / 3.0f;
//	D3DXMatrixPerspectiveFovLH(&matrixProj, fovRad, g_aspectRatio, 1.0f, 100.0f);
//	g_pD3dDevice->SetTransform(D3DTS_PROJECTION, &matrixProj);
//
//	// viewport
//	D3DVIEWPORT9 viewport{ 0,0,g_screenWidth,g_screenHeight,0,1 };
//	g_pD3dDevice->SetViewport(&viewport);
//}
//
//
//
//// Отправить объявления функций, включенных в этот модуль кода:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//					  _In_opt_ HINSTANCE hPrevInstance,
//					  _In_ LPWSTR    lpCmdLine,
//					  _In_ int       nCmdShow)
//{
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
//
//	// Инициализация глобальных строк
//	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//	LoadStringW(hInstance, IDC_DX, szWindowClass, MAX_LOADSTRING);
//	MyRegisterClass(hInstance);
//
//	if (!(g_bConsole = slv::CreateNewConsole(1024)))
//	{
//		return FALSE;
//	}
//
//
//	// to do
//
//
//	g_pSnake = new slv::Snake(100, vec3(-5, 0, 0), vec3(1, 0, 0), 100);
//
//	float t = 0.0f;
//	float dt = 5 * 3.14f / g_turnsCount;
//	vec3 v;
//	vec3 p;
//	vec3 prevP;
//	for (int i = 0; i < g_turnsCount; ++i, t += dt)
//	{
//		p = vec3(t, sin(t), 0.0f);
//		v = p - prevP;
//		if ((v.x * v.x + v.y * v.y + v.z * v.z) < 0.0001f) v = vec3(1, 0, 0);
//		D3DXVec3Normalize(&v, &v);
//		g_turns[i] = v;
//		prevP = p;
//	}
//
//	// Выполнить инициализацию приложения:
//	if (!InitInstance(hInstance, nCmdShow))
//	{
//		return FALSE;
//	}
//
//	g_input.Init(g_hInst, g_hwnd);
//
//	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX));
//	MSG msg;
//	ZeroMemory(&msg, sizeof(msg));
//	while (msg.message != WM_QUIT)
//	{
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		else
//		{
//			RenderingDirect3d();
//		}
//	}
//
//	if (g_bConsole) slv::ReleaseConsole();
//
//	return (int)msg.wParam;
//}
//
//
//
//// Регистрирует класс окна.
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//	WNDCLASSEXW wcex;
//
//	wcex.cbSize = sizeof(WNDCLASSEX);
//
//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc = WndProc;
//	wcex.cbClsExtra = 0;
//	wcex.cbWndExtra = 0;
//	wcex.hInstance = hInstance;
//	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX));
//	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DX);
//	wcex.lpszClassName = szWindowClass;
//	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//	return RegisterClassExW(&wcex);
//}
//
////        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
////        создается и выводится главное окно программы.
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//	g_hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
//
//	g_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//						   CW_USEDEFAULT, 0, g_screenWidth, g_screenHeight, nullptr, nullptr, hInstance, nullptr);
//
//	if (!g_hwnd)
//	{
//		return FALSE;
//	}
//
//	if (!SUCCEEDED(InitDirect3d(g_hwnd)))
//	{
//		return FALSE;
//	}
//
//	ShowWindow(g_hwnd, nCmdShow);
//	UpdateWindow(g_hwnd);
//
//	return TRUE;
//}
//
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//	case WM_COMMAND:
//	{
//		int wmId = LOWORD(wParam);
//		// Разобрать выбор в меню:
//		switch (wmId)
//		{
//		case IDM_ABOUT:
//			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//			break;
//		case IDM_EXIT:
//			DestroyWindow(hWnd);
//			break;
//		default:
//			return DefWindowProc(hWnd, message, wParam, lParam);
//		}
//	}
//	break;
//	case WM_PAINT:
//	{
//		// RenderingDirect3d();
//		ValidateRect(hWnd, NULL);
//	}
//	break;
//	case WM_DESTROY:
//		DeleteDirect3d();
//		PostQuitMessage(0);
//		break;
//	default:
//		return DefWindowProc(hWnd, message, wParam, lParam);
//	}
//	return 0;
//}
//
//// Обработчик сообщений для окна "О программе".
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	UNREFERENCED_PARAMETER(lParam);
//	switch (message)
//	{
//	case WM_INITDIALOG:
//		return (INT_PTR)TRUE;
//
//	case WM_COMMAND:
//		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//		{
//			EndDialog(hDlg, LOWORD(wParam));
//			return (INT_PTR)TRUE;
//		}
//		break;
//	}
//	return (INT_PTR)FALSE;
//}