#include "Application.h"

namespace HydrogenViewer
{

	Application::Application()
	{
		if (glfwInit() == GLFW_FALSE)
		{
			std::cout << "Unable to Init GLFW";
		}

		m_Window.MakeWindow(1000, 500, "Hydrogen Viewer");

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Something Went Wrong, Kill yourself ASAP\n";
			std::cin.get();
		}
	}

	Application::~Application()
	{
		m_Window.DestroyWindow();
		glfwTerminate();
	}

	void Application::Run()
	{
		Setup();

		while (m_IsRunning)
		{
			Event();
			Update();
			Render();
		}
	}




	void Application::Setup()
	{

		//Configure the keyboard and keys
		Keyboard::InitKeyboard(m_Window.GetWindow());

		//Configure the Mouse:
		Mouse::InitMouse(m_Window.GetWindow());
		 

		m_Shader.CreateShader("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Shaders\\base.glsl");

		m_VertexArray.CreateVertexArray();
		m_VertexBuffer.InitBuffer(
			{ -0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f},
			{},
			{1.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f , 0.0f, 0.0f, 1.0f , 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f});

		m_VertexArray.AddLayout(GL_FLOAT, 3, GL_FALSE, 0);
		m_VertexArray.AddLayout(GL_FLOAT, 3, GL_FALSE, sizeof(float)*9);
		m_VertexArray.EnableLayouts(m_VertexBuffer);


		m_Camera.SetupCamera(45.0f, glm::vec3(0.0f, 0.0f, 3.0f), float(m_Window.GetWidth()/m_Window.GetHeight()));
		m_IsRunning = true;
	}

	void Application::Event()
	{

		m_Camera.HandleCameraMovement();
		m_Camera.HandleCameraLooking();

		glfwPollEvents();
		m_IsRunning = !glfwWindowShouldClose(m_Window.GetWindow());
	}

	void Application::Update()
	{
		glm::mat4 Model = glm::mat4(1.0f);
		//Model = glm::translate(Model, glm::vec3(0.5f, 0.0f, -0.8f));
		//Model = glm::rotate(Model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//Model = glm::scale(Model, glm::vec3(0.1f));


		m_Shader.SetUniformMat4("Model", Model);
		m_Shader.SetUniformMat4("View", m_Camera.GetView());
		m_Shader.SetUniformMat4("Projection", m_Camera.GetProjection());

	}

	void Application::Render()
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_VertexArray.Bind();
		m_Shader.Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);


		m_Window.ProcessWindow();
	}

};