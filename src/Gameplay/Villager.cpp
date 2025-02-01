#include <queue>
#include <Gameplay/Villager.h>

#include "Core/CollisionManager.h"
#include "Core/DungeonManager.h"
#include "Gameplay/Player.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Utils/Pathfinding.h"
#include "Utils/Random.h"
#include "Utils/Vector.h"

bool Villager::init(EnemyInfo* enemyInfo) {
    auto villagerInfo = static_cast<VillagerInfo*>(enemyInfo);
    _attackDamage = villagerInfo->attackDamage;

    return Enemy::init(enemyInfo);
}

void Villager::update(float deltaMilliseconds) {
    // Enemy::update(deltaMilliseconds);
    if (_pathToTarget.empty()) {
        _pathToTarget = getPath(sf::Vector2i({
                                    static_cast<int>(_position.x / TILE_WIDTH),
                                    static_cast<int>(_position.y / TILE_HEIGHT)
                                }),
                                sf::Vector2i({
                                    static_cast<int>(Player::getInstance()->getPosition().x / TILE_WIDTH),
                                    static_cast<int>(Player::getInstance()->getPosition().y / TILE_HEIGHT)
                                }),
                                DungeonManager::getInstance()->getRoomCostGrid());
    }

    if (_currentTarget >= _pathToTarget.size()) return;

    sf::Vector2f targetPos(_pathToTarget[_currentTarget].x * TILE_WIDTH, _pathToTarget[_currentTarget].y * TILE_HEIGHT);
    sf::Vector2f currentPos = _sprite.getPosition();
    sf::Vector2f direction = targetPos - currentPos;

    // float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float distance = Vector::magnitude(direction);
    if (distance < _speed.x * deltaMilliseconds) {
        _sprite.setPosition(targetPos);
        _currentTarget++;
    }
    else {
        direction /= distance; // Normalize

        // _position.x += direction.x * deltaMilliseconds;
        // _position.y += direction.y * deltaMilliseconds;

        _sprite.move(direction * _speed.x * deltaMilliseconds);
    }
}

void Villager::render(sf::RenderWindow& window) {
    Enemy::render(window);
}

void Villager::calcTargetPosition() {
    Enemy::calcTargetPosition();

    float targetDistance = Vector::heuristicDistance(_position, _targetPosition);
    _hasCloseTarget = targetDistance <= TARGET_RANGE;
}

void Villager::debugSprite(sf::RenderWindow& window) {
    Enemy::debugSprite(window);

    sf::CircleShape closeTargetCircle(TARGET_RANGE);

    closeTargetCircle.setPosition({ _position.x - TARGET_RANGE / 2, _position.y - TARGET_RANGE / 2 });
    closeTargetCircle.setOutlineColor(sf::Color::Red);
    closeTargetCircle.setOutlineThickness(2.f);
    closeTargetCircle.setFillColor(sf::Color::Transparent);

    // Attack collider
    sf::RectangleShape attackRect(sf::Vector2f(_attackCollider.width, _attackCollider.height));

    attackRect.setPosition(_attackCollider.left, _attackCollider.top);
    attackRect.setOutlineColor(sf::Color::Magenta);
    attackRect.setOutlineThickness(2.f);
    attackRect.setFillColor(sf::Color::Transparent);

    window.draw(closeTargetCircle);
    // window.draw(attackRect);

    // Path
    window.draw(_pathLine);
}

void Villager::move(float deltaMilliseconds) {
    if (_hasCloseTarget) {
        auto currentPos = sf::Vector2i(_position.x, _position.y);
        auto currentTarget = sf::Vector2i(_targetPosition.x, _targetPosition.y);

        if (_pathToTarget.empty()) {
            _pathToTarget = Pathfinding::getPath(
                currentPos,
                currentTarget,
                DungeonManager::getInstance()->getRoomCostGrid()
            );
            _currentTarget = 0;
        }

        if (_currentTarget < _pathToTarget.size()) {
            if (_pathClock.getElapsedTime().asSeconds() > 0.2f) {
                _direction.x = _pathToTarget[_currentTarget].x;
                _direction.y = _pathToTarget[_currentTarget].y;

                _currentTarget++;
                _pathClock.restart();
            }

            float magnitude = Vector::magnitude(_direction);
            if (magnitude > 0.0f) _isMoving = true;
            else _isMoving = false;
        }
    }
    else {
        if (!_pathToTarget.empty()) {
            _pathToTarget.clear();
        }
    }

    Enemy::move(deltaMilliseconds);
}

void Villager::getMoveDirection() {
    if (!_hasCloseTarget) {
        // Patrol delay control
        if (!_isPatrolling && _patrolDelayClock.getElapsedTime().asSeconds() >= PATROL_DELAY_TIME) {
            _patrolClock.restart();
            _isPatrolling = true;

            // TODO: change the random so direction can be a bitmask
            _faceDirection = static_cast<DirectionEnum>(Random::randomInt(0, 3));
        }

        if (_isPatrolling) {}
        // Patrol movement direction
        if (_faceDirection == DirectionEnum::Left) {
            _direction.x = -1.0f;
        }
        if (_faceDirection == DirectionEnum::Right) {
            _direction.x = 1.0f;
        }
        if (_faceDirection == DirectionEnum::Up) {
            _direction.y = -1.0f;
        }
        if (_faceDirection == DirectionEnum::Down) {
            _direction.y = 1.0f;
        }

        // Patrol control
        if (_isPatrolling && _patrolClock.getElapsedTime().asSeconds() >= MAX_PATROL_TIME) {
            _patrolDelayClock.restart();
            _isPatrolling = false;
        }
    }

    _direction = Vector::normalize(_direction);

    float magnitude = Vector::magnitude(_direction);
    if (magnitude > 0.0f) _isMoving = true;
    else _isMoving = false;
}

std::vector<sf::Vector2i> Villager::getPath(
    sf::Vector2i start,
    sf::Vector2i goal,
    std::array<std::array<bool, BACKGROUND_ROW_SIZE>, BACKGROUND_COL_SIZE>& grid
) {
    std::unordered_map<int, Node> allNodes;

    // Nodes to check ordered by fCost (total cost)
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

    // Gets a unique index for any two-dimensional coordinates of a node
    auto nodeIndex = [](int x, int y) { return y * BACKGROUND_ROW_SIZE + x; };

    // Sets the node to start
    Node startNode = { start.x, start.y, 0, Vector::heuristicDistance(start.x, start.y, goal.x, goal.y), 0, nullptr };
    // Node startNode;
    // {
    //     startNode.x = start.x;
    //     startNode.y = start.y;
    //     startNode.gCost = 0;
    //     startNode.hCost = Vector::heuristicDistance(start.x, start.y, goal.x, goal.y);
    //     startNode.fCost = 0;
    //     startNode.parent = nullptr;
    // }
    openSet.push(startNode);
    allNodes[nodeIndex(start.x, start.y)] = startNode;

    // While there's nodes to check
    while (!openSet.empty()) {
        // Extracts the top node from the search queue (the one with least fCost)
        Node current = openSet.top();
        openSet.pop();

        // If the node is the goal, gets the path backwards by node parent and reverses it
        if (current.x == goal.x && current.y == goal.y) {
            std::vector<sf::Vector2i> path;
            while (current.parent) {
                path.push_back({ current.x, current.y });
                current = *current.parent;
            }
            std::reverse(path.begin(), path.end());

            return path;
        }

        // Creates a vector with the adjacent node coordinates
        std::vector<sf::Vector2i> adjacents = {
            { current.x + 1, current.y },
            { current.x - 1, current.y },
            { current.x, current.y + 1 },
            { current.x, current.y - 1 }
        };

        // Checks all adjacent nodes
        for (const auto& adjacent : adjacents) {
            // Ignores collision or non existent nodes
            if (grid[adjacent.y][adjacent.x]) continue;

            // Updates the costs (gCost = path length / hCost = distance from adjacent position to goal / fCost = gCost + hCost (total cost)
            float gCost = current.gCost + 1;
            float hCost = Vector::heuristicDistance(adjacent.x, adjacent.y, goal.x, goal.y);
            float fCost = gCost + hCost;

            // Continues on adjacent node if already checked and total cost is less or equal
            if (allNodes.count(nodeIndex(adjacent.x, adjacent.y)) // Counts nodes by index
                && allNodes[nodeIndex(adjacent.x, adjacent.y)].fCost <= fCost)
                continue;

            Node adjacentNode = {
                adjacent.x,
                adjacent.y,
                gCost,
                hCost,
                fCost,
                &allNodes[nodeIndex(current.x, current.y)]
            };

            openSet.push(adjacentNode);
            allNodes[nodeIndex(adjacent.x, adjacent.y)] = adjacentNode;
        }
    }

    return {};
}
