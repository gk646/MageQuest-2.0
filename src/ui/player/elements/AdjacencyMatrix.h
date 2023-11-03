#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_ADJACENCYMATRIX_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_ADJACENCYMATRIX_H_

struct AdjacencyMatrix {
  inline static constexpr int k_MaxSize = 200;
  inline static constexpr int k_MaxConnections = 5;
  std::unordered_map<int16_t, std::array<int16_t, k_MaxConnections>> connectionsTo;
  std::unordered_map<int16_t, std::array<int16_t, k_MaxConnections>> connectionsFrom;

  AdjacencyMatrix() {
    connectionsTo.reserve(k_MaxSize);
    connectionsFrom.reserve(k_MaxSize);
  }
  //Adds the edge to both maps to allow for fast lookups to all connections to and from a node
  inline void MakeEdge(int16_t from, int16_t to) noexcept {
    if (connectionsTo.contains(from)) {
      for (auto& val : connectionsTo[from]) {
        if (val == -1) {
          val = to;
        }
      }
    } else {
      connectionsTo.insert({from, {to, -1, -1, -1, -1}});
    }

    if (connectionsFrom.contains(to)) {
      for (auto& val : connectionsFrom[to]) {
        if (val == -1) {
          val = from;
        }
      }
    } else {
      connectionsFrom.insert({to, {from, -1, -1, -1, -1}});
    }
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_ADJACENCYMATRIX_H_
