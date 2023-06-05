#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>

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



struct CircleObj
{
    sf::Vector2f pos_i;
    sf::Vector2f pos_f;
    sf::Vector2f velocity;
    sf::Vector2f acc;

    float mass;
    float radius;

    int id;

    sf::CircleShape *circle = new sf::CircleShape;

    CircleObj (sf::Vector2f point, float rad, sf::Vector2f acceleration, float mass, int id)
    {
        radius = rad;
        circle->setRadius(radius);

        pos_f = point;
        pos_i = point;
        this->id = id;
        this->acc = acceleration;

        this->mass = radius*10.0f;

        sf::Vector2f circleCenter(circle->getRadius(), circle->getRadius());
        circle->setOrigin(circleCenter);
        circle->setOutlineThickness(0.9f);
        circle->setOutlineColor(sf::Color::Black);

    }

    void update(float time)
    {
        verletIntegration(pos_i,pos_f,acc,time);
        circle->setPosition(pos_f);
    }

    void verletIntegration(sf::Vector2f pos_i, sf::Vector2f pos_fn, sf::Vector2f acceleration, float time)
    {
        acceleration = -velocity * 0.8f;
        velocity += acceleration * time;
        pos_f += velocity * time;

        if (std::sqrt(velocity.x*velocity.x + velocity.y + velocity.y) < 0.01f)
        {
            velocity = {0.0f, 0.0f};
        }


    }

    bool onCollision2D(CircleObj obj2)
    {
        float dist = calculateDistance(this->pos_f, obj2.pos_f);
        float pushDist;
        sf::Vector2f delta = this->pos_f - obj2.pos_f;

        if (dist < (this->radius+obj2.radius)) // static resolution
        {
            pushDist = 0.5f * (dist - this->radius - obj2.radius);
            this->pos_f.x -= pushDist * (delta.x) / dist;
            this->pos_f.y -= pushDist * (delta.y) / dist;

            obj2.pos_f.x += pushDist * (delta.x) / dist;
            obj2.pos_f.y += pushDist * (delta.y) / dist;

            return true;
        }

        return false;
    }




    bool mouseOnCircle(sf::Vector2f vect)
    {
        float dist = calculateDistance(this->pos_f, vect);

        sf::Vector2f delta = this->pos_f - vect;

        if (dist < (this->radius*2))
        {
            return true;
        }

        return false;
    }


};


void dynamicResponse(CircleObj* j1, CircleObj* j2)
{
    // normal calculation
    sf::Vector2f collisionNormal = j2->pos_f - j1->pos_f;

    //distance calculation using pythagorean theorem
    float distance = calculateDistance(j1->pos_f, j2->pos_f);

    if (distance != 0.0f) collisionNormal /= distance; // avoid div by 0

    sf::Vector2f collisionTangent(-collisionNormal.y, collisionNormal.x);

        // Tangential Velocity calculation (90 degrees to the collision) using dot product
    float dotVelTan1 = j1->velocity.x * collisionTangent.x + j1->velocity.y * collisionTangent.y;
    float dotVelTan2 = j2->velocity.x * collisionTangent.x + j2->velocity.y * collisionTangent.y;

        // Normal Velocity calculation (180 degrees to the collision) using dot product
    float dotVelNorm1 = j1->velocity.x * collisionNormal.x + j1->velocity.y * collisionNormal.y;
    float dotVelNorm2 = j2->velocity.x * collisionNormal.x + j2->velocity.y * collisionNormal.y;

        // Momentum Calculation using the mass and dot products of normal
    float momentum_j1 = (dotVelNorm1 * (j1->mass - j2->mass) + 2.0f * j2->mass * dotVelNorm2) / (j1->mass + j2->mass);
    float momentum_j2 = (dotVelNorm2 * (j2->mass - j1->mass) + 2.0f * j1->mass * dotVelNorm1) / (j1->mass + j2->mass);


        // update the velocity of the objects
    j1->velocity = collisionTangent * dotVelTan1 + collisionNormal * momentum_j1;
    j2->velocity = collisionTangent * dotVelTan2 + collisionNormal * momentum_j2;

    // Adjust the positions slightly to prevent overlap
    float overlap = (j1->radius + j2->radius) - distance;
    sf::Vector2f correction = collisionNormal * (overlap * 0.5f);
    j1->pos_f -= correction;
    j2->pos_f += correction;
}


int main()
{

     // Create the main window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Circle Generator", sf::Style::Default);
    sf::FloatRect boundary(0.0f, 0.0f, window.getSize().x, window.getSize().y);  // Define the boundary rectangle
    std::vector <CircleObj> circ;


    sf::Clock clock;

    CircleObj *pSelectedCirc = nullptr;

    float moveSpeed = 150.0f;

    // simple circle
    float circleRadius = 60.0f;

    float creationInterval = 0.3f; // Time interval between circle creations (in seconds)
    float elapsedTime = 0.0f; // Elapsed time since the last circle creation


    sf::Vector2f circlePos(window.getSize().x / 2, window.getSize().y / 2);


    while (window.isOpen())
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosf(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        std::vector<std::pair<CircleObj*,CircleObj*>> collidedCircles;
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

            case sf::Event::MouseButtonReleased:
                if (evnt.mouseButton.button == sf::Mouse::Right)
                {

                    if (pSelectedCirc != nullptr)
                    {
                        pSelectedCirc->velocity = 5.0f * ((pSelectedCirc->pos_f) - mousePosf);
                        pSelectedCirc = nullptr;
                    }


                    break;
                }

                if (evnt.mouseButton.button == sf::Mouse::Left)
                {
                    pSelectedCirc = nullptr;
                    break;
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;


        for (auto& c : circ)
        {
            c.update(deltaTime);
        }


        for (auto& c : circ)
        {
            for (auto& o : circ)
            {
                if (c.id != o.id)
                {
                    if (c.onCollision2D(o))
                    {
                        dynamicResponse(&c,&o);
                    }
                }
            }

        }


        window.clear();
        for (auto& c : circ)
        {
            window.draw(*(c.circle));
        }









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


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && elapsedTime >= creationInterval)
        {
            elapsedTime = 0.0f;
            circleRadius -= 10.0f;
            std::cout<<circleRadius<<std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && elapsedTime >= creationInterval)
        {
            elapsedTime = 0.0f;
            circleRadius += 10.0f;
            std::cout<<circleRadius<<std::endl;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && elapsedTime >= creationInterval)
        {
            elapsedTime = 0.0f;
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            CircleObj circlex (sf::Vector2f((float)mousePos.x, (float)mousePos.y), circleRadius, {0.0f,0.0f}, 1.0f,circ.size());

            circ.emplace_back(circlex);

            //std::cout << "x: " << circlex.pos.x << " y: " << circlex.pos.y << std::endl;


            //circle.circle.setPosition((float)mousePos.x, (float)mousePos.y); // can also use static_cast<float>(mousePos.x/y)
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {


            for (auto& s : circ)
            {
                if (s.mouseOnCircle(mousePosf))
                {
                    pSelectedCirc = &s;

                    pSelectedCirc->pos_f = mousePosf;

                    break;
                }

            }

            //std::cout << "x: " << circlex.pos.x << " y: " << circlex.pos.y << std::endl;


             // can also use static_cast<float>(mousePos.x/y)
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            for (auto& s : circ)
            {
                if (s.mouseOnCircle(mousePosf) && pSelectedCirc == nullptr)
                {
                    pSelectedCirc = &s;
                    break;
                }


            }

            //std::cout << "x: " << circlex.pos.x << " y: " << circlex.pos.y << std::endl;


             // can also use static_cast<float>(mousePos.x/y)
        }

        if (pSelectedCirc != nullptr)
        {
            sf::VertexArray line(sf::Lines, 10);
            line[0].position = pSelectedCirc->pos_f;
            line[0].color = sf::Color::Green;
            line[1].position = mousePosf;
            line[1].color = sf::Color::Green;
            window.draw(line);

        }

        window.display();


    }

    return 0;
}






