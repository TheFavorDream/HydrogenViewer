#pragma once

#include "Common.h"
#include "glew.h"
#include "VertexBuffer.h"

namespace HydrogenViewer
{
	struct Layout
	{
		uint16    Count;
		GLenum	  Type;
		GLboolean Normalized;
		uint32 Offset;
	};

	class VertexArray
	{
	public:

		 VertexArray() = default;
		~VertexArray();


		void CreateVertexArray();
		void DestroyVertexArray();

		void AddLayout(GLenum pType, uint16 pCount, GLboolean pNormalized, uint32 Offset=0);
		void EnableLayouts(const VertexBuffer& pVertexBuffer);
		void DisableLayouts(const VertexBuffer& pVertexBuffer);

		void Bind() const;
		void Unbind() const;


	private:
		uint32 m_VertexArray = 0;
		std::vector<Layout> m_Layouts;
	};

};