#include "stdafx.h"

#include "Res/Shader.h"
#include "Res/Texture.h"
#include "Model/Mesh.h"


// Define Some Constants
const int mWidth = 1024;
const int mHeight = 768;

static MyGL::MeshPtr cube;

static void PrepareBuffers()
{
	cube = MyGL::Primitives::CreateCube();
}

static void DrawBuffers()
{
	// glDisable(GL_CULL_FACE);


	float angle = glm::two_pi<float>() * std::fmod(glfwGetTime() / 3.0, 1.0);

	glm::mat4 view = glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, -10.f));
	glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(3, 3, 3));
	model = glm::rotate(model, angle, glm::vec3(0.0f, 1.f, 0.f));

	glm::mat4 proj = glm::perspective(glm::radians(45.f), (float)mWidth / mHeight, 1.f, 100.f);

	auto shader = MyGL::ResourceManager::Instance()->GetShader("test");
	shader->Bind();
	shader->SetMatrix("view", view);
	shader->SetMatrix("model", model);
	shader->SetMatrix("proj", proj);

	cube->Draw(shader);
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

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false) {
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mWindow, true);

		// Background Fill Color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawBuffers();

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	MyGL::ResourceManager::Instance()->Deinitialize();

	glfwTerminate();
	return EXIT_SUCCESS;
}
