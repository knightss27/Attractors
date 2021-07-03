#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

struct Position {
    float x, y, z;
};

std::ostream& operator<<(std::ostream& stream, const Position& vertex)
{
    stream << vertex.x << ", " << vertex.y << ", " << vertex.z;
    return stream;
}

class Attractor {
public:
    std::vector<sf::Vertex> vertices;
    float dt = 0.001f;
    float a = 10;
    float b = 28;
    float c = 8.0f / 3.0f;

private:
    Position c_pos = { 0.01f, 0.0f, 0.0f };
    float center_x, center_y;
    float time = 0.0f;
    
public:
    Attractor(int _center_x, int _center_y) {
        center_x = (float)_center_x;
        center_y = (float)_center_y;
    }

    void update() {
        // Update our position
        float dx = (a * (c_pos.y - c_pos.x)) * dt;
        float dy = (c_pos.x * (b - c_pos.z) - c_pos.y) * dt;
        float dz = ((c_pos.x * c_pos.y) - (c * c_pos.z)) * dt;
    
        c_pos.x += dx;
        c_pos.y += dy;
        c_pos.z += dz;

        time += dt;

        std::cout << "(" << c_pos << ") Time: " << time << std::endl;

        // Update the array
        float x_pos = center_x - c_pos.x * 9.0f;
        float y_pos = center_y - c_pos.z * 7.0f;
        vertices.push_back(sf::Vertex(sf::Vector2f(x_pos, y_pos), sf::Color(x_pos/600 * 255, 100, 150)));
    }
};

int main()
{   
    sf::RenderWindow window(sf::VideoMode(600, 600), "Lorenz Attractor");
    Attractor lorenz(300, 500);

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
        window.draw(&lorenz.vertices[0], lorenz.vertices.size(), sf::LineStrip);
        window.display();
    }

    return 0;
}