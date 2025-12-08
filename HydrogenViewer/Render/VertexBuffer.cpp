#include "VertexBuffer.h"

namespace HydrogenViewer
{

	VertexBuffer::VertexBuffer(const std::vector<char>& pData)
	{
		InitBuffer(pData);
	}

	VertexBuffer::VertexBuffer(const std::vector<float>& pPosition, const std::vector<float>& pNormals, const std::vector<float>& pColor)
	{
		InitBuffer(pPosition, pNormals, pColor);
	}

	VertexBuffer::~VertexBuffer()
	{
		FreeBuffer();
	}

	void VertexBuffer::operator=(VertexBuffer && Other) noexcept
	{
		m_BufferID = Other.m_BufferID;
		m_Vertices = Other.m_Vertices;
		Other.m_BufferID = 0;
		Other.m_Vertices = 0;
	}

	void VertexBuffer::InitBuffer(const std::vector<float>& pPosition, const std::vector<float>& pNormals, const std::vector<float>& pColor)
	{
		glGenBuffers(1, &m_BufferID);
		Bind();
		uint32 Size = pPosition.size() + pNormals.size() + pColor.size();
		glBufferData(GL_ARRAY_BUFFER, Size * sizeof(float), NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, pPosition.size()*sizeof(float), (void*)&pPosition[0]);
		glBufferSubData(GL_ARRAY_BUFFER, pPosition.size() * sizeof(float), pNormals.size()* sizeof(float), (void*)&pNormals[0]);
		glBufferSubData(GL_ARRAY_BUFFER, (pPosition.size()+pNormals.size())* sizeof(float), pColor.size() * sizeof(float), (void*)&pColor[0]);


		std::cout << "Position: " << pPosition.size() << "\tColor:" << pColor.size() << '\n';
	}

	void VertexBuffer::InitBuffer(const std::vector<char>& pData)
	{
		glGenBuffers(1, &m_BufferID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, pData.size(), &pData[0], GL_STATIC_DRAW);

	}

	void VertexBuffer::FreeBuffer()
	{
		Unbind();
		glDeleteBuffers(1, &m_BufferID);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

};