#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

std::vector<std::vector<int>> load_matrix() {
    std::fstream file("data5.txt", std::ios_base::in);
    int n_nodes;
    file >> n_nodes;
    std::vector<std::vector<int>> matrix(n_nodes, std::vector<int>(n_nodes, 0));
    for (int i = 0; i < n_nodes; i++) {
        for (int j = 0; j < n_nodes; j++)
            file >> matrix[i][j];
    }
    return matrix;
}

void bellman_ford() {
    auto matrix = load_matrix();
    std::vector<int> distances(matrix.size(), INT_MAX);
    distances[0] = 0;
    for (int k = 0; k < matrix.size(); k++) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix.size(); j++) {
                if (matrix[i][j] != 0 && distances[i] != INT_MAX && distances[i] + matrix[i][j] < distances[j])
                    distances[j] = distances[i] + matrix[i][j];
            }
        }
    }
    for (int i = 0; i < distances.size(); i++) {
        distances[i] = distances[i] == INT_MAX ? -1 : distances[i];
        std::cout << distances[i] << std::endl;
    }
}

struct NodeState {
    int distance;
    bool visited;
};

void dijkstra_naive() {
    const int starting_node = 0;
    auto matrix = load_matrix();
    std::vector<NodeState> node_state(matrix.size(), NodeState { INT_MAX, false} );
    node_state[starting_node].distance = 0;
    for (int i = 0; i < matrix.size(); i++) {
        int min_i = std::distance(node_state.begin(), 
            std::min_element(node_state.begin(), node_state.end(), [](const NodeState& n1, const NodeState& n2) {
                if (!n1.visited && !n2.visited)
                    return n1.distance < n2.distance;
                else if (!n1.visited)
                    return true;
                else
                    return false;
            }
        ));
        node_state[min_i].visited = true;
        for (int neighbor = 0; neighbor < matrix.size(); neighbor++) {
            if (!node_state[neighbor].visited && matrix[min_i][neighbor] != 0 && node_state[min_i].distance != INT_MAX) {
                int distance = node_state[min_i].distance + matrix[min_i][neighbor];
                if (distance < node_state[neighbor].distance) {
                    node_state[neighbor].distance = node_state[min_i].distance + matrix[min_i][neighbor];
                }
            }
        }
    }
    for (int i = 0; i < node_state.size(); i++) {
        node_state[i].distance = node_state[i].distance == INT_MAX ? -1 : node_state[i].distance;
        std::cout << node_state[i].distance << std::endl;
    }
}

struct Node {
    int  id;
    int dist;
};

class BinaryHeap {
public:
    BinaryHeap(int cap): capacity(cap), size(0), data(new Node[capacity]) {}
    ~BinaryHeap() { if (data) delete [] data; }

    void insert(Node element) {
        int i = size;
        data[i].id = element.id;
        data[i].dist = element.dist;
        while (i != 0 && data[parent(i)].dist > data[i].dist) {
            std::swap(data[i], data[parent(i)]);
            i = parent(i);
        }
        size++;
    }

    Node pop() {
        if (size <= 0)
            throw std::exception("Poped an empty binary heap");
        if (size == 1) {
            size--;
            return data[0];
        }
        Node root = data[0];
        data[0] = data[size - 1];
        size--;
        heapify(0);
        return root;
    }

    void heapify(int i) {
        int l = left_child(i);
        int r = right_child(i);
        int smallest = i;
        if (l < size && data[l].dist < data[smallest].dist)
            smallest = l;
        if (r < size && data[r].dist < data[smallest].dist)
            smallest = r;
        if (smallest != i) {
            std::swap(data[i], data[smallest]);
            heapify(smallest);
        }
    }

    bool is_empty() const { return size == 0; }

private:
    int parent(int i)  const { return (i - 1) / 2; }
    int left_child(int i)  const { return (2 * i + 1); }
    int right_child(int i) const { return (2 * i + 2); }
    
    int size, capacity;
    Node* data;
};

void dijkstra_heap() {
    int starting_node = 0;
    auto matrix = load_matrix();
    std::vector<NodeState> node_state(matrix.size(), NodeState{ INT_MAX, false });
    node_state[starting_node].distance = 0;
    BinaryHeap min_heap(matrix.size());
    min_heap.insert(Node{ starting_node, 0 });
    while (!min_heap.is_empty()) {
        int current_node = min_heap.pop().id;
        if (node_state[current_node].visited) continue;
        node_state[current_node].visited = true;
        for (int neighbor = 0; neighbor < matrix.size(); neighbor++) {
            if (!node_state[neighbor].visited && matrix[current_node][neighbor] != 0 && node_state[current_node].distance != INT_MAX) {
                int distance = node_state[current_node].distance + matrix[current_node][neighbor];
                if (distance < node_state[neighbor].distance) {
                    node_state[neighbor].distance = distance;
                    min_heap.insert(Node { neighbor, distance });
                }
            }
        }
    }
    for (int i = 0; i < node_state.size(); i++) {
        node_state[i].distance = node_state[i].distance == INT_MAX ? -1 : node_state[i].distance;
        std::cout << node_state[i].distance << std::endl;
    }
}

int main(int argc, char** argv) {
    // O(n^3)
    bellman_ford();
    // O(n^2)
    dijkstra_naive();
    // O(E logV)???
    dijkstra_heap();
    return 0;
}
