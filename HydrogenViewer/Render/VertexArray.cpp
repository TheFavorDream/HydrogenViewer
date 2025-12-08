#include "VertexArray.h"

namespace HydrogenViewer
{

	VertexArray::~VertexArray()
	{
		DestroyVertexArray();
	}

	void VertexArray::CreateVertexArray()
	{
		glCreateVertexArrays(0, &m_VertexArray);
		Bind();
	}

	void VertexArray::DestroyVertexArray()
	{
		Unbind();
		glDeleteVertexArrays(0, &m_VertexArray);
	}

	void VertexArray::AddLayout(GLenum pType, uint16 pCount, GLboolean pNormalized, uint32 Offset)
	{
		//Add the Layout Configs to the Vector:
		m_Layouts.push_back({pCount, pType, pNormalized, Offset});
	}

	void VertexArray::EnableLayouts(const VertexBuffer & pVertexBuffer)
	{
		pVertexBuffer.Bind();
		Bind();
		int stride = 0;
		for (int i = 0; i < m_Layouts.size(); i++)
		{
			stride = sizeof(float) * m_Layouts[i].Count;
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, m_Layouts[i].Count, m_Layouts[i].Type, m_Layouts[i].Normalized, stride, (void*)m_Layouts[i].Offset);
		}
	}

	void VertexArray::DisableLayouts(const VertexBuffer & pVertexBuffer)
	{
		for (int i = 0; i < m_Layouts.size(); i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_VertexArray);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

};