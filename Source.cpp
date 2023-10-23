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

// O(n^3)
void bellman_ford() {
    auto matrix = load_matrix();
    std::vector<int> distances(matrix.size(), std::numeric_limits<int>::max());
    distances[0] = 0;
    for (int k = 0; k < matrix.size(); k++) {
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix.size(); j++) {
                if (matrix[i][j] != 0 && distances[i] != std::numeric_limits<int>::max() && distances[i] + matrix[i][j] < distances[j])
                    distances[j] = distances[i] + matrix[i][j];
            }
        }
    }
    for (int i = 0; i < distances.size(); i++) {
        distances[i] = distances[i] == std::numeric_limits<int>::max() ? -1 : distances[i];
        std::cout << distances[i] << std::endl;
    }
}

struct NodeState {
    int distance;
    bool visited;
};

// O(n^2)
void dijkstra_naive() {
    const int starting_node = 0;
    auto matrix = load_matrix();
    std::vector<NodeState> node_state(matrix.size(), NodeState { std::numeric_limits<int>::max(), false} );
    node_state[starting_node].distance = 0;
    for (int i = 0; i < matrix.size(); i++) {
        int min_idx = std::distance(node_state.begin(), 
            std::min_element(node_state.begin(), node_state.end(), [](const NodeState& n1, const NodeState& n2) {
                if (!n1.visited && !n2.visited)
                    return n1.distance < n2.distance;
                else if (!n1.visited)
                    return true;
                else
                    return false;
            }
        ));
        node_state[min_idx].visited = true;
        for (int j = 0; j < matrix.size(); j++) {
            if (!node_state[j].visited && matrix[min_idx][j] != 0 && node_state[min_idx].distance != INT_MAX && node_state[min_idx].distance + matrix[min_idx][j] < node_state[j].distance)
                node_state[j].distance = node_state[min_idx].distance + matrix[min_idx][j];
        }
    }
    for (int i = 0; i < node_state.size(); i++) {
        std::cout << node_state[i].distance << std::endl;
    }
}

// O(E logV)???

int main(int argc, char** argv) {
    dijkstra_naive();
    return 0;
}
