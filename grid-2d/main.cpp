#include <cassert>
#include <climits>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>

constexpr unsigned int WINDOW_LENGTH{1920};
constexpr unsigned int WINDOW_HEIGHT{1200};
constexpr unsigned int NUM_ROWS{10};
constexpr unsigned int NUM_COLS{16};
constexpr unsigned int OUTLINE_THICKNESS{10};
constexpr unsigned int CELL_LENGTH{WINDOW_LENGTH / NUM_COLS - OUTLINE_THICKNESS};
constexpr unsigned int CELL_HEIGHT{WINDOW_HEIGHT / NUM_ROWS - OUTLINE_THICKNESS};
constexpr unsigned int START_X{0};
constexpr unsigned int START_Y{0};
constexpr unsigned int END_X{7};
constexpr unsigned int END_Y{6};

// Dijkstra Algorithm
/*
1. Add starting node to priority queue

*/

class Grid
{
public:
    Grid(const unsigned int num_rows, const unsigned int num_cols, const unsigned int cell_length, const unsigned int cell_height, const unsigned int outline_thickness) : num_rows_(num_rows), num_cols_(num_cols), cell_length_(cell_length), cell_height_(cell_height), outline_thickness_(outline_thickness), cells_(num_rows_, std::vector<sf::RectangleShape>(num_cols_, sf::RectangleShape(sf::Vector2f(cell_length_, cell_height_)))), weights_(num_rows, std::vector<unsigned int>(num_cols, UINT_MAX)), visited_(num_rows_, std::vector<bool>(num_cols_, false))
    {
        setCellPositions();
        loadFont();
        std::cout << "Created grid.\n";
    }

    void setCellPositions()
    {
        unsigned int cell_position_x{0};
        unsigned int cell_position_y{0};

        for (unsigned int i = 0; i < num_rows_; i++)
        {
            cell_position_x = 0;

            for (unsigned int j = 0; j < num_cols_; j++)
            {            
                auto& cell = cells_[i][j];
                cell.setPosition(cell_position_x, cell_position_y);
                cell.setOutlineThickness(outline_thickness_);
                cell.setOutlineColor(sf::Color::Black);
                cell.setFillColor(sf::Color::White);

                cell_position_x += cell_length_;
            }

            cell_position_y += cell_height_;
        }
    }

    void loadFont()
    {
        if (!font_.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arialbd.ttf"))
        {
            throw std::invalid_argument("Could not find font file.");
        }
    }

    void setStartCell(const unsigned int start_x, const unsigned int start_y)
    {
        start_x_ = start_x;
        start_y_ = start_y;

        auto& cell = cells_[start_y][start_x];
        cell.setFillColor(sf::Color::Green);
    }

    void setEndCell(const unsigned int end_x, const unsigned int end_y)
    {
        end_x_ = end_x;
        end_y_ = end_y;

        auto& cell = cells_[end_y][end_x];
        cell.setFillColor(sf::Color::Red);
    }

    const std::vector<std::vector<sf::RectangleShape>>& getCells()
    {
        return cells_;
    }

    ~Grid()
    {
        std::cout << "Destroyed grid.\n";
    }

private:
    const unsigned int num_rows_;
    const unsigned int num_cols_;
    const unsigned int cell_length_;
    const unsigned int cell_height_;
    const unsigned int outline_thickness_;
    unsigned int start_x_;
    unsigned int start_y_;
    unsigned int end_x_;
    unsigned int end_y_;
    std::vector<std::vector<sf::RectangleShape>> cells_;
    std::vector<std::vector<unsigned int>> weights_;
    std::vector<std::vector<bool>> visited_;
    sf::Font font_;
};


int main(int argc, char const* argv[])
{
    // Recommended to create window and in main thread for maximum portability.

    // Make sure cell length and height match; cells needs to be a square!
    assert(CELL_HEIGHT == CELL_LENGTH);

    // Need to allocate grid on heap; otherwise, run out of stack memory!
    std::unique_ptr<Grid> grid = std::make_unique<Grid>(NUM_ROWS, NUM_COLS, CELL_LENGTH, CELL_HEIGHT, OUTLINE_THICKNESS);
    grid->setStartCell(START_X, START_Y);
    grid->setEndCell(END_X, END_Y);

    // Create window.
    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGHT), "Welcome to 2D Grid!");

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

                default:
                    break;
            }
        }

        // TO DO: Put into separate thread.
        // Draw grid.
        window.clear();
        
        for (int i = 0; i < NUM_ROWS; i++)
        {
            for (int j = 0; j < NUM_COLS; j++)
            {
                const auto& cells = grid->getCells();
                const auto& cell = cells[i][j];
                window.draw(cell);
            }
        }
        
        window.display();
    }

    return 0;
}
