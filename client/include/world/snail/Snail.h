//
// Created by william on 29/04/23.
//

#ifndef LD53_CLIENT_SNAIL_H
#define LD53_CLIENT_SNAIL_H

#include "SFML/Graphics/Sprite.hpp"
#include "world/GraphEntity.h"

class PathSelArrow;

class Snail : public GraphEntity, public Tickable {
    mutable sf::Sprite snail_sprite;
    mutable sf::Sprite snail_cap_sprite;
    bool isMoving = false;
    float movingProgress = 0.f;
    float progressRate = 2.f;
    float currentProgressRate = 0;

    sf::Vector2f locDiff;

    sf::Vector2f actualPosition;

    PathSelArrow* pathSelArrow;
    bool arrowPosUpdated = false;

public:
    Snail(World& world, GraphNode* node, sf::Color snail_color);

    const sf::Vector2f &getPosition() const override;

    sf::Vector2f getVisualSize() const override;

    ZOrder getZOrder() const override;

    void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

    void tick(float delta);

    void moveLocation(GraphNode* targetLocation);

    void tickMovement(float delta);

    GraphNode* hitScan(const sf::Vector2f& hit);
};

#endif //LD53_CLIENT_SNAIL_H
