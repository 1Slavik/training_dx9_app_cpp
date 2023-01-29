#include "ConeRenderer.h"

namespace ps = slv::modules::render::primitives;

ps::ConeRenderer::ConeRenderer(const Mesh* pMesh, const ConeMesh* pConeMesh, float r, float h)
	: m_pMesh(pMesh), m_pConeMesh(pConeMesh),
	m_radius(r), m_height(h),
	m_pos(vec3()), m_dir(0.f, 1.f, 0.f),
	m_fillMode(FillMode::Solid)
{
}

ps::ConeRenderer::~ConeRenderer()
{
}

void ps::ConeRenderer::Render(IDirect3DDevice9 &dev, UINT streamNumber)
{
	const vec3 up = vec3(0.f, 1.f, 0.f);
	vec3 axis;
	D3DXVec3Cross(&axis, &up, &m_dir);
	float ang = acos(D3DXVec3Dot(&up, &m_dir));
	mat4 mat, tempMat;
	D3DXMatrixTranslation(&tempMat, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixRotationAxis(&mat, &axis, ang);
	mat = mat * tempMat;
	D3DXMatrixScaling(&tempMat, m_radius, m_height, m_radius);
	mat = tempMat * mat;

	DWORD oldFillMode;
	dev.GetRenderState(D3DRS_FILLMODE, &oldFillMode);
	dev.SetRenderState(D3DRS_FILLMODE, static_cast<DWORD>(m_fillMode));

	auto verticesLoc = m_pMesh->GetVerticesLocation().vertices;
	auto indicesLoc = m_pMesh->GetVerticesLocation().indices;

	dev.SetStreamSource(streamNumber, verticesLoc.pBuffer->RawBuf().Buf(), 0, sizeof(Vertex));
	dev.SetFVF(Vertex::fvf);
	dev.SetTransform(D3DTS_WORLD, &mat);
	dev.DrawPrimitive(D3DPT_TRIANGLEFAN, 0, verticesLoc.count - 2);

	dev.SetIndices(indicesLoc.pBuffer->RawBuf().Buf());
	auto slices = m_pConeMesh->Slices();
	dev.DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, slices, 0, slices - 2);

	dev.SetRenderState(D3DRS_FILLMODE, oldFillMode);
}