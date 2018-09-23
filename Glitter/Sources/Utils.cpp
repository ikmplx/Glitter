// ¯\_(ツ)_/¯
#include "stdafx.h"

#include <random>

#include "Utils.h"
#include <ctime>

namespace MyGL
{
	static std::mt19937 sGen;
	static bool sGenInit = false;

	static void Randomize()
	{
		if (!sGenInit) {
			sGenInit = true;

			sGen.seed((int) std::time(nullptr));
		}
	}

	int Math::Random(int minIncl, int maxIncl)
	{
		Randomize();

		return std::uniform_int_distribution<int>(minIncl, maxIncl)(sGen);
	}
}

void MyGL::Utils::GlError(const char * file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: error = "UNKNOWN_ERROR"; break;
		}
		std::cerr << error << " | " << file << " (" << line << ")" << std::endl;
	}
}

void MyGL::Utils::GlClearError()
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
		;
}

#if defined _MSC_VER
#include <intrin.h>
void MyGL::Utils::Assert(bool assertion)
{
	if (!assertion) {
		__debugbreak();
	}
}

#endif
