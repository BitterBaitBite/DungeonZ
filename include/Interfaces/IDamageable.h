#pragma once
#include "SFML/Graphics/Rect.hpp"

class IDamageable {
    public:
        virtual ~IDamageable() {}

        virtual bool IsDead() const = 0;
        virtual int GetHealth() const = 0;
        virtual int GetMaxHealth() const = 0;
        virtual void ReceiveDamage(sf::FloatRect otherCollider, int damageAmount) = 0;

    protected:
        virtual void AddHealth(int health) = 0;
};
