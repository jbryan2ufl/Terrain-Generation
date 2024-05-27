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

	m_windowData->m_view = m_camera.getViewMatrix();

	// m_gridShader.use();
	// m_grid.draw();

	// m_obj.render();

	updateFrameTime();

	std::string frameTimeString{"Frame Time: "+std::to_string(frameTime*1000).substr(0,5)+"ms\n"};
	std::string fpsString{"FPS: "+std::to_string(static_cast<int>(1.0/frameTime))+'\n'};
	std::string mousePosString{"X:"+std::to_string(static_cast<int>(lastX))+" Y:"+std::to_string(static_cast<int>(lastY))};

	m_fpsText.generateText(frameTimeString+fpsString+mousePosString);
	m_fpsText.render();

	m_worldText.render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SetNextWindowBgAlpha(1.0f);
	static int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	ImGui::SetNextWindowPos(ImVec2{width*m_windowData->m_viewRatio,0});
	ImGui::SetNextWindowSize(ImVec2{width*(1-m_windowData->m_viewRatio),height});
	if (ImGui::Begin("Settings", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration))
	{
		if (ImGui::Checkbox("VSync", &vsync))
		{
			glfwSwapInterval(vsync);
		}
		if (ImGui::DragFloat3("Text Position", &m_worldText.m_modelMatrix.m_position[0], 1.0f/10));
		{
			m_worldText.m_modelMatrix.updateAll();
		}
		if (ImGui::DragFloat3("Text Rotation Axis", &m_worldText.m_modelMatrix.m_axis[0], 1.0f/360));
		{
			m_worldText.m_modelMatrix.updateAll();
		}
		if (ImGui::DragFloat("Text Rotation Angle", &m_worldText.m_modelMatrix.m_angle, 1.0f/360))
		{
			m_worldText.m_modelMatrix.updateAll();
		}
		if (ImGui::DragFloat("Text Scale", &m_worldText.m_modelMatrix.m_scaleFactor, 1.0f/360))
		{
			m_worldText.m_modelMatrix.updateAll();
		}
		if (ImGui::DragFloat("Near Plane", &m_camera.m_nearPlane, 0.0001f) ||
			ImGui::DragFloat("Far Plane", &m_camera.m_farPlane, 100.0f) ||
			ImGui::DragFloat("FOV", &m_camera.m_fov, 1.0f) ||
			ImGui::DragFloat3("Camera Position", &m_camera.m_position[0]))
		{
			m_camera.updateCameraVectors();
			m_windowData->m_perspective = glm::perspective(glm::radians(m_camera.m_fov), m_windowData->m_aspectRatio, m_camera.m_nearPlane, m_camera.m_farPlane);
		}
	}
	ImGui::End();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(m_window);
}

Application::Application()
{
}

void Application::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_SAMPLES, 8);

	m_window = glfwCreateWindow(m_windowData->m_windowSize.x, m_windowData->m_windowSize.y, "3D Template", NULL, NULL);

	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(m_window);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
	glfwSetScrollCallback(m_window, scroll_callback);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize Glad" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LESS);
	glfwSwapInterval(vsync);
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);
	// glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_CLAMP);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	m_ioptr=&io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init();
	
	glfwSetWindowUserPointer(m_window, this);

	glViewport(0, 0, m_windowData->m_windowSize.x*m_windowData->m_viewRatio, m_windowData->m_windowSize.y);

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

	std::shared_ptr<WindowData> w{m_windowData};

	m_windowData->m_view = m_camera.getViewMatrix();
	m_windowData->m_perspective = glm::perspective(glm::radians(m_camera.m_fov), m_windowData->m_aspectRatio, m_camera.m_nearPlane, m_camera.m_farPlane);
	m_windowData->m_orthographic = glm::ortho(0.0f, static_cast<float>(m_windowData->m_viewSize.x), static_cast<float>(m_windowData->m_viewSize.y), 0.0f, m_camera.m_nearPlane, m_camera.m_farPlane);

	m_gridShader = Shader("../src/grid.vs", "../src/source.fs");
	m_grid.init();
	m_grid.populate();

	m_fpsText.init(w, "../fonts/slkscr.ttf", 32.0f, glm::vec3{0.0f, 0.0f, 0.0f}, "../src/text.vs", "../src/text.fs", TextViewingMode::Orthographic, TextAnchor::TopLeft, TextJustification::Left);
	m_worldText.init(w, "../fonts/Arial.ttf", 96.0f, glm::vec3{0.0f, 0.0f, 0.0f}, "../src/text.vs", "../src/text.fs", TextViewingMode::Perspective, TextAnchor::Center, TextJustification::Center);
	m_worldText.generateText("HELLO!");

	m_obj.init(w);
}


void Application::process_framebuffer_size(int width, int height)
{
	glViewport(0, 0, width*m_windowData->m_viewRatio, height);
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
		if (key == GLFW_KEY_LEFT)
		{
			m_camera.m_yaw+=90.0f;
			m_camera.updateCameraVectors();
		}
	}
}

void Application::updateMousePos3D()
{
	m_mousePos = normalizePoint(lastX, lastY);

	// m_mousePos3D = glm::inverse(m_model) * glm::vec4{m_mousePos, 0, 1};
}

glm::vec2 Application::normalizePoint(double x, double y)
{
	x=x/m_windowData->m_viewSize.x*2-1;
	y=y/m_windowData->m_viewSize.y*2-1;

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
	yoffset > 0 ? m_worldText.m_modelMatrix.m_scaleFactor *= 1.1f : m_worldText.m_modelMatrix.m_scaleFactor /= 1.1f;

	// updateMousePos3D();
	glm::mat4 translateToCenter = glm::translate(glm::mat4{1.0f}, glm::vec3{m_mousePos, 0.0f});

	glm::mat4 scale = glm::scale(glm::mat4{1.0}, glm::vec3{m_worldText.m_modelMatrix.m_scaleFactor});

	glm::mat4 translateBack = glm::translate(glm::mat4{1.0f}, -glm::vec3{m_mousePos, 0.0f});

	glm::mat4 finalScale{translateToCenter * scale * translateBack};

	m_worldText.m_modelMatrix.m_scaleFactor = finalScale[0][0];
	m_worldText.m_modelMatrix.updateAll();
}

void Application::process_input()
{
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// firstMouse=true;
	}
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		
	}

	if (mouseDragging)
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
