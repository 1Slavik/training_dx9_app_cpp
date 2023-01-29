#pragma once

#include "MeshMetadata.h"

namespace slv
{
	namespace modules
	{
		namespace render
		{
			template <class Vertex, class VertexIndex>
			class MeshSource
			{
			public:
				using MeshMetadata = MeshMetadata<Vertex, VertexIndex>;

			public:
				virtual int ReadMeta(MeshMetadata &destination) const = 0;
				virtual int ReadData(Vertex *destVertices, VertexIndex *destIndices) const = 0;

			};



			template <class Vertex, class VertexIndex>
			class MeshTestSrc : public MeshSource<Vertex, VertexIndex>
			{
				using Base = MeshSource<Vertex, VertexIndex>;
				const Vertex *m_vertices;
				size_t m_verticesCount;
				const VertexIndex *m_indices;
				size_t m_indicesCount;

			public:
				MeshTestSrc(const Vertex *vertices, size_t verticesCount,
					const VertexIndex *indices, size_t indicesCount);

				int ReadMeta(typename Base::MeshMetadata &) const override;
				int ReadData(Vertex *destVertices, VertexIndex *destIndices) const override;
			};


			template <class Vertex, class VertexIndex>
			MeshTestSrc<Vertex, VertexIndex>::MeshTestSrc(
				const Vertex *vertices, size_t verticesCount,
				const VertexIndex *indices, size_t indicesCount)
				: m_vertices(vertices), m_verticesCount(verticesCount),
				m_indices(indices), m_indicesCount(indicesCount)
			{
			}

			template <class Vertex, class VertexIndex>
			int MeshTestSrc<Vertex, VertexIndex>::ReadMeta(typename Base::MeshMetadata &destination) const
			{
				destination.pSrc = this;
				destination.verticesCount = m_verticesCount;
				destination.indicesCount = m_indicesCount;
				return 0;
			}

			template <class Vertex, class VertexIndex>
			int MeshTestSrc<Vertex, VertexIndex>::ReadData(Vertex *destVertices, VertexIndex *destIndices) const
			{
				memcpy(destVertices, m_vertices, m_verticesCount * sizeof(Vertex));
				memcpy(destIndices, m_indices, m_indicesCount * sizeof(VertexIndex));
				return 0;
			}
		}
	}
}