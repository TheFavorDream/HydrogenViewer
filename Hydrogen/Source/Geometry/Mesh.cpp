#include "Mesh.h"

namespace Hydrogen
{

	Mesh::Mesh(std::string pName, std::vector<Primitive>& pPrimitives, uint32 pDefaultPrimitive)
	{
		m_Name = pName;
		m_DefaultPrimitive = pDefaultPrimitive;
		m_Primitives = std::move(pPrimitives);
	}

	Mesh::Mesh(Mesh && Other)
	{
		m_Name = Other.m_Name;
		m_DefaultPrimitive = Other.m_DefaultPrimitive;
		m_Primitives = std::move(Other.m_Primitives);

		Other.m_Primitives.clear();
		Other.m_Name.clear();
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::Render(Model* pModel)
	{

		Buffer& VBO = pModel->m_Buffers[m_Primitives[m_DefaultPrimitive].VboID];
		Buffer& EBO = pModel->m_Buffers[m_Primitives[m_DefaultPrimitive].EboID];
		VertexArray& VAO = pModel->m_VertexArrays[m_Primitives[m_DefaultPrimitive].VaoID];

		VAO.Bind();
		VBO.Bind();
		EBO.Bind();

		GL_CALL(glDrawElements(m_Primitives[m_DefaultPrimitive].RenderingMode, EBO.GetCount(), GL_UNSIGNED_SHORT, 0));
		
		EBO.Unbind();
		VBO.Unbind();
		VAO.Unbind();
	}

};