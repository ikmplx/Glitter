// ¯\_(ツ)_/¯

#include "stdafx.h"

#include <gtest/gtest.h>

#include "Forward.h"

#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/System.h"
#include "Scene/Component.h"

struct TestComponent1 : MyGL::Component
{
};

struct TestComponent2 : MyGL::Component
{
};

class TestSystem1 : public MyGL::System
{
public:
	TestSystem1()
		: MyGL::System(typeid(TestComponent1))
	{
	}

	void Update(MyGL::ScenePtr scene, float dt) override
	{
		updatesCount++;
	}

	int updatesCount = 0;
};

class TestSystem12 : public MyGL::System
{
public:
	TestSystem12()
		: MyGL::System({typeid(TestComponent1), typeid(TestComponent2)})
	{
	}

	void Update(MyGL::ScenePtr scene, float dt) override
	{
		updatesCount++;
	}

	int updatesCount = 0;
};


TEST(SystemTest, Component1)
{
	auto scene = std::make_shared<MyGL::Scene>();
	auto system = std::make_shared<TestSystem1>();

	scene->AddSystem(system);

	auto entity = scene->CreateEntity();
	scene->AddComponent(entity, std::make_shared<TestComponent1>());

	for (int i = 0; i < 3; i++) {
		scene->Update(1.f);

		EXPECT_EQ(i + 1, system->updatesCount);
		EXPECT_EQ(1, system->GetEntities().size());
	}
}

TEST(SystemTest, Component2)
{
	auto scene = std::make_shared<MyGL::Scene>();
	auto system = std::make_shared<TestSystem1>();

	scene->AddSystem(system);

	auto entity = scene->CreateEntity();
	scene->AddComponent(entity, std::make_shared<TestComponent1>());
	scene->AddComponent(entity, std::make_shared<TestComponent2>());

	for (int i = 0; i < 3; i++) {
		scene->Update(1.f);

		EXPECT_EQ(i + 1, system->updatesCount);
		EXPECT_EQ(1, system->GetEntities().size());
	}
}

TEST(SystemTest, SystemTwoOneComponent)
{
	auto scene = std::make_shared<MyGL::Scene>();
	auto system = std::make_shared<TestSystem12>();

	scene->AddSystem(system);

	auto entity = scene->CreateEntity();
	scene->AddComponent(entity, std::make_shared<TestComponent1>());

	scene->Update(1.f);

	EXPECT_EQ(1, system->updatesCount);
	EXPECT_EQ(0, system->GetEntities().size());
}

TEST(SystemTest, SystemTwoTwoComponents)
{
	auto scene = std::make_shared<MyGL::Scene>();
	auto system = std::make_shared<TestSystem12>();

	scene->AddSystem(system);

	auto entity = scene->CreateEntity();
	scene->AddComponent(entity, std::make_shared<TestComponent1>());
	scene->AddComponent(entity, std::make_shared<TestComponent2>());

	for (int i = 0; i < 3; i++) {
		scene->Update(1.f);

		EXPECT_EQ(i + 1, system->updatesCount);
		EXPECT_EQ(1, system->GetEntities().size());
	}
}

TEST(SystemTest, DeleteComponent)
{
	auto scene = std::make_shared<MyGL::Scene>();
	auto system = std::make_shared<TestSystem1>();

	scene->AddSystem(system);

	MyGL::ComponentPtr component = std::make_shared<TestComponent1>();
	auto entity = scene->CreateEntity();
	scene->AddComponent(entity, component);
	scene->Update(1.f);

	EXPECT_EQ(1, system->GetEntities().size());

	scene->RemoveComponent(entity, component);
	scene->Update(1.f);

	EXPECT_EQ(0, system->GetEntities().size());
}

