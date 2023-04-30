//
// Created by cedric on 4/29/23.
//

#include "world/friends/Frog.h"
#include "world/World.h"
#include "GameAssets.h"
#include "SFML/Graphics.hpp"
#include "SpriteUtil.h"

Frog::Frog(World& world, GraphNode* node) : Friend(world, node, *world.getAssets().get(GameAssets::FROG_SPRITE_SHEET), 0.3f, 2.0f) {

}

const FriendType Frog::getFriendType() const {
    return FROG;
}
