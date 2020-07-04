#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <fstream>
#include <unordered_map>

using namespace std;

enum State {UNVISITED, VISITING, VISITED};

class Node {
  public:
    int id;
    State visited;
    static unordered_map<int, Node> mapping;
    static Node& getNodeById(int id) {
      if (mapping.find(id) != mapping.end()) {
        return mapping.find(id)->second;
      } else {
        auto node = Node(id);
        mapping.emplace(id, node);
        return mapping.find(id)->second;
      }
    }
  private:
    Node(int _id) : id(_id), visited(State::UNVISITED) { }
    friend ostream& operator<<(ostream& out, const Node& node) {
      out << "id: " << node.id << '\t' << node.visited << '\t';
      return out;
    }
};

unordered_map<int, Node> Node::mapping = {};

auto createGraph(const vector<pair<int,int>>& edges, int V, bool isDirected) {
  vector<list<int>> graph(V);
  for(auto& edge:edges) {
    Node::getNodeById(edge.first);
    Node::getNodeById(edge.second);
    auto& lst = graph[edge.first];
    lst.push_back(edge.second);
    if (!isDirected) {
      auto& lst2 = graph[edge.second];
      lst2.push_back(edge.first);
    }
  }
  return graph;
}

void printGraph(const vector<list<int>>& graph) {
  int i = 0;
  for (auto& nodeList:graph) {
    cout << i << "-> ";
    for (auto& item:nodeList) {
      cout << Node::getNodeById(item);
    }
    cout << endl;
    i++;
  }
}

void visit(vector<list<int>>& graph, const int index) {
  cout << "visit " << index << endl;
  Node& n = Node::getNodeById(index);
  n.visited = State::VISITING;
  for (auto& id:graph[index]) {
    auto& node = Node::getNodeById(id);
    if (node.visited == UNVISITED) {
      node.visited = VISITING;
      visit(graph, node.id);
    }
  }
  n.visited = State::VISITED;
}

void doDfs(vector<list<int>>& graph) {
  for(auto it = graph.begin(); it != graph.end(); ++it) {
    if (Node::getNodeById(it - graph.begin()).visited == State::UNVISITED) {
      visit(graph, it - graph.begin());
    }
  }
}

int main() {
  stringstream ss;
  int tc;
  string line;

  fstream fin("graph.txt");

  getline(fin, line);
  ss << line;
  ss >> tc;
  ss.clear();

  for (int i = 0; i < tc; ++i) {
    int E, V;
    getline(fin, line);
    ss << line;
    ss >> V;
    ss.clear();

    getline(fin, line);
    ss << line;
    ss >> E;
    ss.clear();
    
    vector<pair<int, int>> edges(E);
    for (int j = 0; j < E; ++j) {
      int e1, e2;
      getline(fin, line);
      ss << line;
      ss >> e1 >> e2;
      edges[j] = make_pair(e1, e2);
      ss.clear();
    }
    auto&& graph = createGraph(edges, V, true);
    printGraph(graph);
    doDfs(graph);
  }
}
