#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#include <slv_dx_common.h>
#include "../../../../framework/appl/DxApp.h"
#include "../../../../utils/debug/Debug.h"

#include "../../MeshSource.h"

#include <vector>
#include <algorithm>

namespace slv
{
	namespace modules
	{
		namespace render
		{
			namespace primitives
			{
				struct ConeDetail
				{
					using vec3 = D3DXVECTOR3;
					struct Vertex
					{
						vec3 pos;
						DWORD c;
						static const DWORD fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;
					};
					struct VertexIndex
					{
						UINT16 index;
						static const D3DFORMAT format = D3DFMT_INDEX16;
					};
				};
				template<class Ostream>
				Ostream& operator<<(Ostream& os, const ConeDetail::Vertex &v)
				{
					os << "{ pos = (" << v.pos.x << ", " << v.pos.y << ", " << v.pos.z << "), "
						<< "color = " << v.c;
					std::cout << " }";
					return os;
				}
				template<class Ostream>
				Ostream& operator<<(Ostream& os, const ConeDetail::VertexIndex &v)
				{
					os << v.index;
					return os;
				}



				namespace render = ::slv::modules::render;
				
				class ConeMesh : public render::MeshSource
					<ConeDetail::Vertex, ConeDetail::VertexIndex>
				{
				public:
					using vec3 = D3DXVECTOR3;
					using mat4 = D3DXMATRIX;
					using App = slv::frwk::appl::Dx9App;
					using Debug = slv::utils::debug::Debug;

					using Vertex = ConeDetail::Vertex;
					using VertexIndex = ConeDetail::VertexIndex;

				private:
					static std::vector<ConeMesh> s_meshes;

					UINT m_slices;
					Vertex *m_vertices;
					UINT m_verticesCount;
					VertexIndex *m_indices;
					UINT m_indicesCount;
					
				public:
					explicit ConeMesh(UINT slices) noexcept;
					ConeMesh(const ConeMesh&) noexcept;
					ConeMesh(ConeMesh&&) noexcept;
					~ConeMesh();

					ConeMesh &operator=(const ConeMesh &);
					ConeMesh &operator=(ConeMesh &&);

					inline UINT Slices() const { return m_slices; }

				private:
					int AllocateMemory(UINT verticesCount, Vertex **pVertices,
						UINT indicesCount, VertexIndex **pIndices);
					void ReleaseMemory(Vertex **pVertices, VertexIndex **pIndices);
					int FillVerticesData();
					int FillIndicesData();

				public:
					int ReadMeta(MeshMetadata &destination) const override;
					int ReadData(Vertex *destVertices, VertexIndex *destIndices) const override;


					static const ConeMesh* Generate(int slices);
					static void Clear() { s_meshes.clear(); };

					template<typename Ostream>
					void PrintDebugInfo(Ostream &os, const char* name = "") const
					{
						os  << name << " (ConeMesh) {\n"
							<< "m_slices = " << m_slices
							<< "\nm_vertices = " << m_vertices
							<< "\nm_indices = " << m_indices
							<< "\n}\n";
					}

					template<typename Ostream>
					static void PrintRegisteredMeshes(Ostream &os)
					{
						if (s_meshes.empty()) os << "\nno meshes registered\n";
						for (int i = 0; i < s_meshes.size(); ++i)
						{
							os << "s_meshes[" << i << "] = ";
							s_meshes[i].PrintDebugInfo(os);
						}
					}
				};
			}
		}
	}
}