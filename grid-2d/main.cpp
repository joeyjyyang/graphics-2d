#include <cassert>
#include <chrono>
#include <climits>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <SFML/Graphics.hpp>
// Needs to be included after <SFML/Graphics.hpp>!
#include <X11/Xlib.h>

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

std::mutex MTX;

class Grid
{
public:
    Grid(const unsigned int num_rows, const unsigned int num_cols, const unsigned int cell_length, const unsigned int cell_height, const unsigned int outline_thickness) : num_rows_(num_rows), num_cols_(num_cols), cell_length_(cell_length), cell_height_(cell_height), outline_thickness_(outline_thickness), cells_(num_rows_, std::vector<sf::RectangleShape>(num_cols_, sf::RectangleShape(sf::Vector2f(cell_length_, cell_height_)))), visited_(num_rows_, std::vector<bool>(num_cols_, false)), weights_(num_rows, std::vector<unsigned int>(num_cols, UINT_MAX)), weights_text_(num_rows, std::vector<sf::Text>(num_cols, sf::Text())), show_weights_(true), show_path_(true)
    {
        initializeCells();

        if (show_weights_)
        {
            loadFont();
            initializeWeightsText();
        }

        std::cout << "Created grid.\n";
    }

    void initializeCells()
    {
        unsigned int cell_position_x{0};
        unsigned int cell_position_y{0};

        for (unsigned int i = 0; i < num_rows_; i++)
        {
            cell_position_x = 0;

            for (unsigned int j = 0; j < num_cols_; j++)
            {   
                // Can't be const.
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

    void initializeWeightsText()
    {
        for (unsigned int i = 0; i < num_rows_; i++)
        {
            for (unsigned int j = 0; j < num_cols_; j++)
            {
                const auto& weight = weights_[i][j];
                const auto& cell = cells_[i][j];
                const auto& cell_position = cell.getPosition();
                // Can't be const.
                auto& weight_text = weights_text_[i][j];

                weight_text.setFont(font_);
                weight_text.setString(std::to_string(weight));
                weight_text.setFillColor(sf::Color::Blue);
                weight_text.setCharacterSize(16);
                weight_text.setPosition(cell_position.x, cell_position.y);

                assert(weight_text.getString() == std::to_string(weight));
            }
        }
    }

    void setStartCell(const unsigned int start_x, const unsigned int start_y)
    {
        start_x_ = start_x;
        start_y_ = start_y;

        auto& cell = cells_[start_y][start_x];
        cell.setFillColor(sf::Color::Green);

        // Set default weight to 0.
        weights_[start_y][start_x] = 0;
        auto& weight_text = weights_text_[start_y][start_x];
        weight_text.setString(std::to_string(0));

        // Check.
        assert(weights_text_[start_y][start_x].getString() == std::to_string(weights_[start_y][start_x]));
    }

    void setEndCell(const unsigned int end_x, const unsigned int end_y)
    {
        end_x_ = end_x;
        end_y_ = end_y;

        auto& cell = cells_[end_y][end_x];
        cell.setFillColor(sf::Color::Red);
    }

    void addPath(const unsigned int index_x, const unsigned int index_y)
    {
        path_.push_back({index_x, index_y});

        if (show_path_)
        {
            auto& cell = cells_[index_x][index_y];
            cell.setFillColor(sf::Color::Blue);
        }
    }

    const std::vector<std::vector<sf::RectangleShape>>& getCells()
    {
        return cells_;
    }

    const std::vector<std::vector<bool>>& getVisited()
    {
        return visited_;
    }

    const std::vector<std::vector<unsigned int>>& getWeights()
    {
        return weights_;
    }

    const std::vector<std::vector<sf::Text>>& getWeightsText()
    {
        return weights_text_;
    }

    const std::vector<std::pair<unsigned int, unsigned int>>& getPath()
    {
        return path_;
    }

    ~Grid()
    {
        std::cout << "Destroyed grid.\n";
    }

    bool show_weights_;
    bool show_path_;

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
    std::vector<std::vector<bool>> visited_;
    std::vector<std::vector<unsigned int>> weights_;
    std::vector<std::vector<sf::Text>> weights_text_;
    std::vector<std::pair<unsigned int, unsigned int>> path_;
    sf::Font font_;
};

void render(sf::RenderWindow& window, const std::shared_ptr<Grid> grid)
{
    while (window.isOpen())
    {
        window.clear();

        MTX.lock();

        for (unsigned int i = 0; i < NUM_ROWS; i++)
        {
            for (unsigned int j = 0; j < NUM_COLS; j++)
            {
                // Draw grid.
                const auto& cells = grid->getCells();
                window.draw(cells[i][j]);

                // Draw weights (optional).
                if (grid->show_weights_)
                {
                    const auto& weights_text = grid->getWeightsText();
                    window.draw(weights_text[i][j]);
                }
            }
        }

        MTX.unlock();

        window.display();
    }
}

// TO DO: Implement Dijkstra's here.
unsigned int a = 1;
unsigned int b = 1;
void buildPath(std::shared_ptr<Grid> grid)
{
    while (true)
    {
        MTX.lock();

        grid->addPath(a, b);
        a++;
        b++;

        std::cout << a;
        std::cout << b;

        MTX.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(int argc, char const* argv[])
{
    // Make sure cell length and height match; cells needs to be a square!
    assert(CELL_HEIGHT == CELL_LENGTH);

    // Breaks cross-platform support! 
    // Is not reliable, but sometimes needs to calledso X is aware that this is a multi-threaded application.
    XInitThreads();

    // Recommended to create window and in main thread for maximum portability.

    // Create window.
    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGHT), "Welcome to 2D Grid!");

    // Must deactivate window before using in another thread.
    window.setActive(false);

    // Need to allocate grid on heap; otherwise, run out of stack memory!
    std::shared_ptr<Grid> grid = std::make_shared<Grid>(NUM_ROWS, NUM_COLS, CELL_LENGTH, CELL_HEIGHT, OUTLINE_THICKNESS);
    grid->setStartCell(START_X, START_Y);
    grid->setEndCell(END_X, END_Y);

    std::thread render_thread(render, std::ref(window), grid);
    std::thread path_thread(buildPath, grid);

    // Event handling in main thread.
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
    }

    render_thread.join();
    path_thread.join();

    return 0;
}
