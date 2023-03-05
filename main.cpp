#include <iostream>
#include <list>
#include <map>
#include <set>
#include <exception>
using namespace std;

template<typename Vertex, typename Weight>
class Graph
{
private:
    //a map to store the adjacency list
    map<Vertex, list<pair<Vertex, Weight>>> adjList;
    set<Vertex> vertices;
public:
    // constructor
    Graph() = default;

    // add a vertex to the graph
    void addVertex(Vertex v) {
        {
            // if the vertex is not in the graph, add it
            if (adjList.find(v) == adjList.end())
                adjList[v] = list<pair<Vertex, Weight>>();
        }
    }

    // add an edge to the graph
    void addEdge(Vertex u, Vertex v, Weight w) {
        // add the vertices if they are not in the graph
        if (adjList.find(u) == adjList.end()) {
            addVertex(u);
        }
        if (adjList.find(v) == adjList.end()) {
            addVertex(v);
        }

        // add the edge
        adjList[u].push_back(make_pair(v, w));
        adjList[v].push_back(make_pair(u, w));
    }

    // remove a vertex from the graph
    void removeVertex(Vertex v);

    // remove an edge from the graph
    void removeEdge(Vertex u, Vertex v);

    // get the weight of an edge
    Weight getWeight(Vertex u, Vertex v);

    // get the list of adjacent vertices
    list<Vertex> getAdjacentVertices(Vertex v);



    Graph(map<Vertex, list<pair<Vertex, Weight>>> const &vertices) {
        for (auto& it : vertices) {
            adjList[it.first].push_back(it.second);
            vertices.insert(it.first);
        }
    }
    // copy constructor
    Graph(Graph<Vertex, Weight>& other) {
        for (auto& it = other.adjList.begin(); it != adjList.end(); it++) {
            adjList[*it.first].push_back(it->second);
        }
        for (auto& it = other.vertices.begin(); it != other.vertices.end(); it++) {
            vertices.insert(*it);
        }
    }

    // move constructor
    Graph(Graph<Vertex, Weight>&& other) noexcept {
        adjList.clear();
        vertices.clear();
        for (auto& it = other.adjList.begin(); it != adjList.end(); it++) {
            adjList[*it.first].push_back(it->second);
        }
        for (auto& it = other.vertices.begin(); it != other.vertices.end(); it++) {
            vertices.insert(*it);
        }
        other.adjList.clear();
        other.vertices.clear();
    }

    //assignment operator
    Graph& operator=(const Graph<Vertex, Weight>& other) {
        adjList.clear();
        vertices.clear();
        for (auto& it = other.adjList.begin(); it != adjList.end(); it++) {
            adjList[*it.first].push_back(it->second);
        }
        for (auto& it = other.vertices.begin(); it != other.vertices.end(); it++) {
            vertices.insert(*it);
        }
        return *this;
    }

    // move operator
    Graph& operator=(Graph<Vertex, Weight>&& other) {
        if (this != other) {
            adjList.clear();
            vertices.clear();
            for (auto& it = other.adjList.begin(); it != adjList.end(); it++) {
                adjList[*it.first].push_back(it->second);
            }
            for (auto& it = other.vertices.begin(); it != other.vertices.end(); it++) {
                vertices.insert(*it);
            }
            other.adjList.clear();
            other.vertices.clear();
        }
        return *this;
    }

    //is empty
    bool empty() {
        return vertices.empty();
    }

    // size
    size_t size() {
        return vertices.size();
    }

    // clear
    void clear() {
        adjList.clear();
        vertices.clear();
    }

    // swap
    void swap(Graph<Vertex, Weight>& other) {
        Graph<Vertex, Weight> temp = *this;
        *this = other;
        other = temp;
    }

    // begin
    auto begin() {
        return vertices.begin();
    }

    // end
    auto end() {
        return vertices.end();
    }

    // cbegin
    auto cbegin() const {
        return vertices.cbegin();
    }

    // cend
    auto cend() const {
        return vertices.cend();
    }

    // degree_in
    size_t degree_in(Vertex key) {
        size_t count = 0;
        try {
            if (adjList.find(key) != adjList.end()) {
                count = adjList[key].size();
            } else {
                invalid_argument error("vertex hasn't been found");
                throw error;
            }
        } catch (invalid_argument& err) {
            cout << err.what() << "\n";
        }
        return count;
    }

    // degree_out
    size_t degree_out(Vertex key) {
        size_t count = 0;
        try {
            if (adjList.find(key) != adjList.end()) {
                count = adjList[key].size();
            } else {
                invalid_argument error("vertex hasn't been found");
                throw error;
            }
        } catch (invalid_argument& err) {
            cout << err.what() << "\n";
        }
        return count;
    }
};

// remove a vertex from the graph
template<typename Vertex, typename Weight>
void Graph<Vertex, Weight>::removeVertex(Vertex v)
{
    // remove the vertex from the graph
    adjList.erase(v);

    // remove all edges incident to the vertex
    for (auto &it : adjList)
    {
        auto &edges = it.second;
        edges.remove_if([&](const pair<Vertex, Weight> &edge) {
            return edge.first == v;
        });
    }
}

// remove an edge from the graph
template<typename Vertex, typename Weight>
void Graph<Vertex, Weight>::removeEdge(Vertex u, Vertex v)
{
    // remove the edge
    adjList[u].remove_if([&](const pair<Vertex, Weight> &edge) {
        return edge.first == v;
    });
}

// get the weight of an edge
template<typename Vertex, typename Weight>
Weight Graph<Vertex, Weight>::getWeight(Vertex u, Vertex v)
{
    // get the weight of the edge
    for (auto &edge : adjList[u])
    {
        if (edge.first == v)
            return edge.second;
    }
    return Weight();
}

// get the list of adjacent vertices
template<typename Vertex, typename Weight>
list<Vertex> Graph<Vertex, Weight>::getAdjacentVertices(Vertex v)
{
    list<Vertex> adjVertices;
    // get the list of adjacent vertices
    for (auto &edge : adjList[v])
    {
        adjVertices.push_back(edge.first);
    }
    return adjVertices;
}
