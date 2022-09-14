// A grid is basically an undirected, weighted graph where all edge weights between adjacent vertices are 1.

#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <vector>

#include "graph.cpp"

template <typename T>
void printPath(std::vector<T>& path)
{
    for (unsigned int i = 0; i < path.size(); i++)
    {
        std::cout << path[i] << "-> ";
    }

    std::cout << "\n";
}

constexpr VertexId<char> SRC_ID{'a'};
constexpr VertexId<char> DEST_ID{'c'};
constexpr EdgeWeight<unsigned int> SRC_DISTANCE{0};

int main(int argc, char* argv[])
{
    // Non-negative weights.
    Graph<char, unsigned int> graph;

    graph.addBidirectionalEdge('a', 'd', 1);
    graph.addBidirectionalEdge('a', 'b', 6);
    graph.addBidirectionalEdge('b', 'd', 2);
    graph.addBidirectionalEdge('d', 'e', 1);
    graph.addBidirectionalEdge('b', 'e', 2);
    graph.addBidirectionalEdge('b', 'c', 5);
    graph.addBidirectionalEdge('e', 'c', 5);

    std::cout << "-----------------------\n";
    printGraph(graph);
    std::cout << "-----------------------\n";

    std::priority_queue<std::pair<EdgeWeight<unsigned int>, VertexId<char>>, std::vector<std::pair<EdgeWeight<unsigned int>, VertexId<char>>>, std::greater<std::pair<EdgeWeight<unsigned int>, VertexId<char>>>> priority_q;

    std::unordered_map<VertexId<char>, EdgeWeight<unsigned int>> distances;
    std::unordered_map<VertexId<char>, bool> visited;
    std::unordered_map<VertexId<char>, VertexId<char>> previous;

    const auto& vertices = graph.getVertices();

    for (const auto& src_id : vertices)
    {
        distances[src_id] = UINT_MAX;
        visited[src_id] = false;
    }

    distances[SRC_ID] = 0;

    priority_q.emplace(SRC_DISTANCE, SRC_ID);

    while (!priority_q.empty())
    {
        const auto current_vertex = priority_q.top();
        priority_q.pop();

        const auto current_distance = current_vertex.first;
        const auto current_id = current_vertex.second;

        visited[current_id] = true;

        const auto& edges = graph.getEdges(current_id);

        for (const auto& [adj_id, adj_distance] : edges)
        {
            // Make sure adjacent vertex is unvisited.
            if (!visited[adj_id])
            {
                auto distance = distances[adj_id];
                auto new_distance = current_distance + adj_distance;

                // Found new shortest path from source vertex, through current vertex, to adjacent vertex.
                if (new_distance < distance)
                {
                    distances[adj_id] = new_distance;
                    previous[adj_id] = current_id;
                    priority_q.emplace(new_distance, adj_id);
                }
            }
        }
    }

    for (const auto& [key, val] : visited)
    {
        std::cout << key << ": " << val << ", ";
    }
    std::cout << "\n";

    for (const auto& [key, val] : distances)
    {
        std::cout << key << ": " << val << ", ";
    }
    std::cout << "\n";

    for (const auto& [key, val] : previous)
    {
        std::cout << key << ": " << val << ", ";
    }
    std::cout << "\n";

    // Get shortest path.
    std::vector<VertexId<char>> shortest_path;

    shortest_path.emplace_back(DEST_ID);
    auto current_id{DEST_ID};

    while (current_id != SRC_ID)
    {
        shortest_path.emplace_back(previous[current_id]);
        current_id = previous[current_id];
    }

    std::reverse(shortest_path.begin(), shortest_path.end());

    printPath(shortest_path);

    return 0;
}
