//
// Created by cedric on 4/29/23.
//

#ifndef LD53_CLIENT_FRIEND_H
#define LD53_CLIENT_FRIEND_H

#include "SFML/Graphics/Sprite.hpp"
#include "world/GraphEntity.h"
#include "world/World.h"

class Friend : public GraphEntity {
    mutable sf::Sprite sprite;


public:
    Friend(World& world, GraphNode* node, const sf::Texture& texture);

    const sf::Vector2f& getPosition() const;

    sf::Vector2f getVisualSize() const override;

    ZOrder getZOrder() const override;

    void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;
};

#endif //LD53_CLIENT_FRIEND_H
