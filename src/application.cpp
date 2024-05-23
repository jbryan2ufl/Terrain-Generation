#define STB_TRUETYPE_IMPLEMENTATION

#include "application.h"

void Application::run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		totalTime += deltaTime;

		process_input();
		draw();
		glfwPollEvents();

		lastTime = currentTime;
	}
}

void Application::updateFrameTime()
{
	double elapsedTime{currentTime-startTime};
	if (elapsedTime > 0.1)
	{
		frameTime = deltaTime;
		startTime=currentTime;
	}
}

void Application::draw()
{
	glClearColor(0.1,0.1,0.1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// m_modelViewProjectionMatrix = glm::mat4{1.0f};
	// m_view = m_camera.getViewMatrix();
	// m_projection = glm::perspective(glm::radians(m_camera.m_fov), static_cast<float>(m_VIEW_WIDTH) / m_VIEW_HEIGHT, m_camera.m_nearPlane, m_camera.m_farPlane);
	// m_projection = glm::ortho(-1.0f, +1.0f, -1.0f, +1.0f, 0.1f, 100.0f);

	// m_model = m_translate * m_scale * m_rotate;
	// for (auto& m : m_modelViewProjectionComponents)
	// {
	// 	m_modelViewProjectionMatrix*=*m;
	// }

	m_gridShader.use();
	m_grid.draw();

	// m_objShader.use();
	// m_obj.render();

	updateFrameTime();

	m_textShader.use();
	m_textManager.generateText("Frame Time:"+std::to_string(frameTime)+"\nFPS:"+std::to_string(1.0/frameTime), 0.0f, 0.0f, 0.002f);
	m_textManager.render();


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowBgAlpha(1.0f);
	glfwGetWindowSize(m_window, &m_SCR_WIDTH, &m_SCR_HEIGHT);
	ImGui::SetNextWindowPos(ImVec2{m_SCR_WIDTH*m_viewport_ratio,0});
	ImGui::SetNextWindowSize(ImVec2{m_SCR_WIDTH*(1-m_viewport_ratio),static_cast<float>(m_SCR_HEIGHT)});
	if (ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration))
	{
		if (ImGui::Checkbox("VSync", &vsync))
		{
			glfwSwapInterval(vsync);
		}
	}
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(m_window);
}

Application::Application()
{}

void Application::init()
{
	// glfw initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_SAMPLES, 8);

	// glfw window creation
	m_window = glfwCreateWindow(m_SCR_WIDTH, m_SCR_HEIGHT, "Curve Design", NULL, NULL);

	// check if window is created
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}

	// glfw callback configuration
	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	// start glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize Glad" << std::endl;
		exit(EXIT_FAILURE);
	}

	// glfw mouse capture
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(vsync);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_FRONT);
	glEnable(GL_LINE_SMOOTH);
	glPointSize(10.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// imgui configuration
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	m_ioptr=&io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init();
	
	glfwSetWindowUserPointer(m_window, this);

	glViewport(0, 0, m_SCR_WIDTH*m_viewport_ratio, m_SCR_HEIGHT);

	// experimental optional windows no border/transparent
	// #ifdef _WIN32
	// 	HWND hwnd = (HWND)glfwGetWin32Window(m_window);
	// 	LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
	// 	style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	// 	SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_EX_LAYERED);

	// 	style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	// 	SetWindowLongPtr(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	// 	SetLayeredWindowAttributes(hwnd, RGB(255, 0, 0), 128, LWA_COLORKEY);
	// #endif

	m_gridShader = Shader("../src/grid.vs", "../src/source.fs");
	m_grid.init();
	m_grid.populate();

	m_textShader = Shader("../src/text.vs", "../src/text.fs");
	m_textManager.init("../fonts/slkscr.ttf", 48);

	m_objShader = Shader("../src/object.vs", "../src/object.fs");
	m_obj.init();

	// m_modelViewProjectionComponents.push_back(&m_projection);
	// m_modelViewProjectionComponents.push_back(&m_view);
	// m_modelViewProjectionComponents.push_back(&m_translate);
	// m_modelViewProjectionComponents.push_back(&m_scale);
	// m_modelViewProjectionComponents.push_back(&m_rotate);
}


void Application::process_framebuffer_size(int width, int height)
{
	glViewport(0, 0, width*m_viewport_ratio, height);
}

// FOR SINGLE PRESSES
void Application::process_key(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(m_window, true);
			close();
		}
		if (key == GLFW_KEY_DELETE)
		{
		}
	}
}

void Application::updateMousePos3D()
{
	m_mousePos = normalizePoint(lastX, lastY);

	m_mousePos3D = glm::inverse(m_model) * glm::vec4{m_mousePos, 0, 1};
}

glm::vec2 Application::normalizePoint(double x, double y)
{
	x=x/m_VIEW_WIDTH*2-1;
	y=y/m_VIEW_HEIGHT*2-1;

	return glm::vec2{x, -y};
}

void Application::process_mouse_button(int button, int action, int mods)
{
	if (!m_ioptr->WantCaptureMouse)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouseDragging = true;
			glfwGetCursorPos(m_window, &lastX, &lastY);
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			mouseDragging = false;
		}
	}
}

void Application::process_cursor_position(double xposIn, double yposIn)
{
	if (!m_ioptr->WantCaptureMouse)
	{
		float xpos=static_cast<float>(xposIn);
		float ypos=static_cast<float>(yposIn);

		float xoffset = xpos-lastX;
		float yoffset = ypos-lastY;

		lastX=xpos;
		lastY=ypos;
		updateMousePos3D();

	}
}

void Application::process_scroll(double xoffset, double yoffset)
{
	yoffset > 0 ? m_zoom *= 1.1f : m_zoom /= 1.1f;

	// updateMousePos3D();
	glm::mat4 translateToCenter = glm::translate(glm::mat4(1.0f), glm::vec3{m_mousePos, 0.0f});

	glm::mat4 scale = glm::scale(glm::mat4{1.0}, glm::vec3{m_zoom});

	glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), -glm::vec3{m_mousePos, 0.0f});

	m_scale = translateToCenter * scale * translateBack;
}

void Application::process_input()
{
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// firstMouse=true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
	}
}

void Application::close()
{
	glfwTerminate();
	exit(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_framebuffer_size(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_key(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_mouse_button(button, action, mods);
}

void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_cursor_position(xposIn, yposIn);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* obj = (Application*)glfwGetWindowUserPointer(window);
	obj->process_scroll(xoffset, yoffset);
}
