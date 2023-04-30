//
// Created by adrien on 29/04/23.
//

#include <iostream>
#include "world/Path.h"

Path::Path(const sf::Texture& pathTexture, const sf::Texture& cumTexture, sf::Vector2f p1, sf::Vector2f p2, sf::View view)
    : p1(p1), p2(p2), pathTexture(pathTexture), cumTexture(cumTexture), cummed(false) {
    sf::Vector2f edge_vector = p2 - p1;
    float edge_vector_mag = edge_vector.length();

    // Draws from p1 to p2
    sf::Vector2f edge_vector_dir = edge_vector.normalized();
    sf::Vector2f cur_pos = p1;

   while ((cur_pos - p1).length() < edge_vector_mag) {
       sf::Sprite sprite = sf::Sprite(pathTexture);
       sprite.setOrigin({sprite.getLocalBounds().width / 2.f, sprite.getLocalBounds().height / 2.f});
       sprite.setPosition(cur_pos);
       sprite.setScale({1.0f / sprite.getTexture()->getSize().x / float(PATH_TO_NODE_RATIO), 1.0f / sprite.getTexture()->getSize().y / float(PATH_TO_NODE_RATIO)});
       sprite.rotate(sf::Vector2f(1.0f, 0.0f).angleTo(edge_vector_dir));
       sprites.push_back(sprite);

        cur_pos = cur_pos + edge_vector_dir * (1.0f / float(PATH_TO_NODE_RATIO));
    }
}

void Path::draw(sf::RenderTarget& target, const sf::RenderStates& states) const {
    for (sf::Sprite sprite : sprites) {
        target.draw(sprite);
    }
    for(sf::Sprite c_sprite : c_sprites) {
        target.draw(c_sprite);
    }
}

void Path::setCumminess(float cumLevel, bool backdoor) {
    unsigned int total_sprites = sprites.size();
    unsigned int sprites_to_cum = std::ceil(total_sprites * cumLevel);

    if (!backdoor) {
        for (int i = 0 ; i < total_sprites ; i++) {
            if (sprites_to_cum-- == 0)
                return;

            sf::Sprite c_sprite = sprites.at(i);
            c_sprite.setPosition(sprites.at(i).getPosition());
            c_sprite.setScale(sprites.at(i).getScale());
            c_sprite.setOrigin(sprites.at(i).getOrigin());
            c_sprite.setRotation(sprites.at(i).getRotation());
            c_sprite.setTexture(cumTexture);
            c_sprite.setColor(cumColor);
            c_sprites.push_back(c_sprite);
        }
    } else {
        for (int i = total_sprites - 1 ; i > 0 ; i--) {
            if (sprites_to_cum-- == 0)
                return;

            sf::Sprite c_sprite = sprites.at(i);
            c_sprite.setPosition(sprites.at(i).getPosition());
            c_sprite.setScale(sprites.at(i).getScale());
            c_sprite.setOrigin(sprites.at(i).getOrigin());
            c_sprite.setRotation(sprites.at(i).getRotation());
            c_sprite.setTexture(cumTexture);
            c_sprite.setColor(cumColor);
            c_sprites.push_back(c_sprite);
        }
    }
}

void Path::setCumColor(sf::Color color) {
    this->cumColor = color;
}

void Path::setCummed(bool cummed) {
    this->cummed = cummed;
}
