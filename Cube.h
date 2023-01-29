#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "slv/framework/appl/DxApp.h"

using App = slv::frwk::appl::Dx9App;

namespace slv
{
	class Cube
	{
		struct Vertex
		{
			static const DWORD c_format = D3DFVF_XYZ | D3DFVF_NORMAL;
			D3DXVECTOR3 pos;
			D3DXVECTOR3 normal;
		};
	private:
		static const D3DXVECTOR3 A, B, C, D, A1, B1, C1, D1;
		static const Vertex s_vertices[];
		static const int s_indices[];
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
		static const int s_verticesCount;
		static const int s_indicesCount;
		static const int s_trianglesCount;

	public:
		Cube();
		~Cube();
		void Render();
	};
}