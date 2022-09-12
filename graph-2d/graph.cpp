#include <unordered_map>
#include <vector>

template <typename T>
using VertexId = T;

template <typename T>
using EdgeWeight = T;

template <typename TId, typename TWeight>
struct Edge
{
    Edge(const VertexId<TId> dest_id, const EdgeWeight<TWeight> weight) : dest_id_(dest_id), weight_(weight)
    {
    }

    VertexId<TId> dest_id_;
    EdgeWeight<TWeight> weight_;
};

template <typename TId, typename TWeight>
using EdgeList = std::vector<Edge<TId, TWeight>>;
//using EdgeList = std::unordered_map<VertexId, EdgeWeight>;

template <typename TId, typename TWeight>
using AdjacencyList = std::unordered_map<VertexId<TId>, EdgeList<TId, TWeight>>;

// This is minimalistically correct.
//template <typename TId, typename TWeight>
//using Graph = AdjacencyList<TId, TWeight>;

template <typename TId, typename TWeight>
class Graph
{
public:
    Graph()
    {
    }

    // Unnecessary to expose in API.
    //const AdjacencyList& getGraph()
    //{
    //    return graph_;
    //}

private:
    AdjacencyList<TId, TWeight> graph_;
};

int main(int argc, char* argv[])
{
    // Non-negative weights.
    Graph<char, unsigned int> graph;

    return 0;
}