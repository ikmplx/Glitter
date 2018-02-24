#include "stdafx.h"

#include "Res/Shader.h"
#include "Res/Texture.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Camera.h"

#include "imgui_impl_glfw_gl3.h"

// Define Some Constants
const int mWidth = 1280;
const int mHeight = 800;

static float guyRotation = 0.f;

static MyGL::Camera camera;

static MyGL::EntityPtr nanosuitPrefab;
static MyGL::EntityPtr towerPrefab;
static MyGL::ScenePtr scene;

static MyGL::EntityPtr centerEntity;
static MyGL::EntityPtr nanosuitEntity1;
static MyGL::EntityPtr nanosuitEntity2;
static MyGL::EntityPtr towerEntity;

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

	camera.MouseMoved(dx, dy);
}

static void MyProcessInput(GLFWwindow* window)
{
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


static void PrepareBuffers()
{
	camera.SetPosition(glm::vec3(0.f, 7.f, 37.f));

	nanosuitPrefab = MyGL::ModelLoader::LoadModel("Models/nanosuit/nanosuit.obj");

	towerPrefab = MyGL::ModelLoader::LoadModel("Models/vox/chr_sword.ply");
	towerPrefab->rotation = glm::angleAxis(glm::radians(-90.f), glm::vec3(1, 0, 0));

	scene = std::make_shared<MyGL::Scene>();

	centerEntity = scene->CreateEntity();

	nanosuitEntity1 = nanosuitPrefab->Clone();
	nanosuitEntity2 = nanosuitPrefab->Clone();

	nanosuitEntity1->position += glm::vec3(-10, 0, 0);
	nanosuitEntity2->position += glm::vec3(10, 0, 0);

	towerEntity = towerPrefab->Clone();
	towerEntity->position += glm::vec3(0, 0, -10);

	centerEntity->AddChild(nanosuitEntity1);
	centerEntity->AddChild(nanosuitEntity2);
	centerEntity->AddChild(towerEntity);
}

static void DrawBuffers()
{
	float pulseProgress = 0.75f + 0.25f * (float)sin(glfwGetTime());
	float constantProgress = (float) fmod(glfwGetTime(), 100.0) * 4.f;

	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)mWidth / mHeight, 1.f, 100.f);

	//nanosuitEntity1->scale = nanosuitEntity2->scale = glm::vec3(pulseProgress);

	//centerEntity->rotation = glm::angleAxis(0.f, glm::vec3(1, 0, 0));

	//centerEntity->rotation *= glm::angleAxis(constantProgress, glm::vec3(0, 1, 0));
	//centerEntity->rotation *= glm::angleAxis(guyRotation, glm::vec3(1, 0, 0));

	//centerEntity->scale = glm::vec3(pulseProgress);

	auto shader = MyGL::ResourceManager::Instance()->GetShader("test");
	shader->Bind();
	shader->SetMatrix("view", view);
	shader->SetMatrix("proj", proj);

	scene->Draw(shader);
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
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	MyGL::ResourceManager::Instance()->Initialize();

	glViewport(0, 0, mWidth, mHeight);

	glfwSetMouseButtonCallback(mWindow, MyMouseButtonCallback);
	glfwSetScrollCallback(mWindow, MyScrollCallback);
	glfwSetKeyCallback(mWindow, MyKeyCallback);
	glfwSetCharCallback(mWindow, MyCharCallback);
	glfwSetCursorPosCallback(mWindow, MyCursorPosCallback);

	PrepareBuffers();

	glEnable(GL_DEPTH_TEST);

	ImGui_ImplGlfwGL3_Init(mWindow, false);

	double lastTime = glfwGetTime();

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

		// Background Fill Color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawBuffers();

		ImGui::SliderFloat("Rotate", &guyRotation, 0.f, glm::two_pi<float>());

		ImGui::Render();

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	ImGui_ImplGlfwGL3_Shutdown();

	MyGL::ResourceManager::Instance()->Deinitialize();

	glfwTerminate();
	return EXIT_SUCCESS;
}
