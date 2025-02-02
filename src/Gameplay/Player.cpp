#include <Gameplay/Player.h>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Core/CollisionManager.h"
#include "Core/DungeonManager.h"
#include "Core/WindowManager.h"
#include "Gameplay/Enemy.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "Utils/Constants.h"
#include "Utils/Vector.h"

Player* Player::_instance { nullptr };

Player* Player::getInstance() {
    if (_instance == nullptr) {
        _instance = new Player();
    }

    return _instance;
}

Player::~Player() {
    if (_instance != nullptr) {
        delete _instance;
        _instance = nullptr;
    }
}

void Player::setPosition(const sf::Vector2f& newPosition) {
    GameObject::setPosition(newPosition);
    _sprite.setPosition(newPosition);
    _fireSprite.setPosition({ newPosition.x + TILE_WIDTH / 2, newPosition.y + TILE_HEIGHT / 3 });
}

bool Player::init(const PlayerInfo& playerInfo) {
    float posX = playerInfo.position.x - playerInfo.spriteWidth / 2;
    float posY = playerInfo.position.y - playerInfo.spriteHeight / 2;
    setPosition(sf::Vector2f(posX, posY));
    _speed = playerInfo.speed;

    _sprite.setTexture(*playerInfo.texture);
    _sprite.setScale(playerInfo.scale);
    _spriteWidth = playerInfo.spriteWidth;
    _spriteHeight = playerInfo.spriteHeight;
    _sprite.setTextureRect(sf::IntRect(0, 0, _spriteWidth, _spriteHeight));
    _spriteColor = _sprite.getColor();

    _fireSprite.setTexture(*playerInfo.fireTexture);
    _fireSprite.setScale(playerInfo.scale);
    _fireSpriteWidth = playerInfo.fireSpriteWidth;
    _fireSpriteHeight = playerInfo.fireSpriteHeight;
    _fireSprite.setTextureRect(sf::IntRect(0, 0, _fireSpriteWidth, _fireSpriteHeight));

    _maxHealth = playerInfo.maxHealth;
    _currentHealth = _maxHealth;

    _playerCollider = {
        playerInfo.position.x - TILE_WIDTH / 3,
        playerInfo.position.y,
        2 * TILE_WIDTH / 3,
        TILE_HEIGHT / 2
    };

    for (int i = 0; i < ATTACK_COMBO_COUNT; i++) {
        _attackDamage[i] = BASE_ATTACK_DAMAGE + i * ATTACK_DAMAGE_MULTIPLIER;
    }

    return true;
}

void Player::update(float deltaMilliseconds) {
    // debugHealth();

    if (_isDead) return;

    // Check if vulnerable
    if (_isInvulnerable && _invulnerabilityClock.getElapsedTime().asSeconds() >= INVULNERABILITY_DURATION) {
        _isInvulnerable = false;
    }

    checkBurningState();

    getAttackInput();
    if (_isAttacking) {
        attack();
    }

    if (!_isAttacking) {
        move(deltaMilliseconds);
    }
}

void Player::renderFire(sf::RenderWindow& window) {
    _sprite.setColor(sf::Color(255, 128, 0, 255));

    sf::IntRect fireSpriteRect;
    sf::Vector2f fireTilePosition { _currentFireTile.x * _fireSpriteWidth, _currentFireTile.y * _fireSpriteHeight };
    fireSpriteRect = sf::IntRect(fireTilePosition.x, fireTilePosition.y, _fireSpriteWidth, _fireSpriteHeight);

    sf::Color fireColor = _fireSprite.getColor();
    fireColor.a = 128;
    _fireSprite.setTextureRect(fireSpriteRect);
    _fireSprite.setColor(fireColor);

    window.draw(_fireSprite);
}

void Player::renderInvulnerable() {
    sf::Color spriteColor = _sprite.getColor();
    if (_isInvulnerable) {
        spriteColor.a = INVULNERABLE_ALPHA;
        _sprite.setColor(spriteColor);
    }
    else {
        _sprite.setColor(_spriteColor);
    }
}

void Player::renderPlayer(sf::RenderWindow& window) {
    sf::IntRect spriteRect = _sprite.getTextureRect();
    sf::Vector2f tilePosition { _currentTile.x * _spriteWidth, _currentTile.y * _spriteHeight };

    switch (_faceDirectionX) {
        case DirectionX::Left:
            spriteRect =
                sf::IntRect(tilePosition.x + _spriteWidth, tilePosition.y, -_spriteWidth, _spriteHeight);
            break;

        case DirectionX::Right:
            spriteRect =
                sf::IntRect(tilePosition.x, tilePosition.y, _spriteWidth, _spriteHeight);
            break;

        default:
            break;
    }

    _sprite.setTextureRect(spriteRect);
    window.draw(_sprite);
}

void Player::render(sf::RenderWindow& window) {
    if (_isDead) return;

    // TODO: render dying sprite
    if (_currentHealth <= 0) {
        // renderDying();
        // return;
    }

    renderInvulnerable();

    renderPlayer(window);

    if (_isBurning) {
        renderFire(window);
    }

    // debugSprite(window);
}

void Player::reset(PlayerInfo& playerInfo) {
    _isDead = false;
    _isMoving = false;
    _isBurning = false;
    _isAttacking = false;
    _isInvulnerable = false;
    _direction = sf::Vector2f(.0f, .0f);
    _faceDirection = DirectionEnum::Right;
    _faceDirectionX = DirectionX::Right;
    _faceDirectionY = DirectionY::Up;
    _attackCounter = 0;
    _currentTile = { 0, 0 };
    _currentFireTile = { 0, 0 };

    float posX = playerInfo.position.x - playerInfo.spriteWidth / 2;
    float posY = playerInfo.position.y - playerInfo.spriteHeight / 2;
    setPosition(sf::Vector2f(posX, posY));

    _sprite.setTextureRect(sf::IntRect(0, 0, _spriteWidth, _spriteHeight));
    _sprite.setColor(_spriteColor);
    _fireSprite.setTextureRect(sf::IntRect(0, 0, _fireSpriteWidth, _fireSpriteHeight));

    _maxHealth = playerInfo.maxHealth;
    _currentHealth = _maxHealth;

    _playerCollider = {
        playerInfo.position.x - TILE_WIDTH / 3,
        playerInfo.position.y,
        2 * TILE_WIDTH / 3,
        TILE_HEIGHT / 2
    };
}

void Player::debugSprite(sf::RenderWindow& window) {
    // Global bounds
    const sf::FloatRect spriteBounds = _sprite.getGlobalBounds();
    sf::RectangleShape boundsRect(sf::Vector2f(spriteBounds.width, spriteBounds.height));

    boundsRect.setPosition(spriteBounds.left, spriteBounds.top);
    boundsRect.setOutlineColor(sf::Color::Cyan);
    boundsRect.setOutlineThickness(2.f);
    boundsRect.setFillColor(sf::Color::Transparent);

    // Collision bounds
    sf::RectangleShape collisionRect(sf::Vector2f(_playerCollider.width, _playerCollider.height));

    collisionRect.setPosition(_playerCollider.left, _playerCollider.top);
    collisionRect.setOutlineColor(sf::Color::Green);
    collisionRect.setOutlineThickness(2.f);
    collisionRect.setFillColor(sf::Color::Transparent);

    // Attack collider
    sf::RectangleShape attackRect(sf::Vector2f(_attackCollider.width, _attackCollider.height));

    attackRect.setPosition(_attackCollider.left, _attackCollider.top);
    attackRect.setOutlineColor(sf::Color::Magenta);
    attackRect.setOutlineThickness(2.f);
    attackRect.setFillColor(sf::Color::Transparent);

    window.draw(boundsRect);
    window.draw(collisionRect);
    window.draw(attackRect);
}

void Player::debugHealth() {
    printf("%i/%i\n", _currentHealth, _maxHealth);
}

/* PLAYER ATTACK */
void Player::attack() {
    bool finishedAnimation = attackAnimation();
    checkAttackCollisions();

    if (finishedAnimation) {
        _attackCounter++;
    }
}

void Player::resetAttackCounter() { _attackCounter = 0; }

void Player::resetAttackAnimation() {
    _currentTile.x = 0;
    _attackAnimationClock.restart();
}

void Player::getAttackInput() {
    if (_isAttacking) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        _isAttacking = true;
        _comboDelayClock.restart();

        if (_attackCounter >= ATTACK_COMBO_COUNT) resetAttackCounter();

        resetAttackAnimation();
    }
    else if (_comboDelayClock.getElapsedTime().asSeconds() > COMBO_DELAY_TIME) {
        resetAttackCounter();
        resetAttackAnimation();
    }

    _attackCollider = { 0.f, 0.f, 0.f, 0.f };
}

bool Player::attackAnimation() {
    bool animationFinished = false;
    _currentTile.x = FRAMES_PER_SECOND * _attackAnimationClock.getElapsedTime().asSeconds();

    if (_currentTile.x == 2) {
        setAttackCollider();
    }

    if (_currentTile.x >= MAX_ATTACK_TILES) {
        _currentTile.x %= MAX_ATTACK_TILES;
        _isAttacking = false;
        animationFinished = true;
    }

    int tileColumn = DEFAULT_TILE_ROW + _attackCounter + 2 * static_cast<int>(_faceDirection);
    if (_faceDirection == DirectionEnum::Left) tileColumn = DEFAULT_TILE_ROW + _attackCounter;

    _currentTile.y = tileColumn;

    return animationFinished;
}

void Player::setAttackCollider() {
    sf::FloatRect attackColliderRect = {
        _playerCollider.left - _playerCollider.width / 4 - TILE_WIDTH / 2,
        _playerCollider.top - _playerCollider.height - TILE_HEIGHT / 2,
        TILE_WIDTH,
        TILE_HEIGHT,
    };

    switch (_faceDirection) {
        case DirectionEnum::Right:
            attackColliderRect.left = _playerCollider.left + _playerCollider.width;
            attackColliderRect.height *= 2;
            break;

        case DirectionEnum::Left:
            attackColliderRect.left = _playerCollider.left - TILE_WIDTH;
            attackColliderRect.height *= 2;
            break;

        case DirectionEnum::Up:
            attackColliderRect.top = _playerCollider.top - TILE_HEIGHT;
            attackColliderRect.width *= 2;
            break;

        case DirectionEnum::Down:
            attackColliderRect.top = _playerCollider.top + _playerCollider.height;
            attackColliderRect.width *= 2;
            break;
    }

    _attackCollider = attackColliderRect;
}

void Player::checkAttackCollisions() {
    std::vector<Enemy*> collidedEnemies = CollisionManager::getInstance()->getEnemyCollisions(_attackCollider);

    for (Enemy* enemy : collidedEnemies) {
        if (enemy == nullptr) continue;

        DoDamage(enemy, _attackDamage[_attackCounter]);
    }
}

void Player::DoDamage(IDamageable* damageable, int damageAmount) {
    damageable->ReceiveDamage(_attackCollider, damageAmount);
}

/* PLAYER MOVEMENT */
void Player::move(float deltaMilliseconds) {
    getMoveInput();
    setFacingDirection();
    setMoveAnimation();
    setMovePosition(deltaMilliseconds);
}

void Player::getMoveInput() {
    _direction = { 0, 0 };
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        _direction.x = -1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        _direction.x = 1.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        _direction.y = -1.0f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        _direction.y = 1.0f;
    }

    _direction = Vector::normalize(_direction);

    float magnitude = Vector::magnitude(_direction);
    magnitude > 0.0f ? _isMoving = true : _isMoving = false;
}

void Player::setFacingDirection() {
    if (_direction.x > 0.0f) {
        _faceDirection = DirectionEnum::Right;
        _faceDirectionX = DirectionX::Right;
    }
    else if (_direction.x < 0.0f) {
        _faceDirection = DirectionEnum::Left;
        _faceDirectionX = DirectionX::Left;
    }
    else if (_direction.y > 0.0f) {
        _faceDirection = DirectionEnum::Down;
        _faceDirectionY = DirectionY::Down;
    }
    else if (_direction.y < 0.0f) {
        _faceDirection = DirectionEnum::Up;
        _faceDirectionY = DirectionY::Up;
    }
}

void Player::checkBurningState() {
    setFireAnimation();

    if (_isBurning) {
        if (_burningClock.getElapsedTime().asSeconds() < _burningTime) {
            if (_burningTickClock.getElapsedTime().asSeconds() >= BURNING_TICK_DELAY) {
                ReceiveDamage(_burningDamage);
                _burningTickClock.restart();
            }
        }
        else _isBurning = false;
    }
}

void Player::setFireAnimation() {
    _currentFireTile.x = FRAMES_PER_SECOND * _animationClock.getElapsedTime().asSeconds();
    _currentFireTile.x %= 7;
    _currentFireTile.y = 0;
}

void Player::setMoveAnimation() {
    _currentTile.x = FRAMES_PER_SECOND * _animationClock.getElapsedTime().asSeconds();

    if (_isMoving) {
        _currentTile.x %= MAX_MOVEMENT_TILES;
        _currentTile.y = RUN_ROW;
    }
    else {
        _currentTile.x %= MAX_IDLE_TILES;
        _currentTile.y = IDLE_ROW;
    }
}

void Player::getNextCollisionBounds(sf::Vector2f velocity, sf::FloatRect& nextBounds) {
    nextBounds = _playerCollider;
    nextBounds.left += velocity.x;
    nextBounds.top += velocity.y;
}

void Player::setMovePosition(float deltaMilliseconds) {
    auto velocity = sf::Vector2f(_direction.x * _speed.x * deltaMilliseconds,
                                 _direction.y * _speed.y * deltaMilliseconds);

    // Check window bounds collisions
    sf::FloatRect nextCollisionBounds;
    getNextCollisionBounds(velocity, nextCollisionBounds);

    if (CollisionManager::getInstance()->isOffScreen(nextCollisionBounds)) {
        if (DungeonManager::getInstance()->moveRoom(nextCollisionBounds)) {
            WarpPosition();
        }
        else return;
    }

    std::vector<DirectionEnum> collisionDirections;
    if (CollisionManager::getInstance()->hasObjectCollision(nextCollisionBounds, collisionDirections)) {
        BouncePosition(velocity, collisionDirections);
    }

    // Update player position
    _position.x += velocity.x;
    _position.y += velocity.y;

    // Update collider position
    _playerCollider.left += velocity.x;
    _playerCollider.top += velocity.y;

    _sprite.setPosition(_position);
    _fireSprite.setPosition({ _position.x + TILE_WIDTH / 2, _position.y + TILE_HEIGHT / 3 });
}

void Player::WarpPosition() {
    sf::Vector2u windowSize = WindowManager::getInstance()->getWindowSize();
    switch (_faceDirection) {
        case DirectionEnum::Right:
            _position.x = _position.x - windowSize.x + _playerCollider.width + 5; // CONST WARP_OFFSET
            _playerCollider.left = _playerCollider.left - windowSize.x + _playerCollider.width + 5;
            break;

        case DirectionEnum::Left:
            _position.x = _position.x + windowSize.x - _playerCollider.width - 5;
            _playerCollider.left = _playerCollider.left + windowSize.x - _playerCollider.width - 5;
            break;

        case DirectionEnum::Up:
            _position.y = _position.y + windowSize.y - _playerCollider.height - 5;
            _playerCollider.top = _playerCollider.top + windowSize.y - _playerCollider.height - 5;
            break;

        case DirectionEnum::Down:
            _position.y = _position.y - windowSize.y + _playerCollider.height + 5;
            _playerCollider.top = _playerCollider.top - windowSize.y + _playerCollider.height + 5;
            break;
    }
}

void Player::BouncePosition(sf::Vector2f velocity) {
    // Negate player position
    _position.x -= velocity.x;
    _position.y -= velocity.y;

    // Negate collider position
    _playerCollider.left -= velocity.x;
    _playerCollider.top -= velocity.y;
}

void Player::BouncePosition(sf::Vector2f& velocity, std::vector<DirectionEnum> directions) {
    for (DirectionEnum direction : directions) {
        switch (direction) {
            case DirectionEnum::Right:
            case DirectionEnum::Left:
                // _position.x -= velocity.x;
                // _playerCollider.left -= velocity.x;
                // break;
                velocity.x = 0;
                break;

            case DirectionEnum::Up:
            case DirectionEnum::Down:
                // _position.y -= velocity.y;
                // _playerCollider.top -= velocity.y;
                // break;
                velocity.y = 0;
                break;
        }
    }
}

void Player::AddHealth(int amount) {
    if (_isDead) return;

    _currentHealth = std::clamp(_currentHealth + amount, 0, _maxHealth);
}

int Player::GetHealth() const {
    return _currentHealth;
}

int Player::GetMaxHealth() const {
    return _maxHealth;
}

void Player::ReceiveDamage(int damageAmount) {
    if (_isDead || _isInvulnerable) return;

    AddHealth(-abs(damageAmount));

    if (_currentHealth == 0) {
        _isDead = true;
        return;
    }

    _sprite.setColor(_damageColor);
    _isInvulnerable = true;
    _invulnerabilityClock.restart();
}

void Player::ReceiveDamage(sf::FloatRect otherCollider, int damageAmount) {
    if (_isDead || _isInvulnerable) return;

    AddHealth(-abs(damageAmount));

    if (_currentHealth == 0) {
        _isDead = true;
        return;
    }

    // TODO: add force depending on 'otherCollider' position

    _sprite.setColor(_damageColor);
    _isInvulnerable = true;
    _invulnerabilityClock.restart();
}

void Player::setInFire(float burningTime, int burningDamage) {
    _burningClock.restart();
    _burningTickClock.restart();
    _isBurning = true;
    _burningTime = burningTime;
    _burningDamage = burningDamage;
}
