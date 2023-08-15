#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
#include <random>
#include <memory>
#include <list>
float calculateDistance(sf::Vector2f pos1, sf::Vector2f pos2)
{
    float dx = pos1.x-pos2.x;
    float dy = pos1.y-pos2.y;
    return std::sqrt(dx*dx + dy*dy);
}

// Testing QuadTree for collision detection

struct Circle{
    sf::Vector2f pos;
    float radius;

    Circle(const sf::Vector2f& p = {0.0f, 0.0f}, const float r = 1.0f) : pos(p), radius(r)
    {

    }

};

struct Rectangle{

    sf::Vector2f pos;
    sf::Vector2f sizer;

    Rectangle(const sf::Vector2f& p = {0.0f, 0.0f}, const sf::Vector2f& s = {1.0f, 1.0f}) : pos(p), sizer(s)
    {

    }

    constexpr bool contains(const sf::Vector2f& p) const
    {
        return !(p.x < pos.x || p.y < pos.y || p.x >= (pos.x + sizer.x) || p.y >= (pos.y + sizer.y));
    }

    constexpr bool contains(const Rectangle& r) const
    {
        return (r.pos.x >= pos.x) && (r.pos.x + r.sizer.x < pos.x + sizer.x) &&
        (r.pos.y >= pos.y) && (r.pos.y + r.sizer.y < pos.y + sizer.y);
    }

    constexpr bool overlaps(const Rectangle& r) const
    {
        return ( pos.x < r.pos.x + r.sizer.x && pos.x + sizer.x >= r.pos.x &&
        pos.y < r.pos.y + r.sizer.y && pos.y + sizer.y >= r.pos.y );
    }



    constexpr bool contains(const Circle& c) const
    {
        return ( c.pos.x >= pos.x) && ( c.pos.x + c.radius < pos.x + sizer.x ) &&
        ( c.pos.y >= pos.y ) && ( c.pos.y + c.radius < pos.y + sizer.y );
    }

    constexpr bool overlaps(const Circle& c) const
    {
        return ( pos.x < c.pos.x + c.radius && pos.x + sizer.x >= c.pos.x &&
        pos.y < c.pos.y + c.radius && pos.y + sizer.y >= c.pos.y );
    }


};



constexpr size_t MAX_DEPTH = 6;

template <typename OBJECT>
class StaticQuadTree
{
    public:
        size_t m_depth = 0;
        Rectangle rect;
        std::array<Rectangle, 4> rChild;
        std::array<std::shared_ptr<StaticQuadTree<OBJECT>>,4> pChild{};
        std::vector<std::pair<Rectangle, OBJECT>> pItems;


        StaticQuadTree(const Rectangle& size = { {0.0f, 0.0f}, {100.0f, 100.0f} }, const size_t nDepth = 0)
        {
            m_depth = nDepth;
            resize(size);
        }

        void resize(const Rectangle& rArea){
            clear();

            rect = rArea;

            sf::Vector2f childSize = rect.sizer / 2.0f;

            rChild =
            {
                Rectangle(rect.pos, childSize),
                Rectangle({rect.pos.x + childSize.x, rect.pos.y}, childSize),
                Rectangle({rect.pos.x, rect.pos.y + childSize.y}, childSize),
                Rectangle(rect.pos + childSize, childSize)
            };

        }

        void clear()
        {
            // Erase any items stored in this layer
            pItems.clear();

            // Iterate through children, erase them too
            for (int i = 0; i < 4; i++)
            {
                if (pChild[i])
                    pChild[i]->clear();
                pChild[i].reset();
            }
        }

        size_t size() const
        {
            size_t nCount = pItems.size();
            for (int i = 0; i < 4; i++)
                if (pChild[i]) nCount += pChild[i]->size();
            return nCount;
        }


        void insert(const OBJECT& item, const Rectangle& itemSize){
            for (int i =0; i < 4; i++)
            {
                if (rChild[i].contains(itemSize)){
                    if (m_depth + 1 < MAX_DEPTH){
                        if (!pChild[i])
                        {
                            pChild[i] = std::make_shared<StaticQuadTree<OBJECT>>(rChild[i], m_depth+1);
                        }

                        pChild[i]->insert(item, itemSize);
                        return;
                    }
                }


            }

            pItems.push_back({itemSize, item});
        }



        std::list<OBJECT> search(const Rectangle& rArea) const{
            std::list<OBJECT> listItems;
            search(rArea, listItems);
            return listItems;
        }

        void search(const Rectangle& rArea, std::list<OBJECT>& listItems)
        {
            for (const auto& p : pItems)
            {
                if (rArea.overlaps(p.first)) listItems.push_back(p.second);
            }


            for (int i =0; i<4; i++){
                if (pChild[i])
                {
                    if (rArea.contains(rChild[i])) pChild[i]->items(listItems);
                } else if (rChild[i].overlaps(rArea)) pChild->search(rArea, listItems);

            }
        }

        void items(std::list<OBJECT>& listItems) const
        {
            for (const auto& p : pItems){
                listItems.push_back(p.second);
            }

            for (int i = 0; i < 4; i++) if (pChild[i]) pChild[i]->items(listItems);
        }

        std::list<OBJECT> items() const
        {
            std::list<OBJECT> listItems;
            items(listItems);
            return listItems;
        }

        const Rectangle& area()
        {
            return rect;
        }

        bool remove(OBJECT pItem){

            auto it = std::find_if(pItems.begin(),pItems.end(),
            [&pItem](const std::pair<Rectangle,OBJECT>& a)
            {
                return a.second == pItem;
            });

            if (it != pItems.end()){
                pItems.erase(it);
                return true;
            }
            else{
                for (int i = 0; i < 4; i++)
                {
                    if (pChild[i])
                    {
                        if (pChild[i]->remove(pItem)) return true;
                    }
                }
            }

            return false;
        }

};
template <typename OBJECT>
class StaticQuadTreeContainer
{
    public:
        using QuadTreeContainer = std::list<OBJECT>;

        QuadTreeContainer m_allItems;
        StaticQuadTree<typename QuadTreeContainer::iterator> root;

        StaticQuadTreeContainer(const Rectangle& size = { {0.0f, 0.0f}, {100.0f, 100.0f} }, const size_t nDepth=0) : root(size,nDepth){

        }

        void resize(const Rectangle& rArea){
            root.resize(rArea);
        }

        size_t size() const{
            return m_allItems;
        }

        bool empty() const{
            return m_allItems.empty();
        }

        bool clear()
        {
            root.clear();
            m_allItems.clear();
        }

        typename QuadTreeContainer::iterator begin()
        {
            return m_allItems.begin();
        }

        typename QuadTreeContainer::iterator end()
        {
            return m_allItems.end();
        }

        typename QuadTreeContainer::const_iterator cbegin()
        {
            return m_allItems.cbegin();
        }

        typename QuadTreeContainer::const_iterator cend()
        {
            return m_allItems.cend();
        }



        void insert(const OBJECT& item, const Rectangle& itemsize)
        {
            m_allItems.push_back(item);
            root.insert(std::prev(m_allItems.end()), itemsize);
        }


        std::list<typename QuadTreeContainer::iterator> search(const Rectangle& rArea) const
        {
            std::list<typename QuadTreeContainer::iterator> listItemPointers;
            root.search(rArea, listItemPointers);
            return listItemPointers;
        }

        void remove(typename QuadTreeContainer::iterator item)
        {
            root.remove(item);
            m_allItems.erase(item);
        }

};




struct Object
{

    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::Vector2f acc;

    float mass;
    float radius;

    int id;

    sf::CircleShape *circle = nullptr;


    // circle object
    Object (sf::Vector2f point, float rad, sf::Vector2f acceleration, float mass, int id)
    {
        circle = new sf::CircleShape;
        radius = rad;
        circle->setRadius(radius);
        pos = point;

        this->id = id;
        this->acc = acceleration;

        this->mass = radius * radius;

        sf::Vector2f circleCenter(circle->getRadius(), circle->getRadius());
        circle->setOrigin(circleCenter);
        circle->setOutlineThickness(0.9f);
        circle->setOutlineColor(sf::Color::Black);

    }



    void update(float time)
    {
        verletIntegration(pos,acc,time);
        circle->setPosition(pos);

    }

    void verletIntegration(sf::Vector2f pos_fn, sf::Vector2f acceleration, float time)
    {
        acceleration = -velocity * 0.8f;
        velocity += acceleration * time;
        pos += velocity * time;

        if (std::sqrt(velocity.x*velocity.x + velocity.y * velocity.y) < 0.01f)
        {
            velocity = {0.0f, 0.0f};
        }
    }

    bool onCollision2D(Object obj2)
    {
        float dist = calculateDistance(this->pos, obj2.pos);
        float pushDist;
        sf::Vector2f delta = this->pos - obj2.pos;

        if (dist < (this->radius+obj2.radius)) // static resolution for circle
        {
            pushDist = 0.5f * (dist - this->radius - obj2.radius);
            this->pos.x -= pushDist * (delta.x) / dist;
            this->pos.y -= pushDist * (delta.y) / dist;

            obj2.pos.x += pushDist * (delta.x) / dist;
            obj2.pos.y += pushDist * (delta.y) / dist;

            return true;
        }

        return false;
    }

    bool mouseOnObj(sf::Vector2f vect)
    {
        float dist = calculateDistance(this->pos, vect);

        sf::Vector2f delta = this->pos - vect;

        if (dist < (this->radius))
        {
            return true;
        }

        return false;
    }
};


void dynamicResponse(Object* j1, Object* j2)
{
    // normal calculation
    sf::Vector2f collisionNormal = j2->pos - j1->pos;

    //distance calculation using pythagorean theorem
    float distance = calculateDistance(j1->pos, j2->pos);

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
    j1->pos -= correction;
    j2->pos += correction;
}


int main()
{

     // Create the main window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Circle Generator", sf::Style::Default);
    sf::FloatRect boundary(0.0f, 0.0f, window.getSize().x, window.getSize().y);  // Define the boundary rectangle


    sf::Font font;

    if (!font.loadFromFile("./fonts/Silver.ttf")) return 10;

    std::string t = "Press Spacebar to spawn a circle\nPress A to decrease and D to increase the radius of circles\nHold Right Mouse button to launch the circles";

    sf::Text text(t, font, 30);
    text.setPosition(0.0f,0.0f);


    Object *pSelectedObj = nullptr;


    // simple circle
    float circleRadius = 60.0f;


    bool circle_mode = true;

    sf::Clock clock;
    float creationInterval = 0.2f; // Time interval between circle creations (in seconds)
    float elapsedTime = 0.0f; // Elapsed time since the last circle creation


    sf::Vector2f circlePos(window.getSize().x / 2, window.getSize().y / 2);


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 500);


    std::list <Object> circ;
    StaticQuadTree<Object> treeObj;
    Rectangle rScreen = {{0.0f,0.0f}, {window.getSize().x, window.getSize().y}};


    while (window.isOpen())
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        sf::Vector2f mousePosf(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        std::vector<std::pair<Object*,Object*>> collidedCircles;

        window.clear();
        std::string circle_size_text = "Radius and Mass: " + std::to_string( (int) circleRadius);
        sf::Text radius_text(circle_size_text, font, 30);
        radius_text.setPosition(window.getSize().x/2+300,0.0f);

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

                    if (pSelectedObj != nullptr)
                    {
                        pSelectedObj->velocity = 5.0f * ((pSelectedObj->pos) - mousePosf);
                        pSelectedObj = nullptr;
                    }

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





        for (auto& c : circ)
        {
            if (c.circle != nullptr)
                window.draw(*(c.circle));
        }

        window.draw(text);
        window.draw(radius_text);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && elapsedTime >= creationInterval)
        {
            elapsedTime = 0.0f;
            circleRadius -= 10.0f;

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && elapsedTime >= creationInterval)
        {
            elapsedTime = 0.0f;
            circleRadius += 10.0f;

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && elapsedTime >= creationInterval)
        {
            elapsedTime = 0.0f;

            if (circle_mode)
            {
                Object obx (sf::Vector2f((float)mousePos.x, (float)mousePos.y), circleRadius, {0.0f,0.0f}, 1.0f,circ.size());
                treeObj.insert(obx, Rectangle(obx.pos,{obx.radius, obx.radius}));
                circ.emplace_back(obx);
            }


            //std::cout << "x: " << circlex.pos.x << " y: " << circlex.pos.y << std::endl;


            //circle.circle.setPosition((float)mousePos.x, (float)mousePos.y); // can also use static_cast<float>(mousePos.x/y)
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && elapsedTime >= creationInterval)
        {

        }



        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            for (auto& s : circ)
            {
                if (s.mouseOnObj(mousePosf) && pSelectedObj == nullptr)
                {
                    pSelectedObj = &s;
                    break;
                }
            }
        }



        if (pSelectedObj != nullptr)
        {
            float dist = calculateDistance(pSelectedObj->pos, mousePosf);
            if (dist > 300.0f)
            {
                pSelectedObj = nullptr;
                continue;
            }
            sf::VertexArray line(sf::Lines, 10);
            line[0].position = pSelectedObj->pos;
            line[0].color = sf::Color::Green;
            line[1].position = mousePosf;
            line[1].color = sf::Color::Green;
            window.draw(line);

        }



        window.display();





        // movement
        /*
        void Render()
        {

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

        }



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





    }

    return 0;
}






