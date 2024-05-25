#pragma once

// stl includes
#include <iostream>
#include <vector>
#include <string>
#include <limits>

// render includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <filesystem>

// imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// font


// project files
#include "shader.h"
#include "camera.h"
#include "textmanager.h"
#include "grid.h"
#include "object.h"

// experimental optional windows no border/transparent
// #ifdef _WIN32
// 	#define GLFW_EXPOSE_NATIVE_WIN32
// 	#include <GLFW/glfw3native.h>
// 	#include <windows.h>
// #endif

class Application
{
public:
	int m_SCR_WIDTH{1280};
	int m_SCR_HEIGHT{720};
	const float m_viewport_ratio{m_SCR_HEIGHT/static_cast<float>(m_SCR_WIDTH)};
	// const float m_viewport_ratio{1};
	int m_VIEW_WIDTH{static_cast<int>(m_SCR_WIDTH*m_viewport_ratio)};
	int m_VIEW_HEIGHT{static_cast<int>(m_SCR_HEIGHT)};

	ImGuiIO* m_ioptr{};

	GLFWwindow* m_window{};

	Shader m_gridShader{};
	Shader m_textShader{};
	Shader m_objShader{};

	Object m_obj{};

	TextManager m_textManager{glm::vec2{m_VIEW_WIDTH, m_VIEW_HEIGHT}};

	Grid m_grid{m_VIEW_WIDTH, m_VIEW_HEIGHT, 25, 25};

	bool mouseDragging{false};
	double lastX{};
	double lastY{};

	glm::vec2 m_mousePos{};
	glm::vec3 m_mousePos3D{};

	bool vsync{true};

	void draw();

	void process_input();

	Camera m_camera{};

	glm::mat4 m_modelViewProjectionMatrix{1.0};
	glm::mat4 m_projection{1.0};
	glm::mat4 m_view{1.0};
	glm::mat4 m_model{1.0};
	glm::mat4 m_translate{1.0};
	glm::mat4 m_rotate{1.0};
	glm::mat4 m_scale{1.0};
	std::vector<glm::mat4*> m_modelViewProjectionComponents{};

	glm::vec2 normalizePoint(double x, double y);
	void updateMousePos3D();

	float m_zoom{1.0f};

public:

	Application();

	void init();

	void process_framebuffer_size(int, int);

	void process_key(int, int, int, int);

	void process_mouse_button(int, int, int);

	void process_cursor_position(double, double);

	void process_scroll(double, double);

	void run();

	void close();

	double lastTime{};
	double currentTime{};
	double deltaTime{};
	double totalTime{};
	double frameTime{};
	double startTime{};

	void updateFrameTime();
};

static void framebuffer_size_callback(GLFWwindow*, int, int);

static void key_callback(GLFWwindow*, int, int, int, int);

static void mouse_button_callback(GLFWwindow*, int, int, int);

static void cursor_position_callback(GLFWwindow* , double, double);

static void scroll_callback(GLFWwindow*, double, double);
