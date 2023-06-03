#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>


void Render()
{
    /*
    //-- -- -- FOR GAME MAKING -- will use later
    sf::RectangleShape rect (sf::Vector2f(300.0f,300.0f));
     //rect.setFillColor(sf::Color::Red);
    rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
    sf::Vector2f position(200.f, 200.f); // Create a vector for the desired position
    rect.setPosition(position);


    sf::Texture iTextureIDLE;
    sf::Texture rTextureRUN;

    if (!iTextureIDLE.loadFromFile("idle2.png")) exit(0);
    if (!rTextureRUN.loadFromFile("run1.png")) exit(0);
    rect.setTexture(&iTextureIDLE);

    // -- -- --
    */
}

float calculateDistance(sf::Vector2f pos1, sf::Vector2f pos2)
{
    float dx = pos1.x-pos2.x;
    float dy = pos1.y-pos2.y;

    return std::sqrt(dx*dx + dy*dy);

}


float calculateVelocity()
{

}

struct CircleObj
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f acc;
    float radius;

    int id;

    sf::CircleShape circle;

    CircleObj (sf::Vector2f p, sf::Vector2f v, float rad, int id)
    {
        pos = p;
        vel = v;
        radius = rad;
        this->id = id;
        display_circ();
    }

    void display_circ()
    {
        circle = sf::CircleShape(radius);
        sf::Vector2f circleCenter(circle.getRadius(), circle.getRadius());
        circle.setOrigin(circleCenter);
        sf::Vector2f circlePos(pos.x, pos.y);
        circle.setPosition(circlePos);

    }

    void velocity2D(sf::Vector2f velocity)
    {
        circle.move(velocity);
    }

    bool onCollision2D(CircleObj other)
    {
        float dist = calculateDistance(this->pos, other.pos);
        float pushDist;
        if (dist <= (this->radius+other.radius))
        {
            pushDist = 0.5f * (dist - this->radius - other.radius);
            this->pos.x -= pushDist * (this->pos.x - other.pos.x) /dist;
            this->pos.y -= pushDist * (this->pos.y - other.pos.y) /dist;

            other.pos.x += pushDist * (this->pos.x - other.pos.x) /dist;
            other.pos.y += pushDist * (this->pos.y - other.pos.y) /dist;

            return true;
        }

        return false;
    }




};


int main()
{
     // Create the main window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Circle Generator", sf::Style::Default);

    std::vector <CircleObj> circ;
    sf::Clock clock;

    float moveSpeed = 350.0f;

    // simple circle
    float circleRadius = 30.0f;

    float creationInterval = 0.01f; // Time interval between circle creations (in seconds)
    float elapsedTime = 0.0f; // Elapsed time since the last circle creation


    sf::Vector2f circlePos(window.getSize().x / 2, window.getSize().y / 2);


    while (window.isOpen())
    {
        window.clear();

        sf::Event evnt;
        while (window.pollEvent(evnt)) // events ie. close/resize/character
        {
            switch(evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, evnt.size.width, evnt.size.height)));
                break;

            case sf::Event::TextEntered:
                if (evnt.text.unicode < 128){

                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;



        for (auto& c : circ)
        {
            for (auto& o : circ)
            {
                if (c.id != o.id)
                {
                    if (c.onCollision2D(o))
                    {
                            c.display_circ();
                            c.circle.setFillColor(sf::Color::Red);
                    }
                }
            }

            window.draw(c.circle);
        }



        window.display();



        // movement
        /*

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            float deltaTime = clock.restart().asSeconds();
            rect.move(-moveSpeed*deltaTime, 0.0f);
            rect.setTexture(&rTextureRUN);
            rect.setScale(-1.0f, 1.0f);
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            float deltaTime = clock.restart().asSeconds();
            rect.move(moveSpeed*deltaTime, 0.0f);
            rect.setTexture(&rTextureRUN);
            rect.setScale(1.0f, 1.0f);

        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            float deltaTime = clock.restart().asSeconds();
            rect.move(0.0f, -moveSpeed*deltaTime);
            rect.setTexture(&rTextureRUN);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            float deltaTime = clock.restart().asSeconds();
            rect.move(0.0f, moveSpeed*deltaTime);
            rect.setTexture(&rTextureRUN);
        }

        else rect.setTexture(&iTextureIDLE);

        */



        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && elapsedTime >= creationInterval)
        {
            elapsedTime = 0.0f;
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            CircleObj circlex (sf::Vector2f((float)mousePos.x, (float)mousePos.y), sf::Vector2f(0.0f,0.0f), circleRadius, circ.size());

            circ.emplace_back(circlex);

            //std::cout << "x: " << circlex.pos.x << " y: " << circlex.pos.y << std::endl;


            //circle.circle.setPosition((float)mousePos.x, (float)mousePos.y); // can also use static_cast<float>(mousePos.x/y)
        }
    }

    return 0;
}






