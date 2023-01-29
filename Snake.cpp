#include "Snake.h"

namespace slv
{
	const float Snake::s_pieceLength = 0.2f;

	Snake::Snake(int maxPointsCount, const vec3 &headPos, const vec3 &headDir, int initialPointsCount)
		: m_head(0),
		m_maxPointsCount(maxPointsCount),
		m_maxGuidesCount(10000), m_firstGuide(0), m_lastGuide(0)
	{
		m_points = new Point[maxPointsCount];
		vec3 dir;
		D3DXVec3Normalize(&dir, &headDir);
		if (initialPointsCount > maxPointsCount) initialPointsCount = maxPointsCount;
		m_pointsCount = initialPointsCount;
		m_tail = initialPointsCount - 1;
		for (int i = 0; i < initialPointsCount; ++i)
		{
			m_points[i] = { headPos - (dir * s_pieceLength * i), dir, m_firstGuide };
		}
		m_guides = new Guide[m_maxGuidesCount];
		m_guides[m_firstGuide] = { m_points[m_head + 1].pos, dir };
	}
	Snake::~Snake()
	{
		delete[] m_guides;
		delete[] m_points;
	}

	void Snake::MoveForward(float distance)
	{
		m_points[m_head].pos += m_points[m_head].dir * distance;
		m_points[m_head + 1].pos += m_points[m_head + 1].dir * distance;
		m_guides[m_firstGuide].pos = m_points[m_head + 1].pos;
		for (int i = m_head + 2; i < m_pointsCount; ++i)
		{
			MovePointForward(i, distance);
		}
	}
	void Snake::Turn(float angDeg)
	{
		if (!IsTurnPossible()) return;
		mat4 mat;
		float angRad = angDeg * D3DX_PI / 180.0f;
		D3DXMatrixRotationZ(&mat, angRad);
		vec3 dir = HeadDir();
		D3DXVec3TransformCoord(&dir, &dir, &mat);
		HeadDir(dir);
	}
	void Snake::HeadDir(const vec3 &dir)
	{
		if (!IsTurnPossible()) return;
		vec3 d;
		D3DXVec3Normalize(&d, &dir);
		vec3 diff = d - HeadDir();
		float sqrMag = D3DXVec3Dot(&diff, &diff);
		if (sqrMag < 0.0000001f) return;
		Point *head = &m_points[m_head];
		Point *neck = &m_points[m_head + 1];
		head->pos = neck->pos + d * s_pieceLength;
		head->dir = neck->dir = d;
		AddGuide();
	}

	void Snake::MovePointForward(int index, float distance)
	{
		Point *p = &m_points[index];
		Guide *g = &m_guides[p->targetGuideIndex];
		p->pos += p->dir * distance;
		float restDistance = D3DXVec3Dot(&p->dir, &(p->pos - g->pos));
		while (restDistance > 0.0f)
		{
			p->dir = g->dir;
			p->pos = g->pos + g->dir * restDistance;
			auto next = NextGuide(p->targetGuideIndex);
			p->targetGuideIndex = (next == m_lastGuide ? m_firstGuide : next);
			g = &m_guides[p->targetGuideIndex];
			restDistance = D3DXVec3Dot(&p->dir, &(p->pos - g->pos));
			if (index == (m_pointsCount - 1)) RemoveGuide();
		}
	}
	int Snake::AddGuide()
	{
		int added = NextGuide(m_firstGuide);
		if (added == m_lastGuide) return -1;

		if (GuidesCount() > 1)
		{
			vec3 d = m_guides[m_firstGuide].pos - m_guides[PrevGuide(m_firstGuide)].pos;
			float sqrMag = d.x * d.x + d.y * d.y + d.z * d.z;
			if (sqrMag < 0.000001f)
			{
				m_guides[PrevGuide(m_firstGuide)].dir = m_guides[m_firstGuide].dir = m_points[m_head].dir;
				return -2;
			}
		}

		m_guides[m_firstGuide].dir = m_points[m_head].dir;
		m_guides[m_firstGuide].pos = m_points[m_head + 1].pos;
		m_guides[added] = m_guides[m_firstGuide];
		m_firstGuide = added;
		return added;
	}
	int Snake::RemoveGuide()
	{
		if (m_lastGuide == m_firstGuide) return -1;
		int removed = m_lastGuide;
		m_lastGuide = NextGuide(removed);
		return removed;
	}
}