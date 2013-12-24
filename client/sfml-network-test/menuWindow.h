#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class menuWindow :
	public sf::RenderWindow
{
	bool newGame;
	bool connect;

public:
	
	sf::Texture _background;
	sf::Texture _newGameButton;
	sf::Texture _newGameActiveButton;
	sf::Sprite newGameButton;
	sf::Sprite background;
    
    sf::Music backgroundMucis;
    sf::SoundBuffer buffer;
    sf::Sound buttonSound;
    
	menuWindow();
	int menuWindowUpdate();  // main loop
	
	bool isNewGame()
	{
		return this->newGame;			
	};
	bool isConnect()
	{
		return this->connect;	
	};
	virtual ~menuWindow(void);
};

