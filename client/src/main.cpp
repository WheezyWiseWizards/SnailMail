#include <SFML/Graphics.hpp>
#include <memory>
#include <WIZ/logging/MultiLogger.h>
#include <WIZ/logging/StdOutLogger.h>
#include <WIZ/logging/DateTimeLoggerWrapper.h>
#include <WIZ/logging/TagLoggerWrapper.h>
#include <WIZ/asset/AssetLoader.h>

#include "MailGame.h"
#include "LoadingScreen.h"

#include <WIZ/logging/DailyFileLogger.h>
#include <iostream>

#ifdef OS_SWITCH
	#include <switch.h>
#endif

int main(int argc, char* argv[])
{
#ifdef OS_SWITCH
	// Setup NXLink
	socketInitializeDefault();
	nxlinkStdio();

	Result rc = romfsInit();
#endif

    std::shared_ptr<wiz::Logger> logger = std::unique_ptr<wiz::Logger>(new wiz::TagLoggerWrapper(
			new wiz::DateTimeLoggerWrapper(
                new wiz::MultiLogger({
                    new wiz::DailyFileLogger("logs/", wiz::LogLevel::DEBUG),
                    new wiz::StdOutLogger(wiz::LogLevel::DEBUG)
                }), "[%H:%M:%S]")));

	std::shared_ptr<wiz::AssetLoader> loader = std::make_shared<wiz::AssetLoader>(*logger);

#ifdef OS_SWITCH
    logger->info("OS is Switch");
#endif

#ifdef OS_WINDOWS
    logger->info("OS is Windows");
#endif

#ifdef OS_MAC
    logger->info("OS is Mac");
#endif

#ifdef OS_UNIX
    logger->info("OS is Unix");
#endif

	sf::VideoMode mode;

#ifdef OS_SWITCH
	mode = sf::VideoMode::getDesktopMode();
#else
	mode = sf::VideoMode(1280, 720, 32);
#endif

	std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>(mode, "Snail Mail");

    srand(time(nullptr));
	MailGame game(window, logger, loader);

	game.setScreen(std::make_shared<LoadingScreen>(game));

	while (window->isOpen()) {
		game.update();
	}

#ifdef OS_SWITCH
	socketExit();
#endif
	return EXIT_SUCCESS;
}
