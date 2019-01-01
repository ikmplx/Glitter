// ¯\_(ツ)_/¯

#include "stdafx.h"

#include <gtest/gtest.h>

#include "Scene/IdPool.h"
#include <random>

TEST(IdPoolTest, Alloc)
{
	// Arrange
	MyGL::IdPool pool;

	// Act
	int id = pool.Alloc();

	// Assert
	EXPECT_EQ(0, id);
}

TEST(IdPoolTest, AllocWithPoolGrow)
{
	// Arrange
	MyGL::IdPool pool;

	// Act
	int id1 = pool.Alloc();
	int id2 = pool.Alloc();

	// Assert
	EXPECT_EQ(0, id1);
	EXPECT_EQ(1, id2);
}

TEST(IdPoolTest, LargeAlloc)
{
	// Arrange
	MyGL::IdPool pool;
	std::vector<int> ids;

	// Act
	for (int i = 0; i < 100; i++) {
		ids.push_back(pool.Alloc());
	}

	// Assert
	std::vector<int> idsExpected;
	for (int i = 0; i < 100; i++) {
		idsExpected.push_back(i);
	}

	EXPECT_EQ(idsExpected, ids);
}

TEST(IdPoolTest, Dealloc)
{
	// Arrange
	MyGL::IdPool pool;

	// Act
	pool.Dealloc(pool.Alloc());
	int id = pool.Alloc();

	// Assert
	EXPECT_EQ(0, id);
}