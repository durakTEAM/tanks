// ----- The server -----

#include <iterator>
#include <sys/types.h>
#include <memory.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <sfml/Network.hpp>

using namespace std;

//ÒËÒÚÂÏ‡ ÍÎ‡ÒÒÓ‚:


using namespace std;

const int screen_height=600; //‡ÁÏÂ˚ ÔÓÎˇ
const int screen_width=800;

const int tile_x=5; //‡ÁÏÂ˚ Ú‡ÈÎ‡
const int tile_y=5;

const int bullet_w=5; //‡ÁÏÂ˚ ÔÛÎË
const int bullet_h=5;

const int tank_w=30; //‡ÁÏÂ˚ Ú‡ÌÍ‡
const int tank_h=30;

const int direction_right = 1;
const int direction_up = 2;
const int direction_left = 3;
const int direction_down = 4;

const int bullet_speed = 15;
const int tank_speed = 5;

const int start_x=31; // ÍÓÓ‰ËÌ‡Ú˚ ÚÓ˜ÍË, ‚ ÍÓÚÓÓÈ ÔÓˇ‚ÎˇÂÚÒˇ ÒÓÁ‰‡ÌÌ˚È Ú‡ÌÍ
const int start_y=31;

const int number_tiles_y=(screen_height/tile_y);

sf::String TileMap[20] =
{
    "11111111111111111111",
    "2                  1",
    "2                  1",
    "2                  1",
    "2   1        1     1",
    "2   111      111   1",
    "2                  1",
    "2        12        1",
    "2       1 2        1",
    "2      1  2        1",
    "2     1   2        1",
    "2                  1",
    "2                  1",
    "2                  1",
    "2  2            2  1",
    "2  2222   22  222  1",
    "2  2            2  1",
    "2                  1",
    "2                  1",
    "21111111111111111111",
};
//Í‡Ú‡


class Point{
public:
    int x, y;
    Point(int x1, int y1){
        x = x1;
        y = y1;
    }
};

class Dynamic_rect : public Point { //ÔÛÎË
public:
	bool life; //ÌÛÊÌÓ ‰Îˇ ÍÓÂÍÚÌÓÈ ‡·ÓÚ˚ Ò ‚ÂÍÚÓÓÏ ÔÛÎ¸
	int h, w;
    std::string texture;
    int speed, direction;
    Dynamic_rect(int x1, int y1, int w1, int h1, int s1, int d1, bool l1) : Point(x1,y1){
        w = w1;
        h = h1;
		speed = s1;
    	direction = d1;
		life=l1;
        
	}
};

class Tank : public Dynamic_rect{
public:
	bool per_left; bool per_right; bool per_up; bool per_down;
	Tank (int x1, int y1, int w1, int h1, int s1, int d1, bool l1) : Dynamic_rect (x1,y1,w1,h1,s1,d1,l1){
        per_down = per_left = per_right = per_up = 1;
	};
};

bool Collision_tank_X(Tank tank_one, Tank tank_two){
    if (((tank_one.x + tank_one.w)<tank_two.x)&&(tank_one.x < tank_two.x)){
        return true;
    }
    else return false;
}
bool Collision_tank_Y(Tank tank_one, Tank tank_two) {
    if ((((tank_one.y+tank_one.w)>tank_two.y)&&(tank_one.y<tank_two.y))){
        return true;
    }
    else return false;
}

bool Explosion_check( Tank tank, Dynamic_rect bul){
	//if ((bul.x > tank.x)&&(bul.x < (tank.x + tank.w))&&(bul.y>(tank.y + tank.h))&&(bul.y < tank.y)) return true; //ÔÓ‚ÂËÚ¸
	if ((bul.x+bul.w > tank.x)&&(bul.x<tank.x)&&(bul.y+bul.h > tank.y)&&(tank.y+tank.h < bul.y)) return true; //ÔÛÎˇ ÒÎÂ‚‡
    if ((bul.x+bul.w > tank.x)&&(tank.x+tank.w>bul.x)&&(bul.y + bul.h > tank.y)&&(bul.y<tank.y)) return true; //ÔÛÎˇ Ò‚ÂıÛ
    if ((tank.x+tank.w > bul.x)&&(tank.x < bul.x)&&(tank.y+tank.h > bul.y)&&(tank.y<bul.y+bul.h)) return true;//ÔÛÎˇ ÒÔ‡‚‡
    if ((bul.x+bul.w > tank.x)&&(bul.x<tank.x+tank.w)&&(tank.y+tank.h>bul.y)&&(tank.y<bul.y))return true; //ÔÛÎˇ ÒÌËÁÛ
    else return false;
}


void Bullet_Walls (Dynamic_rect bul){
    if ((TileMap[bul.x/tile_x][bul.y/tile_y] == '1')||(TileMap[bul.x/tile_x][bul.y/tile_y] == '2')) {
        bul.life = false;
    }
}



void Bullet_fly (Dynamic_rect bul){//‰‚ËÊÂÌËÂ ÔÛÎË
	if (bul.direction == direction_right){
		bul.x += bullet_speed;
	}
	if (bul.direction == direction_left){
		bul.x -= bullet_speed;
	}
	if (bul.direction == direction_up){
		bul.y += bullet_speed;
	}
	if (bul.direction == direction_down){
		bul.y -= bullet_speed;
	}
}


const int maxln_datas=9;
const int maxln_answers=100;
char datas [maxln_datas] ;
char answers[maxln_answers];
//ÙÛÌÍˆËˇ ÓÚÔ‚ÍË ÓÚ‚ÂÚ‡ ÓÚ ÒÂ‚Â‡ ÍÎËÂÌÚÛ








void update(vector<Tank> players, vector <Dynamic_rect> flying_bullets){
    //‰‚ËÊÂÌËÂ ÔÛÎ¸:
    if (!flying_bullets.empty()){ //ÂÒÚ¸ ÔÛÎË, ˜ÚÓ ÎÂÚˇÚ
        vector<Dynamic_rect>::iterator it;
        it = flying_bullets.begin();
        while (it!=flying_bullets.end()){ //‰Îˇ ‚ÒÂı ÔÛÎ¸
            Bullet_fly(*it); // ‰‚Ë„‡ÂÏ ÔÛÎË
            it++;
        }
    }
    
    //ÔÓ‚ÂËÚ¸ ‚ÒÂ ÔÛÎË Ì‡ ÒÚÓÎÍÌÓ‚ÂÌËÂ:
    
    if (!flying_bullets.empty()){
        for (int i=0; i<flying_bullets.size(); i++){ //ÔÓ‚ÂÍ‡ ÔÓ ‚ÒÂÏ ÔÛÎˇÏ
            for (int j=0; j<players.size(); j++){ //ÔÓıÓ‰ ÔÓ ‚ÒÂÏ Ú‡ÌÍ‡Ï
                if (Explosion_check(players[j], flying_bullets[i])) { //ÔÛÎˇ ÔÓÔ‡Î‡ ‚ Ú‡ÌÍ
                    players[j].life=false; //ÔÓÏÂ˜‡ÂÏ ‰Îˇ Û‰‡ÎÂÌËˇ
                    flying_bullets[i].life=false;
                }
            }
        }
        for (int i = 0; i<flying_bullets.size(); i++) {Bullet_Walls(flying_bullets[i]);}
    }
    
    //ÔÓ‚ÂËÚ¸ ‚ÒÂ Ú‡ÌÍË Ì‡ ÒÚÓÎÍÌÓ‚ÂÌËÂ:
    
    if (players.size()>=2){
        // int destroyed=-1;
        for (int i=0; i<players.size(); i++ ){
            for (int j=1; j<players.size(); j++){
                if ( (i!=j)&&(players[i].life)&&(players[j].life) ){
                    if ( (Collision_tank_X(players[i], players[j])) && (Collision_tank_Y(players[i], players[j]))  ){
                        if (players[i].x+players[i].w >= players[j].x) {//Á‡ÔÂ˘‡ÂÏ ‰‚Ë„‡Ú¸Òˇ ‚ Ì‡Ô‡‚ÎÂÌËË ÒÚÓÎÍÌÓ‚ÂÌËˇ
                            players[i].per_right = 0;
                            players[j].per_left = 0;
                        }
                        if (players[i].y+players[i].w >= players[j].y) {
                            players[i].per_down = 0;
                            players[j].per_up = 0;
                        }else {
                            players[i].per_right=players[i].per_down=players[j].per_left=players[j].per_up = 1;
                        }
                    }
                }
            }
        }
    }
    
    
    //Û‰‡ÎˇÂÏ ‚Ò∏ ÛÌË˜ÚÓÊÂÌÌÓÂ. ÂÒÎË Û‰‡ÎˇÚ¸ ‡Ì¸¯Â, ÔÂÂÔÛÚ‡ÂÚÒˇ ‚Òˇ ÌÛÏÂ‡ˆËˇ Ó·˙ÂÍÚÓ‚
    vector<Dynamic_rect>::iterator iter_bul = flying_bullets.begin(); //ÛÌË˜ÚÓÊ‡ÂÏ ‚ÂÁ‡‚¯ËÂÒˇ ÔÛÎË
    while (iter_bul != flying_bullets.end()) {
        if (iter_bul->life==false) flying_bullets.erase(iter_bul);
        iter_bul++;
    }
    
    
    
	
}

using namespace std;

int main(){
    
    vector <Tank> players;
    vector <Dynamic_rect> flying_bullets;
    
    // Create a listener to wait for incoming connections on port 1488
    sf::TcpListener listener;
    listener.listen(1488);
    // Wait for a connection
    sf::TcpSocket socket;
    sf::Packet answer;
    sf::Packet question;
    listener.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
    // Receive a message from the client
    socket.receive(question);
    int q;
    question >> q;
    if (q == -777) {
        
    }
    // Send an answer
    std::string message = "Welcome, client";
    socket.send(message.c_str(), message.size() + 1);

}