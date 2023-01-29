#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "slv/framework/appl/DxApp.h"

namespace slv
{
	class Snake
	{
		using vec3 = D3DXVECTOR3;
		using mat4 = D3DXMATRIX;
		struct Point { vec3 pos; vec3 dir; int targetGuideIndex; };
		struct Guide { vec3 pos; vec3 dir; };

		static const float s_pieceLength;

		int m_head;
		int m_tail;
		int m_maxPointsCount;
		int m_pointsCount;
		Point *m_points;
		int m_maxGuidesCount;
		int m_firstGuide;
		int m_lastGuide;
		Guide *m_guides;
		bool m_moved = true;
	public:
		Snake(int maxPointsCount, const vec3 &pos, const vec3 &dir, int initialPointsCount = 5);
		~Snake();

		void MoveForward(float distance);
		void Turn(float angDeg);
		void HeadDir(const vec3 &dir);

		inline int PointsCount() const { return m_pointsCount; }
		inline int MaxPointsCount() const { return m_maxPointsCount; }
		inline vec3 operator[](int i) const { return m_points[i].pos; }
		inline vec3 HeadPos() const { return m_points[m_head].pos; }
		inline vec3 HeadDir() const { return m_points[m_head].dir; }

	private:
		void MovePointForward(int index, float distance);
		int AddGuide();
		int RemoveGuide();

		inline int NextGuide(int curGuide) { return (curGuide + 1) % m_maxGuidesCount; }
		inline int PrevGuide(int curGuide) { return (curGuide - 1) >= 0 ? curGuide - 1 : m_maxGuidesCount - 1; }
		inline bool IsTurnPossible() { return NextGuide(m_firstGuide) != m_lastGuide; }
		inline int GuidesCount() { return 1 + (m_firstGuide >= m_lastGuide
											   ? (m_firstGuide - m_lastGuide)
											   : (m_firstGuide + m_maxGuidesCount - m_lastGuide));
		}
	};
}