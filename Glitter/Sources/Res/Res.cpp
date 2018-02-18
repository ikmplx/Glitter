#include "stdafx.h"

#include "Res.h"

#include "Shader.h"
#include "Texture.h"

namespace MyGL
{
	Resource::Resource(std::string name, ResourceType resourceType)
		: _type(resourceType)
		, _name(name)
	{
	}

	Resource::~Resource()
	{
	}

	ResourceType Resource::GetType() const
	{
		return _type;
	}

	std::string Resource::GetName() const
	{
		return _name;
	}

	bool ResourceManager::ResourceDescription::operator==(const ResourceDescription & desc) const
	{
		return desc.type == type && desc.name == desc.name;
	}

	size_t ResourceManager::ResourceHasher::operator()(const ResourceDescription & desc) const
	{
		size_t hash1 = std::hash_value(desc.type);
		size_t hash2 = std::hash_value(desc.name);

		return hash1 * 37 + hash2;
	}


	ResourceManager* ResourceManager::_sInstance;

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	void ResourceManager::Initialize()
	{
		_sInstance = new ResourceManager();

		auto res = _sInstance;

		res->AddShader("test", "test");

		res->AddTexture("Crate", "container.jpg");
		res->AddTexture("Awesome", "awesomeface.png");
	}

	void ResourceManager::Deinitialize()
	{
		if (_sInstance) {
			delete _sInstance;
			_sInstance = nullptr;
		}
	}

	ResourceManager* ResourceManager::Instance()
	{
		return _sInstance;
	}

	ShaderPtr ResourceManager::AddShader(const std::string& name, const std::string& baseName)
	{
		ShaderPtr shader = std::make_shared<Shader>(name, std::string("Shaders/") + baseName + std::string(".vert"), std::string("Shaders/") + baseName + std::string(".frag"));
		Add(std::static_pointer_cast<Resource>(shader));
		return shader;
	}

	ShaderPtr ResourceManager::GetShader(const std::string & name)
	{
		return std::static_pointer_cast<Shader>(Find(ResourceType::Shader, name));
	}

	TexturePtr ResourceManager::AddTexture(const std::string & name, const std::string & baseName)
	{
		TexturePtr texture = std::make_shared<Texture>(name, std::string("Textures/") + baseName);
		Add(std::static_pointer_cast<Texture>(texture));
		return texture;
	}

	TexturePtr ResourceManager::GetTexture(const std::string & name)
	{
		return std::static_pointer_cast<Texture>(Find(ResourceType::Texture, name));
	}

	void ResourceManager::Add(ResourcePtr res)
	{
		_resources[ResourceDescription{ res->GetType(), res->GetName() }] = res;
	}

	ResourcePtr ResourceManager::Find(ResourceType type, const std::string & name)
	{
		ResourceDescription desc{ type, name };
		auto iter = _resources.find(desc);
		if (iter != _resources.end()) {
			return iter->second;
		}
		else {
			std::cerr << "Resource with type " << (int) type << " and name " << name << " not found\n";
			return nullptr;
		}
	}
}