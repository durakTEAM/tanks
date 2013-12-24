// ----- The client -----

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "menuWindow.h"

#include <iostream>
#include <iostream>
#include <vector>
#include <string>

using namespace sf;
using namespace std;

#pragma mark constVar
// константные переменные
const int screen_height=600; //размеры поля
const int screen_width=600;

const int tile_x=30; //размеры тайла
const int tile_y=30;

const int tank_w=29; //размеры танка
const int tank_h=29;

const int direction_right = 1;
const int direction_up = 2;
const int direction_left = 3;
const int direction_down = 4;

const int tank_speed = 3;

//const int number_tiles_y=40;			//(screen_height/tile_y);
String TileMap[20] =
{
    "11111111111111111111",
    "1               2  1",
    "1               2  1",
    "1222           22  1",
    "1  2     2         1",
    "1    22  22  22    1",
    "1    2             1",
    "1       2  22      1",
    "1             22   1",
    "1    2   2    2    1",
    "1    2     2  2    1",
    "1   22             1",
    "1       22  2      1",
    "1        2    2    1",
    "1    22  2   22    1",
    "1            2  2  1",
    "1  22           2221",
    "1  2               1",
    "1  2               1",
    "11111111111111111111",
    
};
//карта


#pragma mark classes
// начало системы объектов
class Point{
public:
    int x, y;
    Point(int x1, int y1){
        x = x1;
        y = y1;
    }
};

class Dynamic_rect : public Point { //пули
public:
	bool life; //нужно для корректной работы с вектором пуль
	int h, w;
    std::string texture;
    int speed;
	int direction;
    Dynamic_rect(int x1, int y1, int w1, int h1, std::string t, int s1, int d1, bool l1) : Point(x1,y1){
        w = w1;
        h = h1;
		texture = t;
		speed = s1;
    	direction = d1;
		life=l1;
	}
};


class Tank : public Dynamic_rect{
public:
	bool per_left; bool per_right; bool per_up; bool per_down;
	Tank (int x1, int y1, int w1, int h1, std::string t, int s1, int d1, bool l1) : Dynamic_rect (x1,y1,w1,h1,t,s1,d1,l1){
        per_down = per_left = per_right = per_up = 1;
	};
    
	void update (float &frame){
        frame-=0.3;
		if (direction == direction_right){
			x += speed;
			Collision_walls(0); //проверка по х
		}
		if (direction == direction_left){
			x -= speed;
			Collision_walls(0); //проверка по х
		}
		
		if (direction == direction_up){
			y -= speed;
			Collision_walls(1); //проверка по у
		}
		if (direction == direction_down){
			y += speed;
			Collision_walls(1); //проверка по у
		}
		
	}
    
	void Collision_walls(int dir){
		for (int i=int(y / tile_y); i<int((y + h)/tile_y)+1; i++){
			for (int j=int(x / tile_x); j<int((x + w)/tile_x)+1; j++){
				if (TileMap[i][j]=='1' || TileMap[i][j]=='2'){
					if ((this->direction==direction_left)&&(dir==0)) this->x = j*tile_x + this->w + 1;
					if ((this->direction==direction_right)&&(dir==0)) this->x = j*tile_x - this->w - 1;
					if ((this->direction==direction_up)&&(dir)) this->y = i*tile_y + this->h + 1;
					if ((this->direction==direction_down)&&(dir)) this->y = i*tile_y - this->h - 1;
				}
			}
		}
	}
    
    
};

struct packet_enemy
{
    float       number;         //количество
    int x;   //
    int y;
};
sf::Packet& operator <<(sf::Packet& packet, const packet_enemy& m)
{
    return packet << m.number << m.x << m.y;
}
sf::Packet& operator >>(sf::Packet& packet, packet_enemy& m)
{
    return packet >> m.number >> m.x >> m.y;
}

struct packet_my
{
    int life;         //количество
    int per_down;   //
    int per_up;
    int per_left;
    int per_right;
};
sf::Packet& operator <<(sf::Packet& packet, const packet_my& m)
{
    return packet << m.life << m.per_down << m.per_left << m.per_right << m.per_up;
}
sf::Packet& operator >>(sf::Packet& packet, packet_my& m)
{
    return packet >> m.life >> m.per_down >> m.per_left >> m.per_right >> m.per_up;
}

//конец для системы объектов


int main()
{
    
    menuWindow a;                     //открываем окно меню
	a.create(sf::VideoMode(640, 480), "Tanks - Main menu", sf::Style::None);
    a.backgroundMucis.play();
    if (a.menuWindowUpdate()==1)
    {
        a.backgroundMucis.stop();
        a.menuWindow::~menuWindow();
    
        Clock clock;
        Time update_time=sf::milliseconds(15); //0.03 sec
        Time ttime;
        
        int time;
        float frame = 7; // номер кадра анимации танка
        bool isFocus;
        
        sf::Texture TileSet;
        TileSet.loadFromFile("images/TileSet.png");
        //спрайт танка, не объект класса танк, это то, что отрисовывается
        sf::Sprite tank(TileSet);
        sf::Sprite enemyes(TileSet);
        sf::Sprite background;
        sf::Texture _background;
        sf::Sprite wall(TileSet);
        wall.setTextureRect(sf::IntRect(30,0,30,30));
        _background.loadFromFile("images/map.png");
        background.setTexture(_background);
        sf::RenderWindow mainView(sf::VideoMode(screen_width,screen_height),"Tanks");
        
        // Create a socket and connect it to localhost on port 1488
        sf::TcpSocket socket;
        socket.connect(sf::IpAddress::LocalHost, 1488);
        sf::Packet question;
        sf::Packet answer;
        int number;
        
        // Send a message to the connected host
        question << -777;
        socket.send(question);
        question.clear();
        
        // Receive an answer from the server
        socket.receive(answer);
        std::cout << "The server said: " << std::endl;
        int start_x; // координаты точки, в которой появляется созданный танк
        int start_y;
        Tank *player = nullptr;
        answer >> player->life >>start_x >> start_y;
        answer.clear();
        
        // объявляем переменные и указатели
        player = new Tank(start_x, start_y, tank_w, tank_h, "tank.png", tank_speed, direction_up, 1);
        
        while(mainView.isOpen()){ // запускаем главный цикл приложения
            clock.restart();
        // обрабатываем события
            sf::Event event;
            while (mainView.pollEvent(event))                           // обработка библиотечных событий событий
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    mainView.close();
                if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                {
                    mainView.close();
                }
                if (event.type == sf::Event::LostFocus) {
                    isFocus = false;
                }
                if (event.type == sf::Event::GainedFocus) {
                    isFocus = true;
                }
            }
        
            mainView.clear(sf::Color::Black);
            
            
        // обрабатываем нажатия клавиатуры
            if (frame < 0) frame = 7;
            if (Keyboard::isKeyPressed(Keyboard::Left) && isFocus) {
                player->direction = direction_left;
                player->update(frame);
                tank.setTextureRect(sf::IntRect(int(frame)*30+30,90,-30,30));
            }
            else if (Keyboard::isKeyPressed(Keyboard::Right) && isFocus) {
                player->direction = direction_right;
                player->update(frame);
                tank.setTextureRect(sf::IntRect(int(frame)*30,90,30,30));
            }
            else if (Keyboard::isKeyPressed(Keyboard::Up) && isFocus)	{
                player->direction = direction_up;
                player->update(frame);
                tank.setTextureRect(sf::IntRect(int(frame)*30,60,30,30));
            }
            else if (Keyboard::isKeyPressed(Keyboard::Down) && isFocus) {
                player->direction = direction_down;
                player->update(frame);
                tank.setTextureRect(sf::IntRect(30*int(frame),90,30,-30));
            }
            tank.setPosition(player->x,player->y);
            
            question << player->life << player->x << player->y;
            socket.send(question);
            question.clear();
            
            time = clock.getElapsedTime().asMilliseconds();
            ttime = milliseconds(time);
            if (ttime<update_time){
                sf::sleep(update_time - ttime);
            }
            
            // отрисовываем
            mainView.draw(background);
            
            
            // отправляем данные серверу
            question << '?';
            socket.send(question);
            question.clear();
            socket.receive(answer);
            answer >> player->life;
            if (player->life > 0) {
                socket.receive(answer);
                answer >> number;
                for (int i = 0; i<number; i++) {
                    int x,y;
                    answer >> x >> y;
                    enemyes.setPosition(x, y);
                    mainView.draw(enemyes);
                }
            } else
            {
             cout << "GAME OVER!";
                return 0;
            }
            answer.clear();
            
            mainView.draw(tank);
            
            mainView.display();
            
        }
    }
    
    return 0;
}