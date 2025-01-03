#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

namespace sf
{
	sf::Texture;
}

class AssetManager
{
	public:

		static AssetManager* getInstance();

		~AssetManager();

		sf::Texture* loadTexture(const char* assetPath);

		sf::Texture* getTexture(const char* assetPath) const;


	private:

		static AssetManager* s_instance;

		std::unordered_map<std::string, sf::Texture*> m_texturePathToTexture;
};