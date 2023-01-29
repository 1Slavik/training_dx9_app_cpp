#pragma once


namespace slv
{
	namespace modules
	{
		namespace render
		{
			template <class Vertex, class VertexIndex>
			class MeshSource;

			template <class Vertex, class VertexIndex>
			struct MeshMetadata
			{
				const MeshSource<Vertex, VertexIndex> *pSrc;
				size_t verticesCount;
				size_t indicesCount;
			};
		}
	}
}