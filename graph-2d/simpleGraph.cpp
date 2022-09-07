#include <unordered_map>
#include <vector>

using VertexId = unsigned int;

// Non-negative weights.
using EdgeWeight = unsigned int;

// Vector indices represent VertexId.
using EdgeList = std::vector<EdgeWeight>;

using AdjacencyList = std::unordered_map<VertexId, EdgeList>;

// This is minimalistically correct.
//using Graph = AdjacencyList;

class Graph
{
public:
    Graph()
    {
    }

    // Unnecessary to expose in API.
    /*const AdjacencyList& getGraph()
    {
        return graph_;
    }*/

private:
    AdjacencyList graph_;
};

int main(int argc, char* argv[])
{
    Graph graph;

    return 0;
}