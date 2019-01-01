// ¯\_(ツ)_/¯

#pragma once
#include "Utils.h"

namespace MyGL
{
	template <typename T2>
	struct Array2DRef
	{
		T2* data;
		size_t sizeY;
		size_t x;

		T2& operator[](size_t y)
		{
			MyAssert(y < sizeY);
			MyAssert(y >= 0);
			return data[y + x * sizeY];
		}

		const T2& operator[](size_t y) const
		{
			MyAssert(y < sizeY);
			MyAssert(y >= 0);
			return data[y + x * sizeY];
		}
	};

	template<typename T>
	class Array2D
	{
		std::vector<T> _data;

		size_t _sizeX = 0;
		size_t _sizeY = 0;

	public:
		Array2D() = default;
		Array2D(size_t sizeX, size_t sizeY, T def = T())
		{
			Resize(sizeX, sizeY, def);
		}

		size_t SizeX() const
		{
			return _sizeX;
		}

		size_t SizeY() const
		{
			return _sizeY;
		}

		void ResizeMin(size_t sizeX, size_t sizeY, T def = T())
		{
			if (sizeX < _sizeX && sizeY < _sizeY) {
				return;
			}

			Resize(std::max(sizeX, _sizeX), std::max(sizeY, _sizeY));
		}

		void Resize(size_t sizeX, size_t sizeY, T def = T())
		{
			std::vector<T> newData;

			newData.resize(sizeX * sizeY, def);

			// TODO: reallications that shrink _data can be made without creating new vector
			for (size_t x = 0; x < std::min(sizeX, _sizeX); x++) {
				for (size_t y = 0; y < std::min(sizeY, _sizeY); y++) {
					newData[y + x * sizeY] = std::move(_data[y + x * _sizeY]);
				}
			}

			_sizeX = sizeX;
			_sizeY = sizeY;
			_data = std::move(newData);
		}

		T& SafeRef(size_t x, size_t y)
		{
			ResizeMin(x + 1, y + 1);
			return operator[](x)[y];
		}

		Array2DRef<T> operator[](size_t x) {
			MyAssert(x < _sizeX);
			MyAssert(x >= 0);
			return Array2DRef<T>{_data.data(), _sizeY, x};
		}

		Array2DRef<const T> operator[](size_t x) const {
			MyAssert(x < _sizeX);
			MyAssert(x >= 0);
			return Array2DRef<const T>{_data.data(), _sizeY, x};
		}

		/*
		T* operator[](size_t x)
		{
			return (_data.data() + x * _sizeY);
		}
		*/


	private:
	};
}
