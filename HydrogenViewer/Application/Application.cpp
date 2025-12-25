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
			std::cout << "Something Went Wrong, Kill yourself \n";
			std::cin.get();
		}
	}

	Application::~Application()
	{
		Hydrogen::Loader::Free(&m_Model);
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

		Hydrogen::Loader::SetUpHydrogen(Hydrogen::OPENGL);
		m_Model = Hydrogen::Loader::Load("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Models\\Cone.gltf");

		if (!m_Model)
		{
			std::cout << "Failed to Load the Model\n";
		}

		m_Shader.CreateShader("C:\\Users\\TheVoltage\\Desktop\\Dev\\Hydrogen\\HydrogenViewer\\Resources\\Shaders\\base.glsl");

		m_Camera.SetupCamera(45.0f, glm::vec3(0.0f, 0.0f, 3.0f), 2, 0.01, 800.0f);
		m_IsRunning = true;


		//ImGui Setup
		//IMGUI_CHECKVERSION();
		//ImGui::CreateContext();
		//
		//ImGui_ImplGlfw_InitForOpenGL(m_Window.GetWindow(), true);
		//ImGui_ImplOpenGL3_Init("#version 330");

	}

	void Application::Event()
	{

		//if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse)
		//{
		//	m_Camera.DisableCameraMovement(true);
		//}
		//else
		//{
		//	m_Camera.DisableCameraMovement(false);
		//}

		m_Camera.HandleCameraMovement();
		m_Camera.HandleCameraLooking();

		glfwPollEvents();
		m_IsRunning = !glfwWindowShouldClose(m_Window.GetWindow());
	}

	void Application::Update()
	{
		glm::mat4 Model = glm::mat4(1.0f);

		Model = glm::scale(Model, glm::vec3(0.1f));
		Model = glm::rotate(Model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		Model = glm::translate(Model, glm::vec3(3.0f, 0.0f, -3.0f));

		m_Shader.SetUniformMat4("Model", Model);
		m_Shader.SetUniformMat4("View", m_Camera.GetView());
		m_Shader.SetUniformMat4("Projection", m_Camera.GetProjection());

	}

	void Application::Render()
	{
		glClearColor(0.2f, 0.1f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader.Bind();
		m_Model->RenderScene();

		//RenderUI();

		m_Window.ProcessWindow();
	}

	void Application::RenderUI()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Hello, World");
		ImGui::Text("Emma Myers is my crush now");
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

};