#define SLV_DEBUG_LOGGING 1

#include "framework.h"
#include "dx.h"
#include <d3d9.h>
#include <d3dx9.h>

#include "slv/framework/time/Time.h"
#include "slv/framework/appl/Screen.h"
#include "slv/framework/appl/DxApp.h"
#include "slv/modules/input/Input.h"
#include "slv/modules/input/KeyCode.h"

#include "slv/utils/DxMathEx.h"
#include "slv/utils/ariphmetics/numericalSystems.h"
#include "slv/utils/debug/Padding.h"
#include "slv/utils/debug/Debug.h"
#include "slv/utils/debug/StatusList.h"
#include "slv/utils/ostream/MathTypes.h"

#include "slv/components/Transform.h"

#include "slv/modules/render/primitives/ConeRenderer.h"
#include "slv/modules/render/primitives/mesh_data/ConeMesh.h"
#include "slv/modules/render/primitives/CylinderRenderer.h"
#include "slv/modules/render/primitives/mesh_data/CylinderMesh.h"

#include "slv/modules/render/VertexBuffer.h"
#include "slv/modules/render/IndexBuffer.h"


#include "slv/modules/render/Mesh.h"
#include "slv/modules/render/MeshGroup.h"


#define MAX_LOADSTRING 100


#pragma region types aliases

#pragma region dx types
using vec3 = D3DXVECTOR3;
using vec4 = D3DXVECTOR4;
using mat4 = D3DXMATRIX;
using quat = D3DXQUATERNION;
#pragma endregion dx types

#pragma region slv types
using Screen = slv::frwk::appl::Screen;
using App = slv::frwk::appl::Dx9App;
using Time = slv::frwk::time::Time;
using Input = slv::modules::input::Input;
using Debug = slv::utils::debug::Debug;

using Transform = slv::components::Transform;

using ConeMesh = slv::modules::render::primitives::ConeMesh;
using ConeRenderer = slv::modules::render::primitives::ConeRenderer;
using CylinderMesh = slv::modules::render::primitives::CylinderMesh;
using CylinderRenderer  = slv::modules::render::primitives::CylinderRenderer;

namespace render = slv::modules::render;

template <class Vertex>
using VertexBuffer = render::VertexBuffer<Vertex>;
template <class VertexIndex>
using IndexBuffer = render::IndexBuffer<VertexIndex>;

#pragma endregion slv types

#pragma endregion types aliases


void OnUpdate();
void OnRender();
void OnQuit();


void Matrix()
{
	D3DXMATRIX matrixWorld, matrixTransformation, matrixView, matrixProj;
	
	//world matrix

	D3DXMatrixTranslation(&matrixWorld, 0.0f, 0.0f, 0.0f);
	App::Direct3dDevice()->SetTransform(D3DTS_WORLD, &matrixWorld);

	static FLOAT camPosZ = -6.0f;
	
	vec3 camPos = vec3(0.0f, 0.0f, camPosZ);
	vec3 camDir = vec3(0.0f, 0.0f, 1.0f);
	// view matrix
	D3DXMatrixLookAtLH(&matrixView,
					   &camPos,
					   &(camPos + camDir),
					   &vec3(0.0f, 1.0f, 0.0f));
	App::Direct3dDevice()->SetTransform(D3DTS_VIEW, &matrixView);

	// proj matrix
	FLOAT fovRad = D3DX_PI / 3.0f;
	D3DXMatrixPerspectiveFovLH(&matrixProj, fovRad, Screen::AspectRatio(), 1.0f, 100.0f);
	App::Direct3dDevice()->SetTransform(D3DTS_PROJECTION, &matrixProj);

	// viewport
	D3DVIEWPORT9 viewport{ 0, 0, Screen::WidthPx(), Screen::HeightPx(), 0, 1 };
	App::Direct3dDevice()->SetViewport(&viewport);
}


#pragma region vertices

struct Vertex
{
	vec3 xyz;
	DWORD color;
	static const DWORD fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};
template<class Ostream>
Ostream& operator<<(Ostream& os, const Vertex &v)
{
	os << "{ xyz = (" << v.xyz.x << ", " << v.xyz.y << ", " << v.xyz.z << "), "
		<< "color = ";
	slv::utils::ariphmetics::PrintHex(std::cout, v.color);
	std::cout << " }";
	return os;
}

Vertex g_vertices[] = {
	{ vec3(0.f, 0.f, -2.f), 0xffffffff },
	{ vec3(2.f, 0.f, 0.f), 0xff999999 },
	{ vec3(0.f, -3.f, 0.f), 0xff000000 },

	{ vec3(0.f, 0.f, 0.f), 0xffffffff },
	{ vec3(2.f, 3.f, 0.f), 0xff999999 },
	{ vec3(-2.f, 3.f, 0.f), 0x00000000 },
};
const auto g_verticesCount = sizeof(g_vertices) / sizeof(g_vertices[0]);
Vertex g_vertices2[] = {
	{ vec3(111, 111, 111), 0xaaaaaaaa },
	{ vec3(222, 222, 222), 0xbbbbbbbb },
	{ vec3(333, 333, 333), 0xcccccccc }
};
const auto g_verticesCount2 = sizeof(g_vertices2) / sizeof(g_vertices2[0]);

#pragma endregion vertices

#pragma region indices


struct VertexIndex
{
	UINT16 index;
	static const D3DFORMAT format = D3DFORMAT::D3DFMT_INDEX16;
};
template<class Ostream>
Ostream& operator<<(Ostream& os, const VertexIndex &v)
{
	os << v.index;
	return os;
}

VertexIndex g_indices[]{ 0, 1, 2 };
const auto g_indicesCount = sizeof(g_indices) / sizeof(g_indices[0]);
VertexIndex g_indices2[]{ 3, 1, 2, 2, 3, 1 };
const auto g_indicesCount2 = sizeof(g_indices2) / sizeof(g_indices2[0]);

#pragma endregion indices


using ConeVertex = ConeMesh::Vertex;
using ConeVertexIndex = ConeMesh::VertexIndex;

using CylinderVertex = CylinderMesh::Vertex;
using CylinderVertexIndex = CylinderMesh::VertexIndex;

ConeRenderer *g_pConeRenderer = nullptr;
ConeMesh *g_pConeMesh = nullptr;
render::MeshGroup<ConeVertex, ConeVertexIndex> *g_pMeshGroup;

CylinderRenderer *g_pCylinderRenderer = nullptr;
CylinderMesh *g_pCylinderMesh = nullptr;
render::MeshGroup<CylinderVertex, CylinderVertexIndex> *g_pCylinderMeshGroup;


#undef min
#undef max

struct AnimatedFloat
{
	float currentValue, targetValue, speed;

	void Target(float v) { targetValue = v; }
	void Speed(float v) { speed = v; }
	float Value() const { return currentValue; }

	void Update(float deltaTime)
	{
		float diff = targetValue - currentValue;
		float delta = speed * deltaTime;
	
		if(std::abs(diff) < delta)
		{ currentValue = targetValue; return; }

		currentValue = (diff > 0.f) ? currentValue + delta : currentValue - delta;
	}
};

struct SmoothRotation
{
	vec3 axis;
	float maxSpeed;

	void Rotate(vec3 &v, float deltaTime, float controlValue)
	{
		mat4 mat;
		D3DXMatrixRotationAxis(&mat, &axis, controlValue * maxSpeed * deltaTime);
		D3DXVec3TransformCoord(&v, &v, &mat);
	}
};

vec3 g_cameraPos = vec3(0.f, 0.f, 5.f);
SmoothRotation g_cameraRotationAnim = { vec3(0.f, 1.f, 0.f), 1.f };
AnimatedFloat g_cameraControlValue = { 0.f, 0.f, 10.f };

vec3 g_cylinderDir = vec3(0, 1, 0);
SmoothRotation g_cylinderRotationAnim = { vec3(1.f, 0.f, 0.f), 1.f };
AnimatedFloat g_cylinderControlValue = { 0.f, 0.f, 1.f };


VertexBuffer<Vertex> *g_pVb;



void InitGameObjects()
{
	using Params = render::DxBufParams;
	Params params;
	params.bufferType = Params::Type::Dynamic;
	params.clipping = Params::Clipping::Clip;
	params.optimizedPrimitives = Params::OptimizedPrimitives::None;
	params.otherFlags = Params::Other::WriteOnly;
	params.vertexProcessing = Params::VertexProcessing::Hardware;
	
	auto &device = *App::Direct3dDevice();

	g_pConeMesh = new ConeMesh(20u);
	const size_t sourcesCount = 1;
	auto **sources = new render::MeshSource<ConeVertex, ConeVertexIndex>*[sourcesCount];
	sources[0] = g_pConeMesh;

	g_pMeshGroup = new render::MeshGroup<ConeVertex, ConeVertexIndex>(device, sources, sourcesCount);

	g_pConeRenderer = new ConeRenderer(&g_pMeshGroup->Meshes()[0], g_pConeMesh, 2.0f, 4.0f);
	g_pConeRenderer->SetFillMode(ConeRenderer::FillMode::Wireframe);
	g_pConeRenderer->Dir(vec3(0.f, 0.f, -1.f));
	g_pConeRenderer->Pos(vec3(0.f, 0.f, 0.f));
	
	delete[] sources;
	
	
	g_pCylinderMesh = new CylinderMesh(20u);
	auto **cylinderSources = new render::MeshSource<CylinderVertex, CylinderVertexIndex>*[sourcesCount];
	cylinderSources[0] = g_pCylinderMesh;

	g_pCylinderMeshGroup = new render::MeshGroup<CylinderVertex, CylinderVertexIndex>(device, cylinderSources, sourcesCount);

	g_pCylinderRenderer = new CylinderRenderer(&g_pCylinderMeshGroup->Meshes()[0], g_pCylinderMesh, 1.0f, 3.0f);
	g_pCylinderRenderer->SetFillMode(CylinderRenderer::FillMode::Solid);
	g_pCylinderRenderer->Dir(vec3(0.f, 0.f, 1.f));
	g_pCylinderRenderer->Pos(vec3(0.f, 0.f, 0.f));

	delete[] cylinderSources;

	
	g_pVb = new VertexBuffer<Vertex>(device, g_verticesCount, params);
	g_pVb->Write(g_vertices, 0u, g_verticesCount, slv::modules::render::LockParams::None);

	const float deg2rad = D3DX_PI / 180.f;
}
void CleanupGameObjects()
{
	delete g_pVb;

	delete g_pCylinderRenderer;
	delete g_pCylinderMeshGroup;
	delete g_pCylinderMesh;
	
	delete g_pConeRenderer;
	delete g_pMeshGroup;
	delete g_pConeMesh;
}


void UpdateGameObjects()
{
	auto deltaTime = Time::Delta();
	
	auto& device = *App::Direct3dDevice();
	
#pragma region input
	if (Input::GetKeyDown(Input::KeyCode::N)) device.SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	else if (Input::GetKeyDown(Input::KeyCode::V)) device.SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	else if (Input::GetKeyDown(Input::KeyCode::C)) device.SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	else if (Input::GetKeyDown(Input::KeyCode::P))
	{
		g_pConeRenderer->SetFillMode(ConeRenderer::FillMode::Point);
		g_pCylinderRenderer->SetFillMode(ConeRenderer::FillMode::Point);
	}
	else if (Input::GetKeyDown(Input::KeyCode::W))
	{
		g_pConeRenderer->SetFillMode(ConeRenderer::FillMode::Wireframe);
		g_pCylinderRenderer->SetFillMode(ConeRenderer::FillMode::Wireframe);
	}
	else if (Input::GetKeyDown(Input::KeyCode::S))
	{
		g_pConeRenderer->SetFillMode(ConeRenderer::FillMode::Solid);
		g_pCylinderRenderer->SetFillMode(ConeRenderer::FillMode::Solid);
	}


	auto &camAnim = g_cameraControlValue;
	auto &cylAnim = g_cylinderControlValue;
	camAnim.Update(deltaTime);
	cylAnim.Update(deltaTime);

	float target = 0.f;
	if (Input::GetKey(Input::KeyCode::Right)) ++target;
	if (Input::GetKey(Input::KeyCode::Left)) --target;
	camAnim.Target(target);
	g_cameraRotationAnim.Rotate(g_cameraPos, deltaTime, camAnim.Value());

	target = 0.f;
	if (Input::GetKey(Input::KeyCode::Up)) ++target;
	if (Input::GetKey(Input::KeyCode::Down)) --target;
	cylAnim.Target(target);
	g_cylinderRotationAnim.Rotate(g_cylinderDir, deltaTime, cylAnim.Value());
	g_pCylinderRenderer->Dir(g_cylinderDir);
	g_pConeRenderer->Dir(g_cylinderDir);

#pragma endregion input


	
	Time::Update();
}


void Init(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	Time::Init();
	Screen::Init();
	App::Init(hInstance, lpCmdLine, nCmdShow);
	App::CallbackUpdate(OnUpdate);
	App::CallbackRender(OnRender);
	App::CallbackQuit(OnQuit);

	Input::Init(hInstance, App::Window()->Handle());


	InitGameObjects();


}

void OnUpdate()
{
	Time::Update();
	Input::ReadInput();


	UpdateGameObjects();
}

void OnRender()
{
	if (App::Direct3dDevice() == NULL) return;
	auto &device = *App::Direct3dDevice();

	device.Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
								 D3DCOLOR_XRGB(200, 170, 40), 1.0f, 0);
	device.BeginScene();

	Matrix();


	mat4 cameraMatrix;
	vec3 target(0, 0, 0);
	vec3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&cameraMatrix, &g_cameraPos, &target, &up);
	device.SetTransform(D3DTS_VIEW, &cameraMatrix);
	g_pConeRenderer->Render(device, 0);

	//g_pCylinderRenderer->Render(device, 0);

	//g_pVertexBuffer->Use(device, 0, 0);

	// g_pIndexBuffer->Use(device, 0, 0);
	// device.DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	// device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 3, 0, 1);

	device.EndScene();
	device.Present(NULL, NULL, NULL, NULL);
}

void OnQuit()
{
	CleanupGameObjects();

	Input::Cleanup();
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	Init(hInstance, lpCmdLine, nCmdShow);
	App::EnterGameLoop();

	return 0;
}