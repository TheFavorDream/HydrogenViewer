#pragma once

#include "GLFW/glfw3.h"
#include "Common.h"

namespace HydrogenViewer
{
	class Window
	{
	public:
		
		Window();
		Window(int32 pWidth, int32 pHeight, const char* pTitle);
		~Window();


		bool MakeWindow(int32 pWidth, int32 pHeight, const char* pTitle);
		bool DestroyWindow();


		void ProcessWindow();

		inline GLFWwindow* GetWindow() { return m_Window; }
		inline int32 GetWidth() { return m_Width; }
		inline int32 GetHeight() { return m_Height; }


	private:
		int32 m_Width, m_Height;
		const char* m_Title;
		GLFWwindow* m_Window;
	};
};