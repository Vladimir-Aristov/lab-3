#include <iostream>
#include <list>
#include <map>
#include <set>
#include <exception>
#include <fstream>
#include <utility>
using namespace std;

template<typename Vertex = int, typename Weight = int>
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

//    Graph(map<Vertex, list<pair<Vertex, Weight>>> const &vertices) {
//        for (auto& it : vertices) {
//            adjList[it.first].push_back(it.second);
//            vertices.insert(it.first);
//        }
//    }

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
            for (auto& it : adjList) {
                if (it.first != key and it.second.find(key) != it.second.end()) {
                    count++;
                }
            }
            if (count == 0) {
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

    // loop
    bool loop(Vertex key) {
        bool result = false;
        try {
            for (auto& it : adjList.at(key)) {
            // throws out_of_range exception if there is no such key
                if (it.first == key) {
                    result = true;
                }
            }
        } catch (out_of_range& err) {
            cout << err.what() << "\n";
        }
        return result;
    }

    // insert vertex
    auto insert_node(Vertex node) {
        return vertices.insert(node);
    }

    // insert or assign vertex
    auto insert_or_assign_node(Vertex node);

    // insert edge
    auto insert_edge(Vertex u, Vertex v, Weight w) {
        bool found = true;
        try {
            if (!vertices.count(v) or !vertices.count(u)) {
                found = false;
                invalid_argument error("not valid vertex");
                throw error;
            }
        } catch (invalid_argument& err) {
            cout << err.what() << "\n";
        }
        auto answer = make_pair(adjList.find(u), found);
        adjList[u].push_back(make_pair(v, w));
        return answer;
    }

    // insert or assign edge
    auto insert_or_assign_edge(Vertex v, Vertex u, Weight w) {
        bool found1 = false;
        bool found2 = false;
        try {
            if (vertices.find(v)) {
                found1 = true;
            }
            if (vertices.find(u)) {
                found2 = true;
            }
            if (!(found1 and found2)) {
                invalid_argument error("invalid vertex");
                throw error;
            }
            // there is a set a vertices in private field
            // so inserting both nodes in any situation won't
            // change anything
            adjList[v].push_back(u, w);
        } catch (invalid_argument& err) {
            cout << err.what() << "\n";
        }
        auto pos = adjList[v].find(make_pair(u, w));
        return pos, found1 and found2;
    }

    // clear edges
    void clear_edges() {
        adjList.clear();
    }

    // clear edges from key
    bool erase_edges_go_from(Vertex key) {
        bool found = true;
        if (!vertices.count(key)) {
            found = false;
        }
        auto it = adjList.find(key);
        adjList.erase(it);
        return found;
    }

    // clear edges go to(Vertex key)
    bool erase_edges_go_to(Vertex key) {
        bool found = true;
        if (!vertices.count(key)) {
            found = false;
        }
        for (auto& it : adjList) {
            for (auto& it1 = it.second.begin(); it1 != it.second.end(); it1++) {
                if (it1->first == key) {
                    adjList.erase(it1);
                }
            }
        }
        return found;
    }

    // erase vertex
    bool erase_node(Vertex key) {
        bool found = true;
        if (!vertices.count(key)) {
            found = false;
        }
        {// making it a local variable
            auto it = adjList.find(key);
            adjList.erase(it);
        }
        for (auto& it : adjList) {
            for (auto& it1 = it.second.begin(); it1 != it.second.end(); it1++) {
                if (it1->first == key) {
                    adjList.erase(it1);
                }
            }
        }
        vertices.erase(key);
        return found;
    }

    // reading from file
    bool load_from_file(string path) {
        ifstream inFile;
        inFile.open(path);
        bool correct = true;
        try {
            if (!inFile) {
                invalid_argument error("incorrect path");
                correct = false;
                throw error;
            }
            Vertex from, to;
            Weight weight;
            while (inFile >> from >> to >> weight) {
                insert_or_assign_edge(from, to, weight);
            }

            inFile.close();
        } catch (invalid_argument& err) {
            cout << err.what() << "\n";
        }
        return correct;
    }

    // saving to file
    void save_to_file(string path) {
        ofstream outFile;
        outFile.open(path);
        try {
            if (!outFile) {
                invalid_argument error("incorrect path");
                throw error;
            }

            for (auto& it : adjList) {
                for (auto& it1 = it.second.begin(); it1 != it.second.end(); it1++) {
                    outFile << it.first << " " << it1->first << " " << it1->second << "\n";
                }

            }
            outFile.close();
        } catch (invalid_argument& err) {
            cout << err.what() << "\n";
        }
    }
};