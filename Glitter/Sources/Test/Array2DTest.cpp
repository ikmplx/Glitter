// ¯\_(ツ)_/¯

#include "stdafx.h"

#include <gtest/gtest.h>
#include "Const.h"
#include "Array2D.h"

TEST(Array2DTest, EmptyArray)
{
	// Arrange
	MyGL::Array2D<int> arr;

	// Act
	// Assert
	EXPECT_EQ(0, arr.SizeX());
	EXPECT_EQ(0, arr.SizeY());
}


TEST(Array2DTest, ZeroX)
{
	// Arrange
	MyGL::Array2D<int> arr(0, 2);

	// Act
	// Assert
	EXPECT_EQ(0, arr.SizeX());
	EXPECT_EQ(2, arr.SizeY());
}

TEST(Array2DTest, ZeroY)
{
	// Arrange
	MyGL::Array2D<int> arr(2, 0);

	// Act
	// Assert
	EXPECT_EQ(2, arr.SizeX());
	EXPECT_EQ(0, arr.SizeY());
}

TEST(Array2DTest, ZeroXResize)
{
	// Arrange
	MyGL::Array2D<int> arr(0, 0);

	// Act
	arr.Resize(0, 10);

	// Assert
	EXPECT_EQ(10, arr.SizeY());
}

TEST(Array2DTest, ZeroYResize)
{
	// Arrange
	MyGL::Array2D<int> arr(0, 0);

	// Act
	arr.Resize(10, 0);

	// Assert
	EXPECT_EQ(10, arr.SizeX());
}

TEST(Array2DTest, SimpleArray2x3)
{
	// Arrange
	MyGL::Array2D<int> arr(2, 3);

	// Act
	int c = 0;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 3; y++) {
			arr[x][y] = c++;
		}
	}

	// Assert
	int e = 0;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 3; y++) {
			EXPECT_EQ(e, arr[x][y]);
			e++;
		}
	}
}

TEST(Array2DTest, ResizeLarge)
{
	// Arrange
	MyGL::Array2D<int> arr(2, 3);

	int c = 0;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 3; y++) {
			arr[x][y] = c++;
		}
	}

	// Act
	arr.Resize(3, 4, 100500);

	// Assert
	EXPECT_EQ(3, arr.SizeX());
	EXPECT_EQ(4, arr.SizeY());

	int e = 0;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 4; y++) {
			if (x < 2 && y < 3) {
				EXPECT_EQ(e, arr[x][y]);
				e++;
			} else {
				EXPECT_EQ(100500, arr[x][y]);
			}
		}
	}
}

TEST(Array2DTest, ResizeCross)
{
	// Arrange
	MyGL::Array2D<int> arr(2, 3);

	int c = 0;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 3; y++) {
			arr[x][y] = c++;
		}
	}

	// Act
	arr.Resize(3, 2, 100500);

	// Assert
	EXPECT_EQ(3, arr.SizeX());
	EXPECT_EQ(2, arr.SizeY());

	EXPECT_EQ(0, arr[0][0]);
	EXPECT_EQ(1, arr[0][1]);
	EXPECT_EQ(3, arr[1][0]);
	EXPECT_EQ(4, arr[1][1]);
	EXPECT_EQ(100500, arr[2][0]);
	EXPECT_EQ(100500, arr[2][1]);
}
