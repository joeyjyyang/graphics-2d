// A grid is basically an undirected, weighted graph where all edge weights between adjacent vertices are 1.

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>

constexpr unsigned int NUM_ROWS{5};
constexpr unsigned int NUM_COLS{5};
constexpr unsigned int SRC_ROW{1};
constexpr unsigned int SRC_COL{2};
constexpr unsigned int DEST_ROW{4};
constexpr unsigned int DEST_COL{4};
constexpr unsigned int SRC_WEIGHT{0};

template <typename T>
void printMatrix(std::vector<std::vector<T>>& matrix)
{
    const unsigned int num_rows  = matrix.size();
    const unsigned int num_cols = matrix[0].size();

    for (unsigned int r_i = 0; r_i < num_rows; r_i++)
    {
        for (unsigned int c_i = 0; c_i < num_cols; c_i++)
        {
            std::cout << matrix[r_i][c_i] << " ";
        }

        std::cout << "\n";
    }
}

void printMatrix(std::vector<std::vector<std::pair<int, int>>>& matrix)
{
    const unsigned int num_rows  = matrix.size();
    const unsigned int num_cols = matrix[0].size();

    for (unsigned int r_i = 0; r_i < num_rows; r_i++)
    {
        for (unsigned int c_i = 0; c_i < num_cols; c_i++)
        {
            std::cout << "(" << matrix[r_i][c_i].first << ", " << matrix[r_i][c_i].second << ") ";
        }

        std::cout << "\n";
    }
}

void printPath(std::vector<std::pair<unsigned int, unsigned int>>& path)
{
    for (unsigned int i = 0; i < path.size(); i++)
    {
        std::cout << "(" << path[i].first << ", " << path[i].second << ") -> ";
    }

    std::cout << "\n";
}

int main(int argc, char* argv[])
{
    std::vector<std::pair<int, int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    std::priority_queue<std::pair<unsigned int, std::pair<unsigned int, unsigned int>>, std::vector<std::pair<unsigned int, std::pair<unsigned int, unsigned int>>>, std::greater<std::pair<unsigned int, std::pair<unsigned int, unsigned int>>>> priority_q;

    std::vector<std::vector<char>> grid(NUM_ROWS, std::vector<char>(NUM_COLS, 'o'));
    // Dijkstra's does not work on non-negative weighted graphs; therefore, weights cannot be negative!
    std::vector<std::vector<unsigned int>> weights(NUM_ROWS, std::vector<unsigned int>(NUM_COLS, UINT_MAX));
    std::vector<std::vector<bool>> visited(NUM_ROWS, std::vector<bool>(NUM_COLS, false));
    std::vector<std::vector<std::pair<int, int>>> previous(NUM_ROWS, std::vector<std::pair<int, int>>(NUM_COLS, {-1, -1}));

    // Initialize source vertex.
    grid[SRC_ROW][SRC_COL] = 'a';
    weights[SRC_ROW][SRC_COL] = SRC_WEIGHT;

    // Initialize destination vertex.
    grid[DEST_ROW][DEST_COL] = 'z';

    printMatrix(grid);
    std::cout << "--------\n";
    printMatrix(weights);
    std::cout << "--------\n";
    printMatrix(visited);
    std::cout << "--------\n";
    printMatrix(previous);
    std::cout << "--------\n";

    priority_q.push({SRC_WEIGHT, {SRC_ROW, SRC_COL}});

    while (!priority_q.empty())
    {
        auto current_vertex = priority_q.top();
        priority_q.pop();
        auto current_weight = current_vertex.first;
        auto current_r_i = current_vertex.second.first;
        auto current_c_i = current_vertex.second.second;
        visited[current_r_i][current_c_i] = true;

        // Find all adjacent vertices.
        for (unsigned int i = 0; i < moves.size(); i++)
        {
            auto adj_vertex_r_i = current_r_i + moves[i].first;
            auto adj_vertex_c_i = current_c_i + moves[i].second;

            // Make sure adjacent vertex is within bounds.
            if (adj_vertex_r_i >= 0 && adj_vertex_r_i < NUM_ROWS && adj_vertex_c_i >= 0 && adj_vertex_c_i < NUM_COLS)
            {
                // Make sure adjacent vertex is unvisited.
                if (!visited[adj_vertex_r_i][adj_vertex_c_i])
                {
                    auto adj_vertex_weight = weights[adj_vertex_r_i][adj_vertex_c_i];
                    auto new_weight = current_weight + 1; // Edge weight between adjacent vertices is always 1.

                    // Found new shortest path from source vertex, through current vertex, to adjacent vertex.
                    if (new_weight < adj_vertex_weight)
                    {
                        weights[adj_vertex_r_i][adj_vertex_c_i] = new_weight;
                        previous[adj_vertex_r_i][adj_vertex_c_i] = {current_r_i, current_c_i};
                        priority_q.push({new_weight, {adj_vertex_r_i, adj_vertex_c_i}});
                    }
                }
            }
        }
    }

    printMatrix(grid);
    std::cout << "--------\n";
    printMatrix(weights);
    std::cout << "--------\n";
    printMatrix(visited);
    std::cout << "--------\n";
    printMatrix(previous);
    std::cout << "--------\n";

    // Get shortest path.
    std::vector<std::pair<unsigned int, unsigned int>> shortest_path;

    shortest_path.push_back({DEST_ROW, DEST_COL});
    auto current_r_i{DEST_ROW};
    auto current_c_i{DEST_COL};

    while (current_r_i != SRC_ROW || current_c_i != SRC_COL)
    {
        shortest_path.push_back({previous[current_r_i][current_c_i].first, previous[current_r_i][current_c_i].second});
        auto temp_r_i = previous[current_r_i][current_c_i].first;
        auto temp_c_i = previous[current_r_i][current_c_i].second;
        current_r_i = temp_r_i;
        current_c_i = temp_c_i;
    }

    std::reverse(shortest_path.begin(), shortest_path.end());

    printPath(shortest_path);

    return 0;
}
