#include <SFML/Graphics.hpp>
#include <SnakeBlock.h>
#include <vector>
#include <string>
#include <iostream>

#define OFFSET 64
#define N 22


using namespace std;

sf::Texture asset[6];
sf::Sprite sprite[6];

int tri=0;


void generateSnakeBody();
bool checkCollided();
void deleteSnake();
void restart();

std::vector<SnakeBlock*> snake;

float up = 0;
int x,y,n=0;
sf::Transform applePos;
sf::Text text_point;

int main()
{

    int n_text = 0;

    bool game = true;
    sf::Sprite terrain,apple,apple_point;
    sf::RenderWindow window(sf::VideoMode(31*N, 30*N), "Caterpillar");
    sf::Clock clock;
    sf::Font font;
    sf::Transform point;
    sf::Text title,options,author;
    srand((unsigned)time(NULL));


    if (!font.loadFromFile("../arial.ttf"))
    {
        // error...
    }
    else{
        title.setFont(font);
        title.setString("CATERPILLAR");
        title.setCharacterSize(44);
        title.setStyle(sf::Text::Bold);
        title.setPosition(270,0);

        text_point.setFont(font);
        text_point.setString(std::to_string(n));
        text_point.setCharacterSize(30);
        text_point.setStyle(sf::Text::Bold);
        text_point.setPosition(75,10);

        options.setFont(font);
        options.setString("PRESS R TO RESTART");
        options.setCharacterSize(32);
        options.setStyle(sf::Text::Bold);
        options.setPosition(30,620);

        author.setFont(font);
        author.setString("AUTHOR: GALLOTTINO");
        author.setCharacterSize(32);
        author.setStyle(sf::Text::Bold);
        author.setPosition(430,620);
    }

    for(int i=0;i<2;i++){
        for(int j=0;j<3;j++){
            if(!asset[n_text].loadFromFile("../snake-asset.png",sf::IntRect((j)*32, (i)*32, 32, 32))){

            }
            else{
                sprite[n_text].setTexture(asset[n_text]);
                n_text++;
            }
        }
    }
    x = rand()%N *32;
    y = rand()%N *32;
    terrain.setTexture(asset[0]);
    applePos.translate(x,y);
    point.translate(40,10);
    apple.setTexture(asset[3]);
    apple_point.setTexture(asset[3]);
    apple.setOrigin(16,16);
    terrain.setOrigin(16,16);


    restart();

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        sf::Event event;
        up += elapsed.asSeconds();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(game)
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    // DIR{DX,DOWN,SX,UP}
                    snake[0]->setDir(1);

                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    // DIR{DX,DOWN,SX,UP}
                    snake[0]->setDir(3);

                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    // DIR{DX,DOWN,SX,UP}
                    snake[0]->setDir(2);

                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    // DIR{DX,DOWN,SX,UP}
                    snake[0]->setDir(0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
                deleteSnake();
                restart();
                game = true;
            }

        }

        window.clear();

        for(int i=0;i<18;i++){
            for(int j=0;j<18;j++){
                sf::Transform t;
                t.translate(i*32+OFFSET,j*32+OFFSET);
                window.draw(terrain,t);
            }
        }

        for(int i = snake.size()-1;i>=0;i--){
            if(up >= 0.1 && game){
                snake[i]->update(elapsed);
            }
            snake[i]->render(&window);
        }

        if(up >= 0.1){
            if(checkCollided()){
                game = false;
            }
            up = 0;
        }

        window.draw(apple,applePos);
        window.draw(apple_point,point);
        window.draw(title);
        window.draw(text_point);
        window.draw(options);
        window.draw(author);
        window.display();
    }

    return 0;
}

void generateSnakeBody()
{
    //DX,DOWN,SX UP
    int magicMatrix[4][2] = {
                                {-1,0},
                                {0,-1},
                                {1,0},
                                {0,1},
                            };
    SnakeBlock* tail = snake[snake.size()-1];
    int dir = tail->getDir();


    SnakeBlock* body = new SnakeBlock(tail->Getx()+(32)*magicMatrix[dir][0],
                                      tail->Gety()+(32)*magicMatrix[dir][1],
                                      &asset[0]);

    body->setTexture(asset[2]);
    body->setDir(dir);
    tail->setNext(body);
    body->setPrev(tail);
    body->chooseTexture();
    tail->chooseTexture();

    snake.push_back(body);
}

bool checkCollided()
{
    if(snake[0]->Getx() == x && snake[0]->Gety() == y){
        generateSnakeBody();
        x = rand()%(17) * 32 + 64;
        y = rand()%(17) * 32 + 64;
        applePos = applePos.Identity;
        applePos.translate(x,y);
        //printf("%d,%d\n",x,y);
        n++;
        text_point.setString(std::to_string(n));
        return false;
    }

    for(int i=1;i<snake.size();i++){
        if(snake[0]->Getx() - snake[i]->Getx() == 0 &&
           snake[0]->Gety() - snake[i]->Gety() == 0){
                return true;
           }
    }
    return false;
}

void deleteSnake()
{
    int siz = snake.size();
    for(int i=0;i<siz;i++){
        snake.pop_back();
    }

}

void restart()
{
    snake.push_back(new SnakeBlock(32*6+OFFSET,32+OFFSET,&asset[0]));
    snake[0]->setDir(0);
    for(int i=0;i<3;i++)
        generateSnakeBody();
    n=0;
    text_point.setString(std::to_string(n));
}
