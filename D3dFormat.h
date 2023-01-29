#pragma once
#include <d3d9.h>

namespace slv
{
	class D3dFormat
	{
	private:
		D3DFORMAT m_format;
		const char *m_name;
	public:
		D3dFormat(D3DFORMAT format, const char *name)
			: m_format(format), m_name(name)
		{}
		operator D3DFORMAT() { return m_format; }
		const char *Name() { return m_name; }


		static D3dFormat s_formats[];
		static const int s_formatsCount;

		static void PrintAvailableFormats(LPDIRECT3D9);
	};
}