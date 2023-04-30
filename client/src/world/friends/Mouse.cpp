//
// Created by cedric on 4/29/23.
//

#include "world/friends/Mouse.h"
#include "world/World.h"
#include "GameAssets.h"
#include "SFML/Graphics.hpp"
#include "SpriteUtil.h"

Mouse::Mouse(World& world, GraphNode* node) : Friend(world, node, *world.getAssets().get(GameAssets::MOUSE),
                                                     *world.getAssets().get(GameAssets::MOUSE_LETTER)) {

}