#include "stdafx.h"

#include "Res/Shader.h"
#include "Res/Texture.h"


// Define Some Constants
const int mWidth = 1024;
const int mHeight = 768;

GLuint vba;

GLuint posVbo;
GLuint colorVbo;

GLuint ibo;

static void PrepareBuffers()
{
	glGenVertexArrays(1, &vba);

	glGenBuffers(1, &posVbo);
	glGenBuffers(1, &colorVbo);

	glGenBuffers(1, &ibo);

	const float vertices[] = {
		-0.5f, -0.5f,  0.f, 0.f,
		0.5f, -0.5f,   1.f, 0.f,
		0.5f, 0.5f,    1.f, 1.f,
		-0.5f, 0.5f,    0.f, 1.f
	};

	const float colors[] = {
		0.f, 1.f, 0.f,
		1.f, 0.f, 1.f,
		1.f, 1.f, 0.f,
		1.f, 0.f, 0.5f
	};



	const int indices[] = {
		0, 1, 2, 3
	};

	glBindVertexArray(vba);
	{
		glBindBuffer(GL_ARRAY_BUFFER, posVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
}

static void DrawBuffers()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindVertexArray(vba);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	auto shader = MyGL::ResourceManager::Instance()->GetShader("Rainbow");
	auto texture = MyGL::ResourceManager::Instance()->GetTexture("Crate");

	glActiveTexture(GL_TEXTURE0);
	texture->Bind();

	shader->Bind();
	shader->SetInt("tex", 0);

	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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

	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false) {
		if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(mWindow, true);

		// Background Fill Color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		DrawBuffers();

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	MyGL::ResourceManager::Instance()->Deinitialize();

	glfwTerminate();
	return EXIT_SUCCESS;
}
