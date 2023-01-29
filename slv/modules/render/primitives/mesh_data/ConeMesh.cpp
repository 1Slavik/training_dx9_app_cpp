#include "ConeMesh.h"

using ConeMesh = slv::modules::render::primitives::ConeMesh;

std::vector<ConeMesh> ConeMesh::s_meshes;

const ConeMesh *ConeMesh::Generate(int slices)
{
	if (slices < 3) slices = 3;
	auto iter = std::find_if(s_meshes.begin(), s_meshes.end(),
		[=](const ConeMesh& m) { return m.m_slices == slices; });
	if (iter == s_meshes.end())
	{
		s_meshes.push_back(ConeMesh(slices));
		iter = s_meshes.end() - 1;
	}
	return &(*iter);
}

ConeMesh::ConeMesh(UINT slices) noexcept
	: m_slices(slices < 3 ? 3 : slices), 
	m_vertices(nullptr), m_indices(nullptr)
{
	m_verticesCount = m_slices + 2u;
	m_indicesCount = m_slices;
	if (AllocateMemory(m_verticesCount, &m_vertices, m_indicesCount, &m_indices) != 0)
	{
		m_verticesCount = m_indicesCount = 0u;
		return;
	}
	if (FillVerticesData() != 0) return;
	FillIndicesData();


	std::cout << "ConeMesh::ConeMesh(uint slices)\n";
}
ConeMesh::ConeMesh(const ConeMesh &other) noexcept
	: ConeMesh(other.m_slices)
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
	std::cout << "ConeMesh::ConeMesh(const ConeMesh &other), other.m_slices = "
		<< other.m_slices<< std::endl;
#endif SLV_DEBUG_LOGGING
}

ConeMesh::ConeMesh(ConeMesh &&other) noexcept
	: m_slices(other.m_slices),
	m_vertices (other.m_vertices),
	m_verticesCount(other.m_verticesCount),
	m_indices(other.m_indices),
	m_indicesCount(other.m_indicesCount)
{
	other.m_vertices = nullptr;
	other.m_verticesCount = 0u;
	other.m_indices = nullptr;
	other.m_indicesCount = 0u;

#if SLV_DEBUG_LOGGING
	std::cout << "ConeMesh::ConeMesh(ConeMesh &&other)\n";
#endif SLV_DEBUG_LOGGING
}
ConeMesh::~ConeMesh()
{
	ReleaseMemory(&m_vertices, &m_indices);
}

ConeMesh &ConeMesh::operator=(const ConeMesh &other)
{
	if(this == &other) return *this;
	ReleaseMemory(&m_vertices, &m_indices);
	m_slices = other.m_slices;
	m_verticesCount = m_slices + 2;
	m_indicesCount = m_slices + 1;
	if (AllocateMemory(m_verticesCount, &m_vertices, m_indicesCount, &m_indices) != 0) return *this;
	if (FillVerticesData() != 0) return *this;
	FillIndicesData();
	return *this;
}
ConeMesh &ConeMesh::operator=(ConeMesh &&other)
{
	if(this == &other) return *this;
	std::swap(m_slices, other.m_slices);
	std::swap(m_vertices, other.m_vertices);
	std::swap(m_verticesCount, other.m_verticesCount);
	std::swap(m_indices, other.m_indices);
	std::swap(m_indicesCount, other.m_indicesCount);
	return *this;
}

int ConeMesh::AllocateMemory(UINT verticesCount, Vertex **pVertices,
	UINT indicesCount, VertexIndex **pIndices)
{
	*pVertices = new (std::nothrow) Vertex[verticesCount];
	if (!(*pVertices)) return 1;

	*pIndices = new (std::nothrow) VertexIndex[indicesCount];
	if (!(*pIndices)) return 2;

	return 0;
}
void ConeMesh::ReleaseMemory(Vertex **pVertices, VertexIndex **pIndices)
{
	delete[](*pVertices);
	*pVertices = nullptr;
	delete[](*pIndices);
	*pIndices = nullptr;
}
int ConeMesh::FillVerticesData()
{
	const DWORD apexColor = 0xffb0b0b0;
	const DWORD vertexColor = 0xff707070;
	const float angDeg = 360.0f / m_slices;
	const float angRad = angDeg * D3DX_PI / 180.0f;
	const float radius = 1.0f;
	const float height = 1.0f;
	const float halfHeight = height / 2.f;

	vec3 up(0.f, halfHeight, 0.f);
	vec3 &apex = up;
	m_vertices[0] = { apex, apexColor };

	mat4 mat;
	D3DXMatrixRotationY(&mat, angRad);

	vec3 radiusVector = vec3(0.f, 0.f, radius) - up;
	for (int i = 1; i < m_verticesCount; ++i)
	{
		m_vertices[i] = { radiusVector, vertexColor };
		D3DXVec3TransformCoord(&radiusVector, &radiusVector, &mat);
	}

	return 0;
}
int ConeMesh::FillIndicesData()
{
	UINT evenSlicesNumber = (m_slices - 1u) & 1u;
	UINT iters = m_slices / 2u - evenSlicesNumber;
	UINT16 first = m_slices - 1u;
	UINT16 second = 2u;
	m_indices[0].index = 1u;
	int index = 1;
	for (int i = 0; i < iters; ++i)
	{
		m_indices[index++].index = first--;
		m_indices[index++].index = second++;
	}
	if (evenSlicesNumber) m_indices[index].index = first;
	return 0;
}

int ConeMesh::ReadMeta(MeshMetadata &destination) const
{
	destination.pSrc = this;
	destination.verticesCount = m_verticesCount;
	destination.indicesCount = m_indicesCount;
	return 0;
}
int ConeMesh::ReadData(Vertex *destVertices, VertexIndex *destIndices) const
{
	if (!(destVertices && m_vertices)) return 1;
	if (!(destIndices && m_indices)) return 2;
	memcpy(destVertices, m_vertices, m_verticesCount * sizeof(Vertex));
	memcpy(destIndices, m_indices, m_indicesCount * sizeof(VertexIndex));
	return 0;
}