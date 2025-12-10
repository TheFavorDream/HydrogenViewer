#pragma once

#include "Common.h"
#include "glew.h"
#include "Window.h"
#include "Render/VertexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/Shader.h"
#include "Event/Keyboard.h"
#include "Event/Mouse.h"
#include "Camera/Camera.h"

namespace HydrogenViewer
{
	class Application
	{
	public:

		 Application();
		~Application();

		void Run();

	private:
		void Setup();
		void Event();
		void Update();
		void Render();

	private:




		Window m_Window;

		Shader m_Shader;
		VertexArray m_VertexArray;
		VertexBuffer m_VertexBuffer;

		Camera m_Camera;

		bool m_IsRunning;
	};
};