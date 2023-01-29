#pragma once

#include <d3dx9.h>

namespace slv
{
	namespace components
	{
		class Transform
		{
#pragma region types aliases
		public:
			using vec3 = D3DXVECTOR3;
			using quat = D3DXQUATERNION;
			using mat4 = D3DXMATRIX;
#pragma endregion types aliases

		private:
			static const Transform s_emptyTransform;

			Transform *m_pParent;
			// position in world space
			vec3 m_pos;


			static Transform ConstructEmptyTransform();
		public:
			Transform();

			inline static const Transform& Empty() { return s_emptyTransform; }
			// whether or not this Transform is 'Transform::Empty()'
			inline bool IsEmpty() const { return this == &s_emptyTransform; }

			inline void Parent(const Transform &t) { m_pParent = const_cast<Transform*>(&t); }
			inline const Transform& Parent() const { return *m_pParent; }
			
			// set world position
			inline void Pos(const vec3 &p) { m_pos = p; }
			// get world position
			inline vec3 Pos() const { return m_pos; }

			// set position in parent space
			inline void LocalPos(const vec3& p) { m_pos = m_pParent->m_pos + p; }
			// get position in parent space
			inline vec3 LocalPos() const { return m_pos - m_pParent->m_pos; }



#ifndef NDEBUG
			inline bool ParentIsNullptr() const { return m_pParent == nullptr; }
			inline bool ParentIsEmptyTransform() const { return m_pParent == &s_emptyTransform; }
#endif
		};
	}
}