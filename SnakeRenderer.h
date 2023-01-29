#pragma once
#include "Snake.h"
#include "slv/utils/debug/Debug.h"
#include "slv/modules/render/Texture2d.h"

namespace slv
{
	class SnakeRenderer
	{
		using vec2 = D3DXVECTOR2;
		using vec3 = D3DXVECTOR3;
		using mat4 = D3DXMATRIX;
		using Debug = slv::utils::debug::Debug;
		using App = slv::frwk::appl::Dx9App;
		using Tex2d = slv::modules::render::Texture2d;

#define ttex
#ifdef ttex
		struct Vertex
		{
			static const DWORD c_format = D3DFVF_XYZ | D3DFVF_TEX1;
			vec3 pos;
			vec2 uv;
		};
#else
		struct Vertex
		{
			static const DWORD c_format = D3DFVF_XYZ | D3DFVF_DIFFUSE;
			vec3 pos;
			DWORD color;
		};
#endif

		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuf;
		const Snake *m_pSnake;
		float m_width;
		vec3 m_camDir;
		Tex2d m_texture;
	public:
		SnakeRenderer(const Snake *snake, float width = 0.1f);
		~SnakeRenderer();

		void Render();
		inline void CamDir(const vec3 &camDir) { m_camDir = camDir; }
	private:
		void CalcShoulderBtwPoints(vec3 *pOut, int firstPoint, int secondPoint);
		void CalcShoulderForMiddlePoint(vec3 *pOut, int point);
		void CalcShoulderForEndpoint(vec3 *pOut, int endpoint);
		void SetPointVertices(Vertex *pVertices, int point, const vec3 &vertexOffset);
		void DebugRender();
	};
}