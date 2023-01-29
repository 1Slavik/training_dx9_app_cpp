#pragma once

#include <string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VerticesLocation.h"

namespace slv
{
	namespace modules
	{
		namespace render
		{
			template <class Vertex, class VertexIndex>
			class Mesh
			{
#pragma region types aliases
			public:
				using VertexBuffer = VertexBuffer<Vertex>;
				using IndexBuffer = IndexBuffer<VertexIndex>;
				using VerticesLocation = VerticesLocation<VertexBuffer, IndexBuffer>;
#pragma endregion types aliases
			private:
				std::string m_name;
				VerticesLocation m_verticesLocation;
			public:
				Mesh(std::string name, const VerticesLocation&);


#pragma region debug
#if SLV_DEBUG_LOGGING
				using Padding = slv::utils::debug::Padding;
				template <class ostream>
				int Serialize(ostream&,
					const std::string& objName = "",
					const Padding &p = Padding(0),
					bool showBuffersData = false) const;
#endif
#pragma endregion debug

				inline const VerticesLocation& GetVerticesLocation() const { return m_verticesLocation; }
			};


			template <class Vertex, class VertexIndex>
			Mesh<Vertex, VertexIndex>::Mesh(std::string name, const VerticesLocation &loc)
				: m_name(name), m_verticesLocation(loc)
			{
			}


#if SLV_DEBUG_LOGGING

			template <class Vertex, class VertexIndex>
			template <class ostream>
			int Mesh<Vertex, VertexIndex>::Serialize(ostream& os,
				const std::string &objName, const Padding &padding, bool showBuffersData) const
			{
				os << padding << objName.c_str() << " (Mesh)\n";
				os << padding << "{\n";
				
				Padding dataPadding = padding + 2u;
				m_verticesLocation.Serialize(os, "verticesLocation", dataPadding);
				os << '\n';
				if (showBuffersData)
				{
					m_verticesLocation.vertices.pBuffer->SerializeBufferData(os,
						"vertices", dataPadding,
						m_verticesLocation.vertices.startIndex,
						m_verticesLocation.vertices.count);
					os << '\n';
					m_verticesLocation.indices.pBuffer->SerializeBufferData(os,
						"indices", dataPadding,
						m_verticesLocation.indices.startIndex,
						m_verticesLocation.indices.count);
					os << '\n';
				}

				os << padding << '}';
				return 0;
			}

#endif SLV_DEBUG_LOGGING

		}
	}
}