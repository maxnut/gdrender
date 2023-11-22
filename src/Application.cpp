#include "Application.h"
#include "Input.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

Application* Application::instance;

void glfwSetWindowSizeCallback(GLFWwindow* window, int width, int height)
{
	Application::instance->resolution = {width, height};
}

void Application::init(glm::vec2 resolution, std::string name)
{
	instance = this;
	this->resolution = resolution;

	if (!glfwInit())
	{
		std::cout << "ERROR: could not start GLFW3\n";
		return;
	}

	window = glfwCreateWindow(resolution.x, resolution.y, name.c_str(), glfwGetPrimaryMonitor(), nullptr);
	glViewport(0, 0, resolution.x, resolution.y);

	if (!window)
	{
		std::cout << "ERROR: could not open window with GLFW3\n";
		glfwTerminate();
		return;
	}

	Input::init();

	window = window;
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); 
	//glDepthFunc(GL_LESS);

	glfwWindowHint(GLFW_SAMPLES, 4);

	glClearColor(0, 0, 0, 1.0);

	glfwSetWindowSizeCallback(window, glfwSetWindowSizeCallback);

	glfwSwapInterval(0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 400");

	lastFrame = glfwGetTime();

	setStyle();
}

void Application::setStyle()
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("arial.ttf", 28.0f);
	io.Fonts->Build();
	ImGui_ImplOpenGL3_CreateFontsTexture();
	io.FontDefault = font;

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(5, 5);
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 2.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 0.03f, 0.03f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);

	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.14f, 0.15f, 0.18f, 0.3f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.15f, 0.18f, 0.4f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.14f, 0.15f, 0.18f, 0.5f);

	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.0f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.0f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.80f, 0.0f, 0.61f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 0.30f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.73f);
}

void Application::startUpdate()
{
	while (!glfwWindowShouldClose(window))
	{
		update();

		draw();
	}

	exit();
}

void Application::update()
{
	if (pendingLayer)
	{
		if (currentLayer)
			popLayer();

		currentLayer = pendingLayer;
		pendingLayer = nullptr;
	}

	float currentFrame = glfwGetTime();

	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	totalTime += deltaTime;

	glfwPollEvents();

	Input::update();

	if (currentLayer)
		currentLayer->update();

	Input::postUpdate();
}

void Application::draw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	glClear(GL_COLOR_BUFFER_BIT);

	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	if (currentLayer)
		currentLayer->GUI();

	ImGui::Render();

	if (currentLayer)
		currentLayer->draw();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	glfwSwapBuffers(window);
}

void Application::exit()
{
	if (currentLayer)
		popLayer();

	glfwTerminate();
}

void Application::pushLayer(std::shared_ptr<Layer> layer)
{
	pendingLayer = layer;
}

void Application::popLayer()
{
	currentLayer->exit();
	currentLayer = nullptr;
}