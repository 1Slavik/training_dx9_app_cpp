#include "CylinderMesh.h"

using CylinderMesh = slv::modules::render::primitives::CylinderMesh;

std::vector<CylinderMesh> CylinderMesh::s_meshes;

const CylinderMesh* CylinderMesh::Generate(int slices)
{
	if (slices < 3) slices = 3;
	auto iter = std::find_if(s_meshes.begin(), s_meshes.end(),
		[=](const CylinderMesh& m) { return m.m_slices == slices; });
	if (iter == s_meshes.end())
	{
		s_meshes.push_back(CylinderMesh(slices));
		iter = s_meshes.end() - 1;
	}
	return &(*iter);
}

CylinderMesh::CylinderMesh(UINT slices) noexcept
	: m_slices(slices < 3 ? 3 : slices),
	m_vertices(nullptr), m_indices(nullptr)
{
	m_verticesCount = m_slices * 2u + 2u;
	m_indicesCount = m_slices * 2u;
	if (AllocateMemory(m_verticesCount, &m_vertices, m_indicesCount, &m_indices) != 0)
	{
		m_verticesCount = m_indicesCount = 0u;
		return;
	}
	if (FillVerticesData() != 0) return;
	FillIndicesData();


	std::cout << "CylinderMesh::CylinderMesh(UINT slices)\n";
}
CylinderMesh::CylinderMesh(const CylinderMesh& other) noexcept
	: CylinderMesh(other.m_slices)
	/*m_slices(other.m_slices),
	m_vertices(nullptr),
	m_verticesCount(other.m_verticesCount),
	m_indices(nullptr),
	m_indicesCount(other.m_indicesCount)*/
{
	/*if (AllocateMemory(m_verticesCount, &m_vertices, m_indicesCount, &m_indices) != 0) return;
	if (FillVerticesData() != 0) return;
	FillIndicesData();*/

#if SLV_DEBUG_LOGGING
	std::cout << "CylinderMesh::CylinderMesh(const CylinderMesh &other), other.m_slices = "
		<< other.m_slices << std::endl;
#endif SLV_DEBUG_LOGGING
}

CylinderMesh::CylinderMesh(CylinderMesh&& other) noexcept
	: m_slices(other.m_slices),
	m_vertices(other.m_vertices),
	m_verticesCount(other.m_verticesCount),
	m_indices(other.m_indices),
	m_indicesCount(other.m_indicesCount)
{
	other.m_vertices = nullptr;
	other.m_verticesCount = 0u;
	other.m_indices = nullptr;
	other.m_indicesCount = 0u;

#if SLV_DEBUG_LOGGING
	std::cout << "CylinderMesh::CylinderMesh(CylinderMesh &&other)\n";
#endif SLV_DEBUG_LOGGING
}
CylinderMesh::~CylinderMesh()
{
	ReleaseMemory(&m_vertices, &m_indices);
}

CylinderMesh& CylinderMesh::operator=(const CylinderMesh& other)
{
	if (this == &other) return *this;
	ReleaseMemory(&m_vertices, &m_indices);
	m_slices = other.m_slices;
	m_verticesCount = other.m_verticesCount;
	m_indicesCount = other.m_indicesCount;
	if (AllocateMemory(m_verticesCount, &m_vertices, m_indicesCount, &m_indices) != 0) return *this;
	if (FillVerticesData() != 0) return *this;
	FillIndicesData();
	return *this;
}
CylinderMesh& CylinderMesh::operator=(CylinderMesh&& other)
{
	if (this == &other) return *this;
	std::swap(m_slices, other.m_slices);
	std::swap(m_vertices, other.m_vertices);
	std::swap(m_verticesCount, other.m_verticesCount);
	std::swap(m_indices, other.m_indices);
	std::swap(m_indicesCount, other.m_indicesCount);
	return *this;
}

int CylinderMesh::AllocateMemory(UINT verticesCount, Vertex** pVertices,
	UINT indicesCount, VertexIndex** pIndices)
{
	*pVertices = new (std::nothrow) Vertex[verticesCount];
	if (!(*pVertices)) return 1;

	*pIndices = new (std::nothrow) VertexIndex[indicesCount];
	if (!(*pIndices)) return 2;

	return 0;
}
void CylinderMesh::ReleaseMemory(Vertex** pVertices, VertexIndex** pIndices)
{
	delete[](*pVertices);
	*pVertices = nullptr;
	delete[](*pIndices);
	*pIndices = nullptr;
}

int CylinderMesh::FillVerticesData()
{
	const DWORD topVerticesClr = 0xff707070;
	const DWORD bottomVerticesClr = 0xff101010;
	DWORD currentColor = topVerticesClr;
	const float angDeg = 360.0f / m_slices;
	const float angRad = angDeg * D3DX_PI / 180.0f;
	const float radius = 1.0f;
	const float height = 1.0f;
	const float halfHeight = height * 0.5f;

	mat4 rotationMat;
	D3DXMatrixRotationY(&rotationMat, angRad);
	vec3 radiusVector(0.f, 0.f, radius);
	vec3 up(0.f, halfHeight, 0.f);

	UINT vertex = 0u;
	for (UINT i = 0u; i <= m_slices; ++i)
	{
		//m_vertices[vertex++] = { radiusVector + up, topVerticesClr };
		//m_vertices[vertex++] = { radiusVector - up, bottomVerticesClr };
		m_vertices[vertex++] = { radiusVector + up, currentColor };
		m_vertices[vertex++] = { radiusVector - up, currentColor };
		currentColor = (currentColor == topVerticesClr) ? bottomVerticesClr : topVerticesClr;

		D3DXVec3TransformCoord(&radiusVector, &radiusVector, &rotationMat);
	}
	return 0;
}
int CylinderMesh::FillIndicesData()
{
	UINT evenSlicesNumber = (m_slices - 1u) & 1u;
	UINT iters = m_slices / 2u - evenSlicesNumber;

	// top
	UINT16 first = 2u;
	UINT16 second = m_slices * 2u - 2u;

	m_indices[0].index = 0u;
	int index = 1;
	for (UINT i = 0u; i < iters; ++i)
	{
		m_indices[index++].index = first;
		m_indices[index++].index = second;
		first += 2u;
		second -= 2u;
	}
	if (evenSlicesNumber) m_indices[index++].index = first;
	
	// bottom
	first = 3u;
	second = m_slices * 2u - 1u;

	m_indices[index++].index = 1u;
	for (UINT i = 0u; i < iters; ++i)
	{
		m_indices[index++].index = first;
		m_indices[index++].index = second;
		first += 2u;
		second -= 2u;
	}
	if (evenSlicesNumber) m_indices[index].index = first;

	return 0;
}

int CylinderMesh::ReadMeta(MeshMetadata& destination) const
{
	destination.pSrc = this;
	destination.verticesCount = m_verticesCount;
	destination.indicesCount = m_indicesCount;
	return 0;
}
int CylinderMesh::ReadData(Vertex* destVertices, VertexIndex* destIndices) const
{
	if (!(destVertices && m_vertices)) return 1;
	if (!(destIndices && m_indices)) return 2;
	memcpy(destVertices, m_vertices, m_verticesCount * sizeof(Vertex));
	memcpy(destIndices, m_indices, m_indicesCount * sizeof(VertexIndex));
	return 0;
}