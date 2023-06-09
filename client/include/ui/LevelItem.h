//
// Created by william on 30/04/23.
//

#ifndef LD53_CLIENT_LEVELITEM_H
#define LD53_CLIENT_LEVELITEM_H

#include "SFML/Graphics/Drawable.hpp"
#include "input/ClickableUI.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "ui/LevelIndicator.h"

class World;
class LevelSelMenu;

class LevelItem : public sf::Drawable, public ClickableUI, public Tickable {
    mutable sf::Sprite backgroundSprite;
    mutable sf::Sprite metalSprite;
    mutable sf::Sprite greyOutSprite;

    mutable sf::Text level_num;

    sf::Vector2f parentMenuOffset;
    sf::Vector2f mainOffset;
    sf::Vector2f metalOffset;
    sf::Vector2f numOffset;

    sf::Vector2f size = {100.f, 100.f};

    std::string levelNumString;

    World& world;
    LevelSelMenu &levelSelMenu;

    int levelNum = 0;
    const int max_per_row = 5;

    bool drawMetal = false;

    bool hovered = false;
    float backgroundScale = 1.f;

public:
    LevelItem(World &world, LevelSelMenu & levelSelMenu, sf::Vector2f parentMenuOffset, int levelNum);

    void tick(float delta) override;

    void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

    void hitAction(bool& isHit) override;

    void hoverAction(bool& isHit) override;

    void calculateOffsets();

    void updateMedals();

    ~LevelItem() = default;
};

#endif //LD53_CLIENT_LEVELITEM_H
