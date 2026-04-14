#include "../../include/concurrency/WaitForGraph.h"

// Constructor
WaitForGraph::WaitForGraph() {}

// Destructor
WaitForGraph::~WaitForGraph() = default;

// Add edge: from_txn → to_txn
void WaitForGraph::addEdge(uint32_t from_txn, uint32_t to_txn) {
    graph_[from_txn].insert(to_txn);
}

// Remove specific edge
void WaitForGraph::removeEdge(uint32_t from_txn, uint32_t to_txn) {
    auto it = graph_.find(from_txn);
    if (it != graph_.end()) {
        it->second.erase(to_txn);
    }
}

// Remove all references of a transaction
void WaitForGraph::removeTransaction(uint32_t txn_id) {
    // Remove outgoing edges
    graph_.erase(txn_id);

    // Remove incoming edges
    for (auto& [node, neighbors] : graph_) {
        neighbors.erase(txn_id);
    }
}

// Detect cycle in graph
bool WaitForGraph::hasCycle() const {
    std::map<uint32_t, int> state;  // 0: unvisited, 1: visiting, 2: visited
    
    for (const auto& [node, _] : graph_) {
        if (state[node] == 0) {
            if (detectCycleDFS(node, state)) {
                return true;
            }
        }
    }
    return false;
}

// DFS helper
bool WaitForGraph::detectCycleDFS(uint32_t node, std::map<uint32_t, int>& state) const {
    state[node] = 1;  // visiting

    auto it = graph_.find(node);
    if (it != graph_.end()) {
        for (uint32_t neighbor : it->second) {

            if (state[neighbor] == 1) {
                return true;  // cycle detected
            }

            if (state[neighbor] == 0) {
                if (detectCycleDFS(neighbor, state)) {
                    return true;
                }
            }
        }
    }

    state[node] = 2;  // visited
    return false;
}