#pragma once

#include "../IRenderer.h"
#include "mesh_data/ConeMesh.h"
#include "../Mesh.h"

namespace slv
{
	namespace modules
	{
		namespace render
		{
			namespace primitives
			{
				class ConeRenderer : public ::slv::modules::render::IRenderer
				{
				public:
					using Vertex = ConeMesh::Vertex;
					using VertexIndex = ConeMesh::VertexIndex;
					using Mesh = ::slv::modules::render::Mesh<Vertex, VertexIndex>;
				private:
					const Mesh *m_pMesh;
					const ConeMesh *m_pConeMesh;

					float m_radius, m_height;
					vec3 m_pos, m_dir;
					FillMode m_fillMode;
				public:
					ConeRenderer(const Mesh*, const ConeMesh*, float r, float h);
					virtual ~ConeRenderer();

					void Render(IDirect3DDevice9&, UINT streamNumber) override;

					inline void Radius(float r) { m_radius = r; }
					inline void Height(float h) { m_height = h; }
					inline void Pos(const vec3 &p) { m_pos = p; }
					// this method will normalize the direction
					inline void Dir(const vec3 &d) { DirN(d / sqrt(d.x * d.x + d.y * d.y + d.z * d.z)); }
					// this method expects normalized direction
					inline void DirN(const vec3 &dirNormalized) { m_dir = dirNormalized; }
					inline void SetFillMode(FillMode mode) { m_fillMode = mode; }
				};
			}
		}
	}
}