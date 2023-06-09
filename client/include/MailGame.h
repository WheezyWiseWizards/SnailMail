//
// Created by Winter on 12/02/2022.
//

#ifndef GAMETEMPLATE_EXAMPLEGAME_H
#define GAMETEMPLATE_EXAMPLEGAME_H

#include <WIZ/game/BasicGame.h>

class MailGame : public wiz::BasicGame {
public:
	MailGame(std::shared_ptr<sf::RenderWindow> window,
             std::shared_ptr<wiz::Logger> logger,
             std::shared_ptr<wiz::AssetLoader> assetLoader);
};


#endif //GAMETEMPLATE_EXAMPLEGAME_H
