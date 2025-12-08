#pragma once

#include "Common.h"
#include "glew.h"

namespace HydrogenViewer
{


	class VertexBuffer
	{
	public:

		VertexBuffer() = default;
		VertexBuffer(const std::vector<char>& pData);
		VertexBuffer(const std::vector<float>& pPosition, const std::vector<float>& pNormals, const std::vector<float>& pColor);
	   ~VertexBuffer();

		void operator=(VertexBuffer&& Other) noexcept;

		void InitBuffer(const std::vector<float>& pPosition, const std::vector<float>& pNormals, const std::vector<float>& pColor);
		void InitBuffer(const std::vector<char>& pData);
		void FreeBuffer();

		void Bind()   const;
		void Unbind() const;

		inline uint32 GetID() { return m_BufferID; }
		inline uint32 GetVertexNumbers() { return m_Vertices; }
	private:
		uint32 m_BufferID = 0;
		uint32 m_Vertices = 0;
	};
};