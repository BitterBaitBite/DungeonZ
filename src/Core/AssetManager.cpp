#include <Core/AssetManager.h>

AssetManager* AssetManager::s_instance{ nullptr };

AssetManager* AssetManager::getInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new AssetManager();
	}
	return s_instance;
}

AssetManager::~AssetManager()
{
	for (auto it : m_texturePathToTexture)
	{
		delete it.second;
	}
}

sf::Texture* AssetManager::loadTexture(const char* assetPath)
{
	auto it = m_texturePathToTexture.find(assetPath);
	if (it != m_texturePathToTexture.end())
	{
		return it->second; // Already loaded, reuse
	}
	else
	{
		sf::Texture* newTexture = new sf::Texture();
		const bool loadOk = newTexture->loadFromFile(assetPath);
		if (!loadOk)
		{
			delete newTexture;
			return nullptr;
		}
		m_texturePathToTexture[assetPath] = newTexture;
		return newTexture; // just created a new one
	}
}

sf::Texture* AssetManager::getTexture(const char* assetPath) const
{
	const auto it = m_texturePathToTexture.find(assetPath);
	return (it != m_texturePathToTexture.end()) ? it->second : nullptr;
}