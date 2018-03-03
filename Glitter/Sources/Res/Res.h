#pragma once

#include "Forward.h"

namespace MyGL
{
	enum class ResourceType 
	{
		Texture,
		Shader,
		Cubemap
	};

	class Resource 
	{
	public:
		Resource(std::string name, ResourceType resourceType);
		virtual ~Resource();

		ResourceType GetType() const;
		std::string GetName() const;

	private:
		ResourceType _type;
		std::string _name;
	};
	using ResourcePtr = std::shared_ptr<Resource>;


	class ResourceManager
	{
		struct ResourceDescription
		{
			ResourceType type;
			std::string name;

			bool operator==(const ResourceDescription& desc) const;
		};

		struct ResourceHasher
		{
			size_t operator()(const ResourceDescription& res) const;
		};

	public:
		ResourceManager();
		~ResourceManager();

		static void Initialize();
		static void Deinitialize();
		static ResourceManager* Instance();

		ShaderPtr AddShader(const std::string& name, const std::string& baseName, bool useGeometry = false);
		ShaderPtr AddVPQuadShader(const std::string& name, const std::string& baseName);
		ShaderPtr GetShader(const std::string & name);

		TexturePtr EnsureTexture(const std::string& name, const std::string& path);
		TexturePtr AddTexture(const std::string& name, const std::string& baseName);
		TexturePtr GetTexture(const std::string & name);

		CubemapPtr AddCubemap(const std::string& name, const std::string& baseName, const char* extension, std::vector<const char*> faces);
		CubemapPtr GetCubemap(const std::string & name);

	private:
		void Add(ResourcePtr res);
		ResourcePtr Find(ResourceType type, const std::string& name);

	private:
		static ResourceManager* _sInstance;
		std::unordered_map<ResourceDescription, ResourcePtr, ResourceHasher> _resources;
	};
}