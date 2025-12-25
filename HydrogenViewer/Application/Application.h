#pragma once

#include "Common.h"
#include "Hydrogen.h"
#include "Window.h"
#include "Event/Keyboard.h"
#include "Event/Mouse.h"
#include "Camera/Camera.h"
#include "Render/Shader.h"

//ImGui (temp)
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

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
		void RenderUI();

	private:




		Window m_Window;

		Shader m_Shader;
		Camera m_Camera;

		Hydrogen::Model* m_Model = NULL;

		bool m_IsRunning;
	};
};