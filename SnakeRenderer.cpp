#include "SnakeRenderer.h"
#include "slv/modules/render/TextureParams.h"


slv::SnakeRenderer::SnakeRenderer(const Snake *snake, float width)
	: m_pVertexBuf(nullptr), m_pSnake(snake), m_width(width),
	m_camDir(vec3(0.0f, 0.0f, -1.0f)),
	m_texture(Tex2d())
{
	auto bufSize = snake->MaxPointsCount() * 2 * sizeof(Vertex);
	if (FAILED(App::Direct3dDevice()->CreateVertexBuffer(
		bufSize, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, Vertex::c_format, D3DPOOL_DEFAULT, &m_pVertexBuf, nullptr)))
		return;

	m_texture.LoadFromFile(L"/projects_cpp/dx/snakeTex.png");
}
slv::SnakeRenderer::~SnakeRenderer()
{
	if (m_pVertexBuf) m_pVertexBuf->Release();
}

void slv::SnakeRenderer::Render()
{
	auto &dev = *App::Direct3dDevice();

	const int count = m_pSnake->PointsCount();
	const int verticesCount = count * 2;

	Vertex *pVerticesData;
	if (FAILED(m_pVertexBuf->Lock(0, verticesCount * sizeof(Vertex), (void **)& pVerticesData, D3DLOCK_DISCARD)))
	{
		Debug::Log(std::cout, "SnakeRenderer::Render() : failed to lock vertex buffer", __FILE__, __LINE__);
		return;
	}

	vec3 vertexOffset;
	// CalcShoulderForEndpoint(&vertexOffset, 0);
	// SetPointVertices(pVerticesData, 0, vertexOffset);

	const Snake &points = *m_pSnake;

	int vi = 0;
	
	vec3 tangent = points[1] - points[0];
	D3DXVec3Cross(&vertexOffset, &tangent, &m_camDir);
	D3DXVec3Normalize(&vertexOffset, &vertexOffset);
	vertexOffset *= m_width;
	float uvX = 0.0f;
	pVerticesData[vi++] = { points[0] + vertexOffset, vec2(uvX, 1.0f) };
	pVerticesData[vi++] = { points[0] - vertexOffset, vec2(uvX, 0.0f) };
	++uvX;


	for (int i = 1; i < count - 1; ++i)
	{
		// CalcShoulderForMiddlePoint(&vertexOffset, i);
		// SetPointVertices(pVerticesData, i, vertexOffset);

		tangent = points[i + 1] - points[i - 1];
		D3DXVec3Cross(&vertexOffset, &tangent, &m_camDir);
		D3DXVec3Normalize(&vertexOffset, &vertexOffset);
		vertexOffset *= m_width;
		pVerticesData[vi++] = { points[i] + vertexOffset, vec2(uvX, 1.0f) };
		pVerticesData[vi++] = { points[i] - vertexOffset, vec2(uvX, 0.0f) };
		++uvX;
	}

	// CalcShoulderForEndpoint(&vertexOffset, count - 1);
	// SetPointVertices(pVerticesData, count - 1, vertexOffset);
	tangent = points[count - 1] - points[count - 2];
	D3DXVec3Cross(&vertexOffset, &tangent, &m_camDir);
	D3DXVec3Normalize(&vertexOffset, &vertexOffset);
	vertexOffset *= m_width;
	pVerticesData[vi++] = { points[count - 1] + vertexOffset, vec2(uvX, 1.0f) };
	pVerticesData[vi++] = { points[count - 1] - vertexOffset, vec2(uvX, 0.0f) };



	m_pVertexBuf->Unlock();

	m_texture.Use();

	dev.SetStreamSource(0, m_pVertexBuf, 0, sizeof(Vertex));
	dev.SetFVF(Vertex::c_format);
	dev.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, verticesCount - 2);
	// dev->DrawPrimitive(D3DPT_POINTLIST, 0, verticesCount);

	// DebugRender();
}

void slv::SnakeRenderer::CalcShoulderBtwPoints(vec3 * pOut, int firstPoint, int secondPoint)
{
	const Snake &points = *m_pSnake;
	vec3 tangent = points[secondPoint] - points[firstPoint];
	D3DXVec3Cross(pOut, &tangent, &m_camDir);
	D3DXVec3Normalize(pOut, pOut);
	*pOut *= m_width;
}
void slv::SnakeRenderer::CalcShoulderForMiddlePoint(vec3 * pOut, int point)
{
	CalcShoulderBtwPoints(pOut, point - 1, point + 1);
}
void slv::SnakeRenderer::CalcShoulderForEndpoint(vec3 * pOut, int endpoint)
{
	if (endpoint == 0) CalcShoulderBtwPoints(pOut, 0, 1);
	else CalcShoulderBtwPoints(pOut, endpoint - 1, endpoint);
}
void slv::SnakeRenderer::SetPointVertices(Vertex * pVertices, int point, const vec3 & vertexOffset)
{
	float uvX = (float)point;
	pVertices[point << 1] = { (*m_pSnake)[point] + vertexOffset, vec2(uvX, 1.0f) };
	pVertices[(point << 1) + 1] = { (*m_pSnake)[point] - vertexOffset, vec2(uvX, 0.0f) };
}
void slv::SnakeRenderer::DebugRender()
{
	auto dev = App::Direct3dDevice();
	auto count = m_pSnake->PointsCount();
	
	dev->SetStreamSource(0, m_pVertexBuf, 0, sizeof(Vertex));
	dev->SetFVF(Vertex::c_format);
	
	mat4 mat;
	for (int i = 0; i < count; ++i)
	{
		const vec3 &v = (*m_pSnake)[i];
		D3DXMatrixTranslation(&mat, v.x, v.y, v.z);
		dev->SetTransform(D3DTS_WORLD, &mat);
		dev->DrawPrimitive(D3DPT_POINTLIST, 0, 1);
	}
}
