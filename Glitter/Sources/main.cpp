#include "stdafx.h"

#include "Res/Shader.h"
#include "Res/Texture.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Scene/Scene.h"

#include "imgui_impl_glfw_gl3.h"

// Define Some Constants
const int mWidth = 1280;
const int mHeight = 800;

static float guyRotation = 0.f;

static MyGL::EntityPtr nanosuitPrefab;
static MyGL::EntityPtr towerPrefab;
static MyGL::ScenePtr scene;

static MyGL::EntityPtr centerEntity;
static MyGL::EntityPtr nanosuitEntity1;
static MyGL::EntityPtr nanosuitEntity2;
static MyGL::EntityPtr towerEntity;

static void PrepareBuffers()
{
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

	glm::mat4 view = glm::translate(glm::mat4(1), glm::vec3(0.f, -7.f, -37.f));
	glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)mWidth / mHeight, 1.f, 100.f);

	//nanosuitEntity1->scale = nanosuitEntity2->scale = glm::vec3(pulseProgress);

	centerEntity->rotation = glm::angleAxis(0.f, glm::vec3(1, 0, 0));

	centerEntity->rotation *= glm::angleAxis(constantProgress, glm::vec3(0, 1, 0));
	centerEntity->rotation *= glm::angleAxis(guyRotation, glm::vec3(1, 0, 0));

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

	PrepareBuffers();

	glEnable(GL_DEPTH_TEST);

	ImGui_ImplGlfwGL3_Init(mWindow, true);

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false) {
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mWindow, true);

		ImGui_ImplGlfwGL3_NewFrame();

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
