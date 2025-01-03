#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
}

class GameObject
{
	public:

		GameObject() = default;
		GameObject(float x, float y) : m_position{ x,y } {}
		virtual ~GameObject() = default; // IMPORTANT! If we don't define it as virtual, we will leak memory

		const sf::Vector2f& getPosition() const { return m_position; }
		sf::Vector2f& getPosition() { return m_position; }
		void setPosition(const sf::Vector2f& newPosition) { m_position = newPosition; }

		virtual void update(float delta) = 0;
		virtual void render(sf::RenderWindow& window) = 0;

	protected:

		sf::Vector2f m_position{ .0f, .0f };
};