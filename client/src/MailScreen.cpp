//
// Created by cedric on 4/29/23.
//
//
// Created by Alexander Winter on 2022-03-29.
//

#include "MailScreen.h"
#include "GameAssets.h"
#include "world/World.h"

MailScreen::MailScreen(wiz::Game& game)
        : Screen(game), world(game.getAssets()), testPath(*game.getAssets().get(GameAssets::PATH), {10, 10}, {100, 100}) {
}

void MailScreen::tick(float delta) {
}

void MailScreen::render(sf::RenderTarget &target) {
    target.clear(sf::Color::Green);
    target.draw(testPath);
}

const std::string &MailScreen::getName() const {
    return name;
}

void MailScreen::windowClosed() {
    getGame().getWindow().close();
}

void MailScreen::show() {
    getGame().addWindowListener(this);
}

void MailScreen::hide() {
    getGame().removeWindowListener(this);
}
