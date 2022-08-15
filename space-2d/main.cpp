#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

class Planet : public sf::CircleShape
{
public:
    template <typename T>
    Planet(const T radius, const sf::Color& color, const T position_x, const T position_y, T velocity_x = 0, T velocity_y = 0, T acceleration_x = 0, T acceleration_y = 0) : velocity_(velocity_x, velocity_y), acceleration_(acceleration_x, acceleration_y)
    {
        setRadius(radius);
        setFillColor(color);
        setPosition(position_x, position_y);

        std::cout << "Planet created at (" << position_x << ", " << position_y << ")\n";
    }

    sf::Vector2f getVelocity() const
    {
        return velocity_;
    }

    sf::Vector2f getAcceleration() const
    {
        return acceleration_;
    }

    template<typename T>
    void setVelocity(const T velocity_x, const T velocity_y)
    {
        velocity_.x = velocity_x;
        velocity_.y = velocity_y;
    }

    template<typename T>
    void setAcceleration(const T acceleration_x, const T acceleration_y)
    {
        acceleration_.x = acceleration_x;
        acceleration_.y = acceleration_y;
    }

    template <class T>
    void applyMotion(const T dt)
    {
        auto prev_position = getPosition();
        auto prev_velocity = getVelocity();
        auto prev_acceleration = getAcceleration();
        //auto radius = getRadius();

        // Capture dt by reference.
        auto displacement_lambda = [&](const float velocity, const float acceleration) 
        {
            return velocity * dt + 0.5 * acceleration * dt * dt;
        };

        // Capture dt by reference.
        auto velocity_lambda = [&](const float velocity, const float acceleration)
        {
            return velocity + acceleration * dt;
        };

        auto displacement_x = displacement_lambda(prev_velocity.x, prev_acceleration.x);
        auto displacement_y = displacement_lambda(prev_velocity.y, prev_acceleration.y);
        auto velocity_x = velocity_lambda(prev_velocity.x, prev_acceleration.x);
        auto velocity_y = velocity_lambda(prev_velocity.y, prev_acceleration.y);
        // Constant acceleration.
        auto acceleration_x = prev_acceleration.x;
        auto acceleration_y = prev_acceleration.y;

        move(displacement_x, displacement_y);

        setVelocity(velocity_x, velocity_y);
        setAcceleration(acceleration_x, acceleration_y);
    }

    ~Planet() 
    {
        std::cout << "Planet destroyed.\n";
    }

private:
    sf::Vector2f velocity_{0, 0};
    sf::Vector2f acceleration_{0, 0};
};

constexpr int WINDOW_LENGTH{800}; //1920;
constexpr int WINDOW_HEIGHT{600}; //1200;

int main(int argc, char const* argv[])
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGHT), "Welcome to 2D Space!");

    // Allocate planet elements on heap.
    std::vector<std::unique_ptr<Planet>> planets;

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            // Handle event using event type.
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    break;

                // Need to define scope in case statement to be able to create new variables/objects (e.g. planet)!
                case sf::Event::MouseButtonReleased:
                {
                    std::unique_ptr<Planet> planet = std::make_unique<Planet>(20, sf::Color::Green, event.mouseButton.x, event.mouseButton.y);
                    planet->setVelocity(50, 50);
                    planet->setAcceleration(100, 100);
                    // Move ownership of std::unique_ptr; cannot copy!
                    planets.push_back(std::move(planet));
                    break;
                }

                default:
                    break;
            }
        }

        window.clear();

        //
        // Update window below.
        // Recommended not to put window drawing in a separate
        //
        sf::Time elapsed = clock.restart();
        auto dt = elapsed.asSeconds();

        for (auto& planet : planets) {
            planet->applyMotion(dt);
            window.draw(*planet);
        }
        
        window.display();
    }

    return 0;
}