#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{

    // Create the main window
    sf::RenderWindow window( sf::VideoMode(1000, 1000), "S", sf::Style::Default);


    sf::RectangleShape rect (sf::Vector2f(300.0f,300.0f));
     //rect.setFillColor(sf::Color::Red);
    rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);


    sf::Texture iTextureIDLE;
    sf::Texture rTextureRUN;
    sf::Clock clock;
    if (!iTextureIDLE.loadFromFile("idle2.png")) exit(0);

    if (!rTextureRUN.loadFromFile("run1.png")) exit(0);


    rect.setTexture(&iTextureIDLE);


    float moveSpeed = 250.0f;


    while (window.isOpen()){
        sf::Event evnt;
        while (window.pollEvent(evnt))
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


        // --

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

        /*
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            rect.setPosition((float)mousePos.x, (float)mousePos.y); // can also use static_cast<float>(mousePos.x/y)
        }*/







        window.clear();
        window.draw(rect);
        window.display();


    }

    return 0;
}






