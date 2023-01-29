#pragma once
#include <d3d9.h>
#include <d3dx9.h>


template<typename Ostream>
Ostream& operator<<(Ostream &os, const D3DXVECTOR2 &v)
{
	os << "( " << v.x << ", " << v.y << " )";
	return os;
}

template<typename Ostream>
Ostream& operator<<(Ostream &os, const D3DXVECTOR3 &v)
{
	os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
	return os;
}

template<typename Ostream>
Ostream& operator<<(Ostream &os, const D3DXQUATERNION &q)
{
	os << "( " << q.x << ", " << q.y << ", " << q.z << ", " << q.w << " )";
	return os;
}


template<typename Ostream>
Ostream& operator<<(Ostream &os, const D3DXPLANE &p)
{
	os << "( " << p.a << ", " << p.b << ", " << p.c << ", " << p.d << " )";
	return os;
}

template<typename Ostream, char newLine = '\n'>
Ostream& operator<<(Ostream &os, const D3DXMATRIX &m)
{
	os << '{' << newLine;
	for (int i = 0; i < 4; ++i)
	{
		os << '\t';
		for (int j = 0; j < 4; ++j)
		{
			os << m.m[i][j];
			if (!(i == 3 && j == 3)) os << ", ";
		}
		os << newLine;
	}
	os << '}' << newLine;
	return os;
}
