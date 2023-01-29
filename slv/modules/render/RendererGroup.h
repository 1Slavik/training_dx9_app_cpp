#pragma once

#include <vector>
#include "DxBuffer.h"
#include "IGroupedRenderer.h"
#include "VerticesLocation.h"


namespace slv
{
	namespace modules
	{
		namespace render
		{
			template <class VertexIndex, class VertexIndex>
			class RendererGroup : public IRenderer
			{
#pragma region types aliases
			public:
				using VertexBuffer = DxVertexBuffer<VertexIndex>;
				using IndexBuffer = DxIndexBuffer<VertexIndex>;
#pragma endregion types aliases
			private:
				std::vector<IGroupedRenderer*> m_renderers;
				VertexBuffer *m_pVertexBuffer;
				IndexBuffer *m_pIndexBuffer;

			public:
				RendererGroup(VertexBuffer*, IndexBuffer*);
				~RendererGroup();

				void Add(const IGroupedRenderer*);
				void Remove(const IGroupedRenderer*);
				void Render(IDirect3DDevice9 &, UINT streamNumber) override;
			};


			template <class VertexIndex, class VertexIndex>
			RendererGroup<VertexIndex, VertexIndex>::RendererGroup(
				VertexBuffer *pVertexBuf, IndexBuffer *pIndexBuf)
				: m_renderers(),
				m_pVertexBuffer(pVertexBuf),
				m_pIndexBuffer(pIndexBuf)
			{
				m_renderers.reserve(4);
			}

			template <class VertexIndex, class VertexIndex>
			RendererGroup<VertexIndex, VertexIndex>::~RendererGroup()
			{
			}


			template <class VertexIndex, class VertexIndex>
			void RendererGroup<VertexIndex, VertexIndex>::Add(const IGroupedRenderer* pRend)
			{
				m_renderers.push_back(pRend);
			}
			template <class VertexIndex, class VertexIndex>
			void RendererGroup<VertexIndex, VertexIndex>::Remove(const IGroupedRenderer* pRend)
			{
				auto iter = std::find(m_renderers.cbegin(), m_renderers.cend(), pRend);
				m_renderers.erase(iter);
			}


			template <class VertexIndex, class VertexIndex>
			void RendererGroup<VertexIndex, VertexIndex>::Render(IDirect3DDevice9 &device, UINT streamNumber)
			{
				m_pVertexBuffer->Use(device, streamNumber, 0);
				m_pIndexBuffer->Use(device, streamNumber, 0);
				for (int i = 0; i < m_renderersCount; ++i)
				{
					m_renderers[i]->Render();
				}
			}
		}
	}
}