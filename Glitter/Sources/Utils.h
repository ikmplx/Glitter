// ¯\_(ツ)_/¯

#pragma once

namespace MyGL
{
	struct Component;

	namespace Utils
	{
		template<typename T2>
		struct TypeIdCounter
		{
			static int counter;
		};

		template<typename T2>
		int TypeIdCounter<T2>::counter;

		extern template struct TypeIdCounter<MyGL::Component>;

		template<typename T, typename T2>
		struct TypeId
		{
			static int GetId()
			{
				static int counter = TypeIdCounter<T2>::counter++;
				return counter;
			}
		};

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

	namespace Math
	{
		int Random(int minIncl, int maxIncl);
	}
}

#define GL_ERROR() ::MyGL::Utils::GlError(__FILE__, __LINE__)
#define GL_CLEAR_ERROR() ::MyGL::Utils::GlClearError()

#define MyAssert(x) MyGL::Utils::Assert(x)
