//
// Created by william on 30/04/23.
//

#ifndef LD53_CLIENT_LEVELINDICATOR_H
#define LD53_CLIENT_LEVELINDICATOR_H

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "input/ClickableUI.h"
#include "world/Tickable.h"

class World;
class LevelSelMenu;

class LevelIndicator : public sf::Drawable, public ClickableUI, public Tickable {
    mutable sf::Sprite backgroundSprite;
    mutable sf::Text label;

    mutable sf::Text level_num;

    sf::Vector2f mainOffset = {50.0f, 70.0f};
    sf::Vector2f numOffset = {40.f, -75.f};

    sf::Vector2f labelOffset = {-40.f, -75.f};

    std::string levelNum;

    World& world;
    LevelSelMenu& levelSelMenu;

    bool hovered = false;
    float backgroundScale = 1.f;

public:
    LevelIndicator(World &world, LevelSelMenu& levelSelMenu);

    void tick(float delta) override;

    void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

    void hitAction(bool& isHit) override;

    void hoverAction(bool& isHit) override;

    ~LevelIndicator() = default;
};

#endif //LD53_CLIENT_LEVELINDICATOR_H
