#include <unordered_map>
#include <vector>

using VertexId = int;

// Non-negative weights.
using EdgeWeight = unsigned int;

struct Edge
{
    Edge(const VertexId dest_id, const EdgeWeight weight) : dest_id_(dest_id), weight_(weight)
    {
    }

    VertexId dest_id_;
    EdgeWeight weight_;
};

using EdgeList = std::vector<Edge>;

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