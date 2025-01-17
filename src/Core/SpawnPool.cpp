//#include <Core/SpawnPool.h>
//
//SpawnPool::ObjectPool::ObjectPool() {
//    activePool = new std::set<GameObject*>();
//    inactivePool = new std::set<GameObject*>();
//}
//
//SpawnPool::ObjectPool::~ObjectPool() {
//    while (!activePool->empty()) delete activePool->end, activePool->erase(activePool->end);
//    delete activePool;
//
//    while (!inactivePool->empty()) delete inactivePool->end, inactivePool->erase(inactivePool->end);
//    delete inactivePool;
//
//    delete object;
//}
//
//SpawnPool* SpawnPool::_instance { nullptr };
//
//SpawnPool::SpawnPool() {}
//
//SpawnPool::~SpawnPool() {}
//
//
//SpawnPool* SpawnPool::getInstance() {
//    if (_instance == nullptr) {
//        _instance = new SpawnPool();
//    }
//
//    return _instance;
//}
