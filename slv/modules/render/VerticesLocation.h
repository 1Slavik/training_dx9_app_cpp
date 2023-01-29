#pragma once

namespace slv
{
	namespace modules
	{
		namespace render
		{
			template <class VertexBuffer, class IndexBuffer>
			struct VerticesLocation
			{
				template <class Buffer>
				struct BufferedLocation
				{
					Buffer *pBuffer;
					size_t startIndex;
					size_t count;

#if SLV_DEBUG_LOGGING
					using Padding = slv::utils::debug::Padding;
					template <class ostream>
					int Serialize(ostream&, const std::string& objName = "", const Padding &p = Padding(0)) const;
#endif SLV_DEBUG_LOGGING
				};

				BufferedLocation<VertexBuffer> vertices;
				BufferedLocation<IndexBuffer> indices;

#if SLV_DEBUG_LOGGING
				using Padding = slv::utils::debug::Padding;
				template <class ostream>
				int Serialize(ostream&, const std::string& objName = "", const Padding &p = Padding(0)) const;
#endif SLV_DEBUG_LOGGING
			};



#if SLV_DEBUG_LOGGING

			template <class VertexBuffer, class IndexBuffer>
			template <class Buffer>
			template <class ostream>
			int VerticesLocation<VertexBuffer, IndexBuffer>
				::BufferedLocation<Buffer>
				::Serialize(ostream& os, const std::string &objName, const Padding & padding) const
			{
				os << padding << objName.c_str() << " (BufferedLocation)\n";
				os << padding << "{\n";
				
				Padding dataPadding = padding + Padding::defLength;
				os << dataPadding << "pBuffer = " << pBuffer << '\n'
					<< dataPadding << "startIndex = " << startIndex << '\n'
					<< dataPadding << "count = " << count << '\n';

				os << padding << '}';
				return 0;
			}


			template <class VertexBuffer, class IndexBuffer>
			template <class ostream>
			int VerticesLocation<VertexBuffer, IndexBuffer>
				::Serialize(ostream& os, const std::string &objName, const Padding & padding) const
			{
				os << padding << objName.c_str() << " (VerticesLocation)\n";
				os << padding << "{\n";

				Padding dataPadding = padding + Padding::defLength;
				vertices.Serialize(os, "vertices location", dataPadding);
				os << '\n';
				indices.Serialize(os, "indices location", dataPadding);

				os << '\n' << padding << '}';
				return 0;
			}
#endif SLV_DEBUG_LOGGING

		}
	}
}