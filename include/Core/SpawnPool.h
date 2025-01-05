#pragma once
//#include <list>
//#include <memory>
//#include <set>
//#include <unordered_map>
//
//#include <Gameplay/GameObject.h>
//#include <Types/Direction.h>
//
//class SpawnPool {
//    class ObjectPool {
//        public:
//            ObjectPool();
//            ~ObjectPool();
//            GameObject* object { nullptr };
//
//            void preload();
//            GameObject* spawnObject(sf::Vector2f position, Direction direction, sf::Vector2f scale);
//            GameObject* newObject(sf::Vector2f position, Direction direction);
//            void despawnObject(GameObject* object);
//
//        private:
//            std::set<GameObject*>* activePool { nullptr };
//            std::set<GameObject*>* inactivePool { nullptr };
//    };
//
//    public:
//        ~SpawnPool();
//        static SpawnPool* getInstance();
//        std::list<ObjectPool*> getPools() const { return _pools; }
//
//    private:
//        // Singleton instance
//        static SpawnPool* _instance;
//
//        SpawnPool();
//        std::list<ObjectPool*> _pools;
//
//        void init();
//        GameObject* spawn(GameObject* object);
//        GameObject* spawn(GameObject* object, sf::Vector2f position);
//        GameObject* spawn(GameObject* object, sf::Vector2f position, Direction direction);
//        GameObject* spawn(GameObject* object, sf::Vector2f position, Direction direction, sf::Vector2f scale);
//        void despawn(GameObject* object);
//        void createPools();
//};
