#include "stdafx.h"

#include <thread>

#include "Res/Shader.h"
#include "Res/Texture.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Camera.h"
#include "Utils.h"

#include "GameState.h"
#include "TestState.h"

#include "imgui_impl_glfw_gl3.h"

// Define Some Constants
const int mWidth = 1280;
const int mHeight = 800;

static std::unique_ptr<MyGL::GameState> sCurrentState;

static bool isCursorInitialized;
static float xCursor, yCursor;
static float deltaTime;

bool isImGuiActive;

static void MyMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (isImGuiActive)
		ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
}

static void MyScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (isImGuiActive)
		ImGui_ImplGlfwGL3_ScrollCallback(window, xoffset, yoffset);

}

static void MyKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (isImGuiActive)
		ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);

	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		isImGuiActive = !isImGuiActive;

		if (isImGuiActive) {
			isCursorInitialized = false;
		}
	}
}

static void MyCharCallback(GLFWwindow* window, unsigned int c)
{
	if (isImGuiActive)
		ImGui_ImplGlfwGL3_CharCallback(window, c);

}

static void MyCursorPosCallback(GLFWwindow*, double x, double y)
{
	if (isImGuiActive) {
		return;
	}

	if (!isCursorInitialized) {
		isCursorInitialized = true;
		xCursor = (float) x;
		yCursor = (float) y;
	}

	float dx = (float)x - xCursor;
	float dy = (float)y - yCursor;

	xCursor = (float) x;
	yCursor = (float) y;

	sCurrentState->GetCamera().MouseMoved(dx, dy);
}

static void MyProcessInput(GLFWwindow* window)
{
	auto& camera = sCurrentState->GetCamera();

	if (!isImGuiActive) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.Move(MyGL::Camera::MoveDirection::Forward, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.Move(MyGL::Camera::MoveDirection::Backward, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.Move(MyGL::Camera::MoveDirection::Left, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.Move(MyGL::Camera::MoveDirection::Right, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			camera.Move(MyGL::Camera::MoveDirection::Up, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			camera.Move(MyGL::Camera::MoveDirection::Down, deltaTime);
	}
}

int main() 
{
	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// glfwWindowHint(GLFW_SAMPLES, 4);

	auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Check for Valid Context
	if (mWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);

	// V-Sync
	glfwSwapInterval(1);

	gladLoadGLLoader((GLADloadproc)(&glfwGetProcAddress));

	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VENDOR));
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_RENDERER));
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	// fprintf(stderr, "OpenGL %s\n", glGetString(GL_EXTENSIONS));

	MyGL::UboManager::Initialize();
	MyGL::ResourceManager::Instance()->Initialize();

	glViewport(0, 0, mWidth, mHeight);

	glfwSetMouseButtonCallback(mWindow, MyMouseButtonCallback);
	glfwSetScrollCallback(mWindow, MyScrollCallback);
	glfwSetKeyCallback(mWindow, MyKeyCallback);
	glfwSetCharCallback(mWindow, MyCharCallback);
	glfwSetCursorPosCallback(mWindow, MyCursorPosCallback);

	sCurrentState = std::make_unique<MyGL::TestState>();
	sCurrentState->SetViewportSize(mWidth, mHeight);
	sCurrentState->Init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	ImGui_ImplGlfwGL3_Init(mWindow, false);

	double lastTime = glfwGetTime();

	struct {
		int frames;
		double lastTime;
		double lastFps;
	} fpsCounter = { 0, lastTime, 0 };

	double frameTimeLimit = 1.0 / 70.0;

	double nextFrameTime = lastTime + frameTimeLimit;
	double frameLimiterActive = 0.0;

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false) {
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mWindow, true);
		double currentTime = glfwGetTime();
		deltaTime = (float)(currentTime - lastTime);
		lastTime = currentTime;

		MyProcessInput(mWindow);

		ImGui_ImplGlfwGL3_NewFrame();
		glfwSetInputMode(mWindow, GLFW_CURSOR, !isImGuiActive ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

		sCurrentState->SetViewportSize(mWidth, mHeight);
		sCurrentState->Update(deltaTime);

		// Background Fill Color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sCurrentState->Draw();

		// FPS
		fpsCounter.frames++;
		double fpsDelta = currentTime - fpsCounter.lastTime;
		if (fpsDelta >= 1.f) {
			fpsCounter.lastFps = fpsCounter.frames / fpsDelta;
			fpsCounter.lastTime = currentTime;
			fpsCounter.frames = 0;
		}

		ImGui::Text("FPS: %.2lf", fpsCounter.lastFps);
		ImGui::Text("Frame limiter: %.2lf", frameLimiterActive);

		ImGui::Render();

		// FPS limiter
		for (;;) {
			double waitTime = nextFrameTime - glfwGetTime();

			if (waitTime <= 0) {
				break;
			}

			//if (waitTime > frameTimeLimit / 10.f) {
				std::this_thread::sleep_for(std::chrono::microseconds((int)(waitTime * 1000000)));
			//}
			frameLimiterActive = glfwGetTime();
		}

		nextFrameTime += frameTimeLimit;

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	ImGui_ImplGlfwGL3_Shutdown();

	sCurrentState->Deinit();
	sCurrentState.reset();

	MyGL::ResourceManager::Instance()->Deinitialize();
	MyGL::UboManager::Deinitialize();

	glfwTerminate();
	return EXIT_SUCCESS;
}
