#include "CylinderRenderer.h"

namespace ps = slv::modules::render::primitives;

ps::CylinderRenderer::CylinderRenderer(const Mesh* pMesh, const CylinderMesh* pCylinderMesh, float r, float h)
	: m_pMesh(pMesh), m_pCylinderMesh(pCylinderMesh),
	m_radius(r), m_height(h),
	m_pos(vec3()), m_dir(0.f, 1.f, 0.f),
	m_fillMode(FillMode::Solid)
{
}

ps::CylinderRenderer::~CylinderRenderer()
{
}

void ps::CylinderRenderer::Render(IDirect3DDevice9& dev, UINT streamNumber)
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
	dev.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0u, verticesLoc.count - 2u);

	dev.SetIndices(indicesLoc.pBuffer->RawBuf().Buf());
	auto slices = m_pCylinderMesh->Slices();
	// dev.DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0u, 0u, slices, 0u, slices - 2u);
	// dev.DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0u, 0u, slices, slices, slices - 2u);

	dev.SetRenderState(D3DRS_FILLMODE, oldFillMode);
}