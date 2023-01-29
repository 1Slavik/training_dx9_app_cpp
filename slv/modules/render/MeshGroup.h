#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "MeshSource.h"

namespace slv
{
	namespace modules
	{
		namespace render
		{
			template <class Vertex, class VertexIndex>
			class MeshGroup
			{
#pragma region types aliases
			public:
				using Mesh = Mesh<Vertex, VertexIndex>;
				using MeshSource = MeshSource<Vertex, VertexIndex>;
				using Metadata = MeshMetadata<Vertex, VertexIndex>;
				using VertexBuffer = VertexBuffer<Vertex>;
				using IndexBuffer = IndexBuffer<VertexIndex>;
#pragma endregion types aliases
			private:
				VertexBuffer *m_pVB;
				IndexBuffer *m_pIB;
				Mesh *m_meshes;
				size_t m_meshesCount;

			public:
				MeshGroup(IDirect3DDevice9 &, MeshSource **pSources, size_t count);
				~MeshGroup();

				inline VertexBuffer* VB() { return m_pVB; }
				inline IndexBuffer * IB() { return m_pIB; }
				inline Mesh* Meshes() { return m_meshes; }
			};


			template <class Vertex, class VertexIndex>
			MeshGroup<Vertex, VertexIndex>::MeshGroup(IDirect3DDevice9 &dev, MeshSource **pSources, size_t count)
				: m_pVB(nullptr), m_pIB(nullptr), m_meshes(nullptr), m_meshesCount(0)
			{
				if (count == 0) return;


				Metadata *metadatas = new Metadata[count];
				Metadata summaryMeta = { nullptr, 0, 0 };
				for (size_t i = 0; i < count; ++i)
				{
					auto &meta = metadatas[i];
					pSources[i]->ReadMeta(meta);
					summaryMeta.verticesCount += meta.verticesCount;
					summaryMeta.indicesCount += meta.indicesCount;
				}
				
				DxBufParams params = {
					DxBufParams::Type::Static,
					DxBufParams::Clipping::DontClip,
					DxBufParams::OptimizedPrimitives::None,
					DxBufParams::Other::WriteOnly,
					DxBufParams::VertexProcessing::Hardware
				};
				m_pVB = new VertexBuffer(dev, summaryMeta.verticesCount, params);
				m_pIB = new IndexBuffer(dev, summaryMeta.indicesCount, params);
				m_meshes = static_cast<Mesh*>(::operator new(sizeof(Mesh) * count));
				m_meshesCount = count;

				auto *vertices = new Vertex[summaryMeta.verticesCount];
				auto *indices = new VertexIndex[summaryMeta.indicesCount];
				auto *pStartVertex = vertices;
				auto *pStartIndex = indices;

				using VerticesLoc = VerticesLocation<VertexBuffer, IndexBuffer>;

				for (size_t i = 0; i < count; ++i)
				{
					auto &meta = metadatas[i];
					pSources[i]->ReadData(pStartVertex, pStartIndex);

					VerticesLoc loc = {
						{ m_pVB, pStartVertex - vertices, meta.verticesCount },
						{ m_pIB, pStartIndex - indices, meta.indicesCount }
					};
					new(&m_meshes[i]) Mesh("nonameMesh", loc);

					pStartVertex += meta.verticesCount;
					pStartIndex += meta.indicesCount;
				}
				
				m_pVB->Write(vertices, 0, summaryMeta.verticesCount, LockParams::None);
				m_pIB->Write(indices, 0, summaryMeta.indicesCount, LockParams::None);


				delete[] indices;
				delete[] vertices;
				delete[] metadatas;
			}

			template <class Vertex, class VertexIndex>
			MeshGroup<Vertex, VertexIndex>::~MeshGroup()
			{
				for (size_t i = 0; i < m_meshesCount; ++i) m_meshes[i].~Mesh();
				::operator delete(m_meshes);
				delete m_pIB;
				delete m_pVB;
			}
		}
	}
}