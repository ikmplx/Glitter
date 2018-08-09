#include "stdafx.h"

#include <gtest/gtest.h>

#include "Forward.h"

#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/System.h"
#include "Scene/Component.h"

TEST(SceneTest, SingleEntity)
{
	auto scene = std::make_shared<MyGL::Scene>();
	auto entity = scene->CreateEntity();
}

TEST(SceneTest, EntityPosition)
{
	auto scene = std::make_shared<MyGL::Scene>();
	
	auto entity = std::make_shared<MyGL::Entity>();
	entity->position = glm::vec3(1, 0, 2);
	scene->AddEntity(entity);
	
	auto p = entity->GetGlobalPosition();

	EXPECT_FLOAT_EQ(1, p.x);
	EXPECT_FLOAT_EQ(0, p.y);
	EXPECT_FLOAT_EQ(2, p.z);
}

TEST(SceneTest, EntityPositionHier)
{
	auto scene = std::make_shared<MyGL::Scene>();

	auto entity = std::make_shared<MyGL::Entity>();
	entity->position = glm::vec3(1, 0, 2);

	auto entity2 = std::make_shared<MyGL::Entity>();
	entity2->position = glm::vec3(5, 5, 5);

	entity->AddChild(entity2);

	scene->AddEntity(entity);

	auto p = entity2->GetGlobalPosition();

	EXPECT_FLOAT_EQ(6, p.x);
	EXPECT_FLOAT_EQ(5, p.y);
	EXPECT_FLOAT_EQ(7, p.z);
}

TEST(SceneTest, EntityPositionManualSet)
{
	auto scene = std::make_shared<MyGL::Scene>();

	auto entity = std::make_shared<MyGL::Entity>();
	entity->position = glm::vec3(0, 0, 0);
	scene->AddEntity(entity);

	entity->GetGlobalPosition();

	entity->position = glm::vec3(1, 0, 2);
	entity->InvalidateTransform();

	auto p = entity->GetGlobalPosition();

	EXPECT_FLOAT_EQ(1, p.x);
	EXPECT_FLOAT_EQ(0, p.y);
	EXPECT_FLOAT_EQ(2, p.z);
}

TEST(SceneTest, EntityPositionRotate)
{
	auto scene = std::make_shared<MyGL::Scene>();

	auto entity = std::make_shared<MyGL::Entity>();
	entity->position = glm::vec3(0, 0, 0);
	entity->rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));

	auto entity2 = std::make_shared<MyGL::Entity>();
	entity2->position = glm::vec3(1, 0, 0);
	entity2->rotation = glm::angleAxis(1.5f, glm::vec3(1, 1, 1));

	entity->AddChild(entity2);

	scene->AddEntity(entity);

	auto p = entity2->GetGlobalPosition();

	EXPECT_NEAR(-1, p.x, 0.0001);
	EXPECT_NEAR(0, p.y, 0.0001);
	EXPECT_NEAR(0, p.z, 0.0001);
}