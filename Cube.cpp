#include <iostream>
#include "Cube.h"

namespace slv
{


#pragma region vertices
	const D3DXVECTOR3 Cube::A(-0.5f, 0.5f, 0.5f);
	const D3DXVECTOR3 Cube::B(0.5f, 0.5f, 0.5f);
	const D3DXVECTOR3 Cube::C(0.5f, -0.5f, 0.5f);
	const D3DXVECTOR3 Cube::D(-0.5f, -0.5f, 0.5f);

	const D3DXVECTOR3 Cube::A1(-0.5f, 0.5f, -0.5f);
	const D3DXVECTOR3 Cube::B1(0.5f, 0.5f, -0.5f);
	const D3DXVECTOR3 Cube::C1(0.5f, -0.5f, -0.5f);
	const D3DXVECTOR3 Cube::D1(-0.5f, -0.5f, -0.5f);

	const Cube::Vertex Cube::s_vertices[]{
		{ A1, D3DXVECTOR3(0.0f, 0.0f, -1.0f), },
		{ B1, D3DXVECTOR3(0.0f, 0.0f, -1.0f), },
		{ C1, D3DXVECTOR3(0.0f, 0.0f, -1.0f), },
		{ D1, D3DXVECTOR3(0.0f, 0.0f, -1.0f), },

		{ A, D3DXVECTOR3(0.0f, 0.0f, 1.0f), },
		{ B, D3DXVECTOR3(0.0f, 0.0f, 1.0f), },
		{ C, D3DXVECTOR3(0.0f, 0.0f, 1.0f), },
		{ D, D3DXVECTOR3(0.0f, 0.0f, 1.0f), },
	};
	const int Cube::s_verticesCount = sizeof(Cube::s_vertices) / sizeof(Cube::s_vertices[0]);
#pragma endregion vertices


#pragma region indices
	const int Cube::s_indices[]{
		0, 1, 3, // A1, B1, D1
		2, 3, 1, // C1, D1, B1

		4, 1, 0, // A, B1, A1
		5, 1, 4, // B, B1, A

		4, 0, 3, // A, A1, D1
		4, 3, 7, // A, D1, D

		3, 2, 6, // D1, C1, C
		7, 3, 6, // D, D1, C

		1, 6, 2, // B1, C, C1
		5, 6, 1, // B, C, B1

		5, 4, 6, // B, A, C
		7, 6, 4  // D, C, A 
	};
	const int Cube::s_indicesCount = sizeof(Cube::s_indices) / sizeof(Cube::s_indices[0]);
	const int Cube::s_trianglesCount = Cube::s_indicesCount / 3;
#pragma endregion indices

	Cube::Cube() : m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr)
	{
		if (FAILED(App::Direct3dDevice()->CreateVertexBuffer(s_verticesCount * sizeof(Vertex), 0,
													Vertex::c_format,
													D3DPOOL_DEFAULT,
													&m_pVertexBuffer, nullptr)))
		{
			return;
		}
		VOID *pBV;
		if (FAILED(m_pVertexBuffer->Lock(0, sizeof(s_vertices), (void **)& pBV, 8)))
		{
			return;
		}
		memcpy(pBV, s_vertices, sizeof(s_vertices));
		m_pVertexBuffer->Unlock();

		if (FAILED(App::Direct3dDevice()->CreateIndexBuffer(sizeof(s_indices), 0,
												   D3DFMT_INDEX32,
												   D3DPOOL_DEFAULT,
												   &m_pIndexBuffer, nullptr)))
		{
			return;
		}
		VOID *pIndices;
		if (FAILED(m_pIndexBuffer->Lock(0, sizeof(s_indices), (VOID**)&pIndices, 0)))
		{
			return;
		}
		memcpy(pIndices, s_indices, sizeof(s_indices));
		m_pIndexBuffer->Unlock();
	}

	Cube::~Cube()
	{
		if(m_pIndexBuffer != nullptr) m_pIndexBuffer->Release();
		if(m_pVertexBuffer != nullptr) m_pVertexBuffer->Release();
	}

	void Cube::Render()
	{
		App::Direct3dDevice()->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));
		App::Direct3dDevice()->SetFVF(Vertex::c_format);
		App::Direct3dDevice()->SetIndices(m_pIndexBuffer);
		App::Direct3dDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, s_verticesCount, 0, s_trianglesCount);
	}
}