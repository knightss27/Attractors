#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

struct Position {
    float x, y, z;
};

std::ostream& operator<<(std::ostream& stream, const Position& vertex)
{
    stream << vertex.x << ", " << vertex.y << ", " << vertex.z;
    return stream;
}

class Attractor : public sf::Drawable {
public:
    std::vector<std::vector<sf::Vertex>> verticeList;
    float dt = 0.01f;
    float a = 5;
    float b = 28;
    float c = 8.0f / 3.0f;
    bool removeTrails = false;

    enum Type {
        Lorenz = 0, FixedPoint
    };

private:
    std::vector<Position> c_positions;
    float center_x, center_y;
    float c_time = 0.0f;
    Type attractor_type;
    sf::RenderWindow window;

public:
    Attractor(Type _type, int _center_x, int _center_y, int numLines, bool _rTrails) {
        center_x = (float)_center_x;
        center_y = (float)_center_y;
        attractor_type = _type;
        removeTrails = _rTrails;
        for (int i = 0; i < numLines; i++) {
            // Generate random values if we have lots of lines
            if (_type == Type::FixedPoint) {
                float randomX = (float)(rand() % 600) - 300;
                float randomY = (float)(rand() % 600) - 300;
                c_positions.push_back({ randomX, randomY, 0.0f });
            }
            else if (_type == Type::Lorenz) {
                float randomX = (float)(rand() % 10) - 5;
                float randomZ = (float)(rand() % 10) - 5;
                c_positions.push_back({ 0.01f + randomX, 0.0f, randomZ });
            }
            
            verticeList.push_back({});
        }
    }

    void update() {
        
        if (removeTrails) {
            if (verticeList[0].size() > 100) {
                for (int i = 0; i < verticeList.size(); i++) {
                    verticeList[i].erase(verticeList[i].begin(), verticeList[i].begin() + 1);
                }
            }
        }
        
        switch (attractor_type)
        {
            case Type::Lorenz:
                update_lorenz();
                break;
            case Type::FixedPoint:
                update_fixed_point();
                break;
            default:
                break;
        }
    }

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates state) const
    {
        for (std::vector<sf::Vertex> vertices : verticeList){
            target.draw(&vertices[0], vertices.size(), sf::LineStrip);
        }
    }

private:
    void update_lorenz() {
        // Messing around with changing a value.
        a += 0.01;
        // std::cout << a << std::endl;
        for (int i = 0; i < c_positions.size(); i++) {
            // Update our position
            float dx = (a * (c_positions[i].y - c_positions[i].x)) * dt;
            float dy = (c_positions[i].x * (b - c_positions[i].z) - c_positions[i].y) * dt;
            float dz = ((c_positions[i].x * c_positions[i].y) - (c * c_positions[i].z)) * dt;

            c_positions[i].x += dx;
            c_positions[i].y += dy;
            c_positions[i].z += dz;

            c_time += dt;

            // std::cout << "(" << c_positions[i] << ") Time: " << time << std::endl;

            // Update the array
            float x_pos = center_x - c_positions[i].x * 9.0f;
            float z_pos = center_y - c_positions[i].z * 7.0f;
            verticeList[i].push_back(sf::Vertex(sf::Vector2f(x_pos, z_pos), sf::Color(x_pos / 600 * 255, i * (255/c_positions.size()), 150)));
        }
    }

    void update_fixed_point() {
        for (int i = 0; i < c_positions.size(); i++) {
            float fixed_a = 0.4f;
            float fixed_b = 0.4f;

            // Update our position
            float dx = (-c_positions[i].y - fixed_a * c_positions[i].y) * dt;
            float dy = (c_positions[i].x - fixed_b * c_positions[i].y) * dt;

            c_positions[i].x += dx;
            c_positions[i].y += dy;

            c_time += dt;

            // std::cout << "(" << c_pos << ") Time: " << time << std::endl;

            // Update the arry
            float x_pos = center_x - c_positions[i].x * 1.f;
            float y_pos = center_y - c_positions[i].y * 1.f;
            verticeList[i].push_back(sf::Vertex(sf::Vector2f(x_pos, y_pos), sf::Color(x_pos / 600 * 255, i*(255 / c_positions.size()), i * (255 / c_positions.size()))));
        }
    }
};

int main()
{   
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(600, 600), "Lorenz Attractor");
    Attractor lorenz(Attractor::Type::FixedPoint, 300, 300, 200, false);

    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        lorenz.update();
        window.draw(lorenz);
        window.display();
    }

    return 0;
}