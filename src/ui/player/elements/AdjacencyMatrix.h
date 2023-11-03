#ifndef MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_ADJACENCYMATRIX_H_
#define MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_ADJACENCYMATRIX_H_

struct AdjacencyMatrix {
  inline static constexpr int kMaxSize = 250;
  std::unordered_map<int16_t, std::array<int16_t, 4>> edgeList;
  AdjacencyMatrix() { edgeList.reserve(kMaxSize); }
  inline void MakeEdge(int16_t from, int16_t to) noexcept {
    if (edgeList.contains(from)) {
      for (auto& val : edgeList[from]) {
        if (val == -1) {
          val = to;
        }
      }
    } else {
      edgeList.insert({from, {to, -1, -1, -1}});
    }
  }
  inline bool IsConnectionBetween(int16_t from, int16_t to) noexcept {
    if (edgeList.contains(from)) {
      const auto& edges = edgeList[from];
      return std::find(edges.begin(), edges.end(), to) != edges.end();
    } else if (edgeList.contains(to)) {

    }
    return false;
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_ELEMENTS_ADJACENCYMATRIX_H_
