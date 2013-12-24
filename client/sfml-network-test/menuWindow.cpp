#include "menuWindow.h"

int menuWindow::menuWindowUpdate(void)
{   
        // run the program as long as the window is open
	while(this->isOpen()){
		// check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (this->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                this->close();
            if (event.type == sf::Event::MouseMoved || event.type == sf::Event::MouseButtonPressed) {
                
                if (sf::Mouse::getPosition(*this).x >= 180 && sf::Mouse::getPosition(*this).y >= 360
                    && sf::Mouse::getPosition(*this).x <= 460 && sf::Mouse::getPosition(*this).y <= 410)
                {
                   
                    this->newGameButton.setTexture(_newGameActiveButton);
                    if (!(this->buttonSound.getStatus() == sf::Sound::Playing) && !newGame){
                        this->buttonSound.play();
                        this->newGame = true;
                    }
                    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        this->close();
                        return 1;
                    }
                } else
                {
                    this->newGame = false;
                    this->newGameButton.setTexture(_newGameButton);
                }
            }
        }

		

        // clear the window with black color
        this->clear(sf::Color::Black);

        // draw everything here...
		this->draw(this->background);
		this->draw(this->newGameButton);
		// end the current frame
        this->display();
	}
    return 0;
}

menuWindow::menuWindow()
{
	sf::RenderWindow::RenderWindow();
	this->_background.loadFromFile("images/backgroundImage.gif");
	this->_newGameButton.loadFromFile("images/newGameButton.gif");
	this->_newGameActiveButton.loadFromFile("images/newGameActiveButton.png");
	this->background.setTexture(_background);
	this->newGameButton.setTexture(_newGameButton);
	
	this->newGame = false;
	this->connect = false;
    this->backgroundMucis.openFromFile("music/backgroundMusic.wav");
    this->buffer.loadFromFile("music/buttonClick.wav");
    this->buttonSound.setBuffer(buffer);
    
	this->backgroundMucis.setVolume(50);
	this->backgroundMucis.openFromFile("music/backgroundMusic.wav");
    this->backgroundMucis.setLoop(true);
    this->backgroundMucis.setPlayingOffset(sf::seconds(8));
    
    this->buffer.loadFromFile("music/buttonSound.wav");
    this->buttonSound.setBuffer(buffer);
    this->buttonSound.setVolume(100);
    
}

menuWindow::~menuWindow(void)
{
    
}
