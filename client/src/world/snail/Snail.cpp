//
// Created by william on 29/04/23.
//

#include "world/snail/Snail.h"
#include "GameAssets.h"
#include "world/World.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SpriteUtil.h"
#include "ui/PathSelectionArrowUI.h"
#include "world/friends/Friend.h"

Snail::Snail(World& world, GraphNode* node, sf::Color snail_color)
    : GraphEntity(world, node),
        snail_color(snail_color),
        clickSound(),
        deliverySound() {
    snail_sprite.setTexture(*world.getAssets().get(GameAssets::SNAILY));
    snail_cap_sprite.setTexture(*world.getAssets().get(GameAssets::SNAILY_CAP));

    snail_cap_sprite.setColor(snail_color);
    actualPosition = node->getPosition();
    snail_sprite.setPosition(actualPosition);
    snail_cap_sprite.setPosition(actualPosition);

    pathSelArrow = new PathSelectionArrowUI(world, snail_color);
    clickSound.setBuffer(*world.getAssets().get(GameAssets::CLICK));
    deliverySound.setBuffer(*world.getAssets().get(GameAssets::DELIVERY));
}

const sf::Vector2f &Snail::getPosition() const {
    return actualPosition;
}

sf::Vector2f Snail::getVisualSize() const {
    return World::VIEW_SIZE;
}

ZOrder Snail::getZOrder() const {
    return ZOrder::Snail_ZOrder;
}

void Snail::deleteYourself() {
    deleteMyself = true;
}

void Snail::draw(sf::RenderTarget& target, const sf::RenderStates& states) const {
    SpriteUtil::setSpriteSize(snail_sprite, sf::Vector2f{2.0f, 2.0f});
    SpriteUtil::setSpriteSize(snail_cap_sprite, sf::Vector2f{2.0f, 2.0f});
    float scaleX = snail_sprite.getScale().x;
    float scaleY = snail_sprite.getScale().y;
    if(locDiff.x >= 0) {
        snail_sprite.setScale(sf::Vector2f {std::abs(scaleX), std::abs(scaleY)});
        snail_cap_sprite.setScale(sf::Vector2f {std::abs(scaleX), std::abs(scaleY)});
    } else {
        snail_sprite.setScale(sf::Vector2f {-std::abs(scaleX), std::abs(scaleY)});
        snail_cap_sprite.setScale(sf::Vector2f {-std::abs(scaleX), std::abs(scaleY)});
    }

    SpriteUtil::setSpriteOrigin(snail_sprite, sf::Vector2f{0.5f, 1.f});
    SpriteUtil::setSpriteOrigin(snail_cap_sprite, sf::Vector2f{0.5f, 1.f});
    if(!moving) {
        for (auto ent : world.getEntities()) {
            if (Friend* friend2 = dynamic_cast<Friend*>(ent)) {
                if (friend2->getLocation() == location) {

                    snail_sprite.setScale(sf::Vector2f {std::abs(snail_sprite.getScale().x) * -1, snail_sprite.getScale().y});
                    snail_cap_sprite.setScale(sf::Vector2f {std::abs(snail_cap_sprite.getScale().x) * -1, snail_cap_sprite.getScale().y});
                    break;
                }
            }
        }
    }

    target.draw(snail_sprite);
    target.draw(snail_cap_sprite);
    target.draw(*pathSelArrow);
}

void Snail::tick(float delta) {
    if(moving)
        tickMovement(delta);
    else
        blockedMoving = false;

    pathSelArrow->tick(delta, moving, getLocation(), snail_color);

    sf::Vector2f spritePos = actualPosition;
    float desiredAngle = 0.0f;

    if(!moving) {
        for (auto ent : world.getEntities()) {
            if (Friend* friend2 = dynamic_cast<Friend*>(ent)) {
                if (friend2->getLocation() == location) {
                    spritePos += {1.0f, 0.0f};
                    break;
                }
            }
        }
    } else {
        desiredAngle = std::atan2(locDiff.y, locDiff.x);
        if(getTargetPosition().x < getLocation()->getPosition().x)
            desiredAngle += M_PI;
    }

    float trans = pow(0.99f, delta * 1000.0f);

    float rads = snail_sprite.getRotation().asRadians();

    if(abs(rads - desiredAngle) > M_PI) {
        if(rads > desiredAngle) {
            desiredAngle += 2 * M_PI;
        } else {
            desiredAngle -= 2 * M_PI;
        }
    }

    snail_sprite.setPosition(spritePos * (1.0f - trans) + snail_sprite.getPosition() * trans);
    snail_cap_sprite.setPosition(spritePos * (1.0f - trans) + snail_cap_sprite.getPosition() * trans);
    snail_sprite.setRotation(sf::radians(desiredAngle) * (1.0f - trans) + snail_sprite.getRotation() * trans);
    snail_cap_sprite.setRotation(sf::radians(desiredAngle) * (1.0f - trans) + snail_cap_sprite.getRotation() * trans);
}

void Snail::moveLocation(GraphNode* node) {
    if (moving || !world.getGraph()->areAdjacent(getLocation(), node) || !node)
        return;

    Path& path = world.getGraph()->getPath(getLocation(), node);
    if(path.isBlocked(getLocation(), snail_color))
        return;

    moving = true;
    setDestination(node);

    const sf::Vector2f& startLoc = getLocation()->getPosition();
    const sf::Vector2f& endLoc = getTargetPosition();
    locDiff = endLoc - startLoc;
}

bool Snail::hasMovementOption() {
    for(GraphNode* neighbor : getLocation()->getNeighbors()) {
        Path& path = world.getGraph()->getPath(getLocation(), neighbor);
        if(path.isBlocked(getLocation(), snail_color))
            continue;

        bool neighbor_blocking = false;
        for(Snail* snail: world.getSnails()) {
            if(neighbor == snail->getLocation() && snail->isMoving() && snail->getDestination() == getLocation()) {
                neighbor_blocking = true;
                break;
            }
        }

        if(neighbor_blocking)
            continue;
        return true;
    }

    return false;
}

void Snail::tickMovement(float delta) {
    currentProgressRate = progressRate / locDiff.length();
    float prevProgress = movingProgress;
    movingProgress += delta * currentProgressRate;
    Path& path = world.getGraph()->getPath(getLocation(), getDestination());

    float progressCap = 1.0f;

    for(Snail* snail : world.getSnails()) {
        if(snail == this)
            continue;

        if(!snail->moving && snail->getLocation() == getDestination()) {
            progressCap = std::min(progressCap, 1.0f - 2.0f / locDiff.length());
        }
        if(snail->moving && snail->getLocation() == getLocation() && snail->getDestination() == getDestination()) {
            if(snail->getMovingProgress() > prevProgress)
                progressCap = std::min(snail->getMovingProgress() - 2.0f / locDiff.length(), progressCap);
        }
    }

    movingProgress = std::min(movingProgress, progressCap);

    if(movingProgress < 1.0f) {
        actualPosition = this->getLocation()->getPosition() + locDiff * movingProgress;
        path.addSlime(getLocation(), std::min(movingProgress, 1.0f), prevProgress, snail_color);
    } else {
        path.addSlime(getLocation(), 1.0f, prevProgress, snail_color);
        setLocation(getDestination());
        moving = false;
        actualPosition = getLocation()->getPosition();
        movingProgress = 0.0f;
    }
    blockedMoving = movingProgress == prevProgress;
}

bool Snail::isBlockedMoving() const {
    return blockedMoving;
}

GraphNode* Snail::hitScan(const sf::Vector2f& hit) {
    if (moving)
        return nullptr;

    GraphNode* target = pathSelArrow->hitScanAll(hit);
    if (target) {
        moveLocation(target);
        getWorld().hasSnailMadeFirstMove = true;
        clickSound.play();
    }
    return target;
}

bool Snail::hover(sf::Vector2f vector2) {
    if(moving)
        return false;
    return pathSelArrow->hover(vector2);
}

const sf::Color &Snail::getSnailColor() const {
    return snail_color;
}
bool Snail::isMoving() {
    return moving;
}

void Snail::setProgressRate(float progressRate) {
    Snail::progressRate = progressRate;
}

float Snail::getMovingProgress() const {
    return movingProgress;
}
