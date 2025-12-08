#include "Window.h"

namespace HydrogenViewer
{
	Window::Window():
		m_Window(NULL), m_Width(1000), m_Height(800), m_Title("Test")
	{
	}
	
	Window::Window(int32 pWidth, int32 pHeight, const char * pTitle):
		m_Window(NULL), m_Width(pWidth), m_Height(pHeight), m_Title(pTitle)
	{
		MakeWindow(m_Width, m_Height, m_Title);
	}
	
	Window::~Window()
	{
		DestroyWindow();
	}
	
	bool Window::MakeWindow(int32 pWidth, int32 pHeight, const char * pTitle)
	{
		if (m_Window != NULL)
			return HYD_WINDOW_FAILD;

		m_Window = glfwCreateWindow(pWidth, pHeight, pTitle, NULL, NULL);
			
		if (m_Window == NULL)
		{
			return HYD_WINDOW_FAILD;
		}
		glfwMakeContextCurrent(m_Window);
		return HYD_TRUE;
	}
	
	bool Window::DestroyWindow()
	{
		glfwDestroyWindow(m_Window);
		m_Window = NULL;
		return HYD_TRUE;
	}

	void Window::ProcessWindow()
	{
		glfwSwapBuffers(m_Window);
	}

};

