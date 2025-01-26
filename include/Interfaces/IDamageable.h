#include <algorithm>

#pragma once

class IDamageable {
    public:
        virtual ~IDamageable() {}

        virtual int GetHealth() const;
        virtual int GetMaxHealth() const;

        virtual void AddHealth(int health);
};
