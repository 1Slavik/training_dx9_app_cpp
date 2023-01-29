#pragma once

namespace slv
{
	namespace utils
	{
		namespace ariphmetics
		{
			template <class Ostream>
			void PrintCharHex(Ostream &os, unsigned char v)
			{
				const unsigned int byteHalf = 4;
				unsigned int highHalf = v >> byteHalf;
				unsigned char hexHighHalf = highHalf < 10u
					? '0' + highHalf
					: 'a' + highHalf - 10u;
				unsigned int lowHalf = v & 0x0f;
				unsigned char hexLowHalf = lowHalf < 10u
					? '0' + lowHalf
					: 'a' + lowHalf - 10u;
				os << hexHighHalf << hexLowHalf;
			}

			template <class Ostream, typename T>
			void PrintHex(Ostream &os, T v)
			{
				os << "0x";
				unsigned char *pc = (unsigned char*)(&v);

				unsigned short x = 0x0001;
				// 1 - little endian
				// 0 - big endian
				int endianness = *((unsigned char *)&x);

				unsigned int byteIndex;
				for (unsigned int i = 0; i < sizeof(T); ++i)
				{
					os << ' ';
					byteIndex = endianness ? sizeof(T) - 1 - i : i;
					PrintCharHex(os, pc[byteIndex]);
				}
			}
		}
	}
}