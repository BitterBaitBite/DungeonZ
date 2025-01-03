#include <Core/AssetManager.h>
#include <Core/World.h>
#include <Gameplay/Zombie.h>
#include <SFML/Graphics/RenderWindow.hpp>

World::~World()
{
	delete m_enemy;
}

bool World::load()
{
	constexpr float millisecondsToSeconds = 1 / 1000.f;

	// To-Do, read ALL from file, this is just a quick example to understand that here is where entities are created but consider grouping/managing actors in a smarter way
	sf::Texture* zombieTexture = AssetManager::getInstance()->loadTexture("../Data/Images/Enemies/zombie.png");
	Zombie::ZombieDescriptor zombieDescriptor;
	zombieDescriptor.texture = zombieTexture;
	zombieDescriptor.position = { 50.f, 50.f };
	zombieDescriptor.speed = { 400.f * millisecondsToSeconds, .0f }; // 400 units per second, or 0.4 units per millisecond, using the latter so it's in alignment with delta time
	zombieDescriptor.tileWidth = 192.f;
	zombieDescriptor.tileHeight = 256.f;
	Zombie* zombie = new Zombie();
	const bool initOk = zombie->init(zombieDescriptor);

	m_enemy = zombie;

	return initOk;
}

void World::update(uint32_t deltaMilliseconds)
{
	m_enemy->update(deltaMilliseconds);
}

void World::render(sf::RenderWindow& window)
{
	m_enemy->render(window);
}