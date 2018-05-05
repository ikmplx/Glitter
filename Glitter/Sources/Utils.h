// ¯\_(ツ)_/¯

#pragma once

namespace MyGL
{
	namespace Utils
	{
		inline constexpr size_t HashCombine()
		{
			return 0;
		}

		template <typename T, typename... Rest>
		size_t HashCombine(const T& val, Rest... rest)
		{
			size_t seed = HashCombine(rest...);
			return std::hash<T>()(val) + (seed << 6) + (seed << 16) - seed;
		}

		void GlError(const char *file, int line);
		void GlClearError();

		void Assert(bool assertion);
	}
}

#define GL_ERROR() ::MyGL::Utils::GlError(__FILE__, __LINE__)
#define GL_CLEAR_ERROR() ::MyGL::Utils::GlClearError()

#define MyAssert(x) MyGL::Utils::Assert(x)