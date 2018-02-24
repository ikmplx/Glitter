#include "stdafx.h"

#include "Res.h"

#include "Shader.h"
#include "Texture.h"
#include "Utils.h"

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
		return desc.type == type && desc.name == name;
	}

	size_t ResourceManager::ResourceHasher::operator()(const ResourceDescription & desc) const
	{
		return Utils::HashCombine(desc.name, desc.type);
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

	TexturePtr ResourceManager::EnsureTexture(const std::string & name, const std::string & path)
	{
		TexturePtr ret;

		auto iter = _resources.find(ResourceDescription{ ResourceType::Texture, name });
		if (iter != _resources.end()) {
			ret = std::static_pointer_cast<Texture>(iter->second);
		}
		else {
			ret = std::make_shared<Texture>(name, path);
			Add(ret);
		}

		return ret;
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

#if 1
		std::cout << "[ResourceManager] Hash status: ";
		for (size_t i = 0; i < _resources.bucket_count(); i++) {
			std::cout << _resources.bucket_size(i) << " ";
		}
		std::cout << "\n";
#endif
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