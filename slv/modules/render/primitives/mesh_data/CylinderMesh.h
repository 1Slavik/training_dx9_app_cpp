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

#pragma region vertex info

				struct CylinderDetail
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
				Ostream& operator<<(Ostream& os, const CylinderDetail::Vertex& v)
				{
					os << "{ pos = (" << v.pos.x << ", " << v.pos.y << ", " << v.pos.z << "), "
						<< "color = " << v.c;
					std::cout << " }";
					return os;
				}
				template<class Ostream>
				Ostream& operator<<(Ostream& os, const CylinderDetail::VertexIndex& v)
				{
					os << v.index;
					return os;
				}

#pragma endregion vertex info


				namespace render = ::slv::modules::render;

				class CylinderMesh : public render::MeshSource
					<CylinderDetail::Vertex, CylinderDetail::VertexIndex>
				{
				public:
					using vec3 = D3DXVECTOR3;
					using mat4 = D3DXMATRIX;
					using App = slv::frwk::appl::Dx9App;
					using Debug = slv::utils::debug::Debug;

					using Vertex = CylinderDetail::Vertex;
					using VertexIndex = CylinderDetail::VertexIndex;

				private:
					static std::vector<CylinderMesh> s_meshes;

					UINT m_slices;
					Vertex* m_vertices;
					UINT m_verticesCount;
					VertexIndex* m_indices;
					UINT m_indicesCount;

				public:
					explicit CylinderMesh(UINT slices) noexcept;
					CylinderMesh(const CylinderMesh&) noexcept;
					CylinderMesh(CylinderMesh&&) noexcept;
					~CylinderMesh();

					CylinderMesh& operator=(const CylinderMesh&);
					CylinderMesh& operator=(CylinderMesh&&);

					inline UINT Slices() const { return m_slices; }

				private:
					int AllocateMemory(UINT verticesCount, Vertex** pVertices,
						UINT indicesCount, VertexIndex** pIndices);
					void ReleaseMemory(Vertex** pVertices, VertexIndex** pIndices);
					int FillVerticesData();
					int FillIndicesData();

				public:
					int ReadMeta(MeshMetadata& destination) const override;
					int ReadData(Vertex* destVertices, VertexIndex* destIndices) const override;


					static const CylinderMesh* Generate(int slices);
					static void Clear() { s_meshes.clear(); };

					template<typename Ostream>
					void PrintDebugInfo(Ostream& os, const char* name = "") const
					{
						os << name << " (CylinderMesh) {\n"
							<< "m_slices = " << m_slices
							<< "\nm_vertices = " << m_vertices
							<< "\nm_indices = " << m_indices
							<< "\n}\n";
					}

					template<typename Ostream>
					static void PrintRegisteredMeshes(Ostream& os)
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