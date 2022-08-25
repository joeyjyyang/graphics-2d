#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

constexpr int WINDOW_LENGTH{1920};
constexpr int WINDOW_HEIGHT{1200};
constexpr int NUM_ROWS{10};
constexpr int NUM_COLS{16};
constexpr int OUTLINE_THICKNESS{10};
constexpr int TILE_LENGTH{WINDOW_LENGTH / NUM_COLS - OUTLINE_THICKNESS};
constexpr int TILE_HEIGHT{WINDOW_HEIGHT / NUM_ROWS - OUTLINE_THICKNESS};

int main(int argc, char const* argv[])
{
    // Recommended to create window and in main thread for maximum portability.

    // Make sure tile length and height match; tile needs to be a square!
    assert(TILE_HEIGHT == TILE_LENGTH);

    // Create window.
    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGHT), "Welcome to 2D Grid!");

    std::vector<std::vector<sf::RectangleShape>> grid(NUM_ROWS, std::vector<sf::RectangleShape>(NUM_COLS, sf::RectangleShape(sf::Vector2f(TILE_LENGTH, TILE_HEIGHT))));

    //sf::RectangleShape tile(sf::Vector2f(TILE_LENGTH, TILE_HEIGHT));
    //tile.setPosition(500, 500);

    int tile_position_x{0};
    int tile_position_y{0};

    // Set tile positions.
    for (int i = 0; i < NUM_ROWS; i++)
    {
        tile_position_x = 0;

        for (int j = 0; j < NUM_COLS; j++)
        {            
            auto& tile = grid[i][j];
            tile.setPosition(tile_position_x, tile_position_y);
            tile.setOutlineColor(sf::Color::Green);
            tile.setOutlineThickness(OUTLINE_THICKNESS);

            tile_position_x += TILE_LENGTH;
        }

        tile_position_y += TILE_HEIGHT;
    }

    // Draw grid.
    while (window.isOpen())
    {
        window.clear();
        
        for (int i = 0; i < NUM_ROWS; i++)
        {
            for (int j = 0; j < NUM_COLS; j++)
            {
                const auto& tile = grid[i][j];

                window.draw(tile);
            }
        }
        
        window.display();
    }

    return 0;
}
