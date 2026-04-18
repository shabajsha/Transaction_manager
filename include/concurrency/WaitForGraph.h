#ifndef WAIT_FOR_GRAPH_H
#define WAIT_FOR_GRAPH_H

#include <map>
#include <set>
#include <vector>
#include <cstdint>

class WaitForGraph {
public:
    WaitForGraph();
    ~WaitForGraph();

    // Add dependency: from_txn is waiting for to_txn
    void addEdge(uint32_t from_txn, uint32_t to_txn);

    // Remove specific dependency
    void removeEdge(uint32_t from_txn, uint32_t to_txn);

    // Remove all edges related to a transaction (IMPORTANT for abort)
    void removeTransaction(uint32_t txn_id);

    // Detect if deadlock exists
    bool hasCycle() const;

private:
    // Graph: txn -> set of transactions it is waiting for
    std::map<uint32_t, std::set<uint32_t>> graph_;

    // DFS helper for cycle detection
    bool detectCycleDFS(uint32_t node, std::map<uint32_t, int>& state) const;
};

#endif // WAIT_FOR_GRAPH_H