#ifndef MAGEQUEST_SRC_PATHFINDING_PATHFINDER_H_
#define MAGEQUEST_SRC_PATHFINDING_PATHFINDER_H_
namespace PathFinding {
inline constexpr uint16_t MAX_LENGTH = 1024;
struct NodeP {
  PointI position{};
  uint16_t f_cost = 0;
  uint16_t g_cost = 0;
  NodeP* parent = nullptr;
  NodeP() = default;
  NodeP(const NodeP& n) = default;
  NodeP(const PointI& position, uint16_t g_cost, uint16_t h_cost, NodeP* parent)
      : position(position), g_cost(g_cost), f_cost(g_cost + h_cost), parent(parent) {}

  inline bool operator<(const NodeP& o) const { return f_cost < o.f_cost; }
  inline bool operator>(const NodeP& o) const { return f_cost > o.f_cost; }
  inline bool operator==(const NodeP& o) const { return f_cost == o.f_cost; }
  inline bool operator<=(const NodeP& o) const { return f_cost <= o.f_cost; }
};
static inline std::array<NodeP, 1024> node_pool{};
static inline PriorityQueue<NodeP> frontier{};
static inline std::unordered_set<PointI> closedSet{};
static inline PointI star_movement[8] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0},
                                         {1, 0},   {-1, 1}, {0, 1},  {1, 1}};

static inline PointI cross_movement[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

static inline PointI FindNextPoint(NodeP* target_node) noexcept {
  NodeP* node = target_node;

  while (node->parent && node->parent->parent) {
    node = node->parent;
  }

  return node->position;
}

static PointI AStarPathFinding(const PointI& start, const PointI& target) noexcept {
  if (start == target) {
    return {0, 0};
  }
  frontier.clear();
  closedSet.clear();
  frontier.emplace(start, 0, start.dist(target), nullptr);
  uint16_t counter = 0;
  while (!frontier.empty() && counter < MAX_LENGTH) {
    node_pool[counter] = frontier.top();
    auto* current = &node_pool[counter];
    if (current->position == target) {
      return FindNextPoint(current);
    }
    frontier.pop();
    closedSet.insert(current->position);
    for (const auto& dir : star_movement) {
      int newX = current->position.x + dir.x;
      int newY = current->position.y + dir.y;
      PointI new_pos(newX, newY);
      if (!closedSet.contains(new_pos) &&
          COLLISIONS[CURRENT_BACK_GROUND[newX][newY]] != C_SOLID &&
          COLLISIONS[CURRENT_MIDDLE_GROUND[newX][newY]] != C_SOLID && newX >= 0 &&
          newY < CURRENT_MAP_SIZE && newY >= 0 && newX < CURRENT_MAP_SIZE) {
        uint16_t tentative_g_cost = current->g_cost + 1 + RANGE_01(RNG_RANDOM);
        uint16_t h_cost = abs(newX - target.x) + abs(newY - target.y);
        frontier.emplace(new_pos, tentative_g_cost, h_cost, current);
      }
    }
    counter++;
  }
  return {-1, -1};
}
/*
PointI astar_pathfinding_visual(const PointI& start, const PointI& target) {
  if (start == target) {
    return target;
  }
  DrawRectangle(start.x * 48 + DRAW_X, start.y * 48 + DRAW_Y, 48, 48, GREEN);
  DrawRectangle(target.x * 48 + DRAW_X, target.y * 48 + DRAW_Y, 48, 48, RED);

  int max_length = 50;

  frontier.emplace(start, 0, start.dist(target), nullptr);

  while (!frontier.empty() && frontier.size() < max_length) {
    auto* current = new NodeP(frontier.top());
    frontier.pop();

    if (current->position == target) {
      return FindNextPoint(current);
    }

    closedSet.insert(current->position);
    for (const auto& dir : cross_movement) {
      int newX = current->position.x + dir.x;
      int newY = current->position.y + dir.y;
      PointI new_pos(newX, newY);

      if (COLLISIONS[CURRENT_BACK_GROUND[newX][newY]] == C_SOLID) {
        DrawRectangle(newX * 48 + DRAW_X, newY * 48 + DRAW_Y, 48, 48, GRAY);
      }
      if (newX >= 0 && newY < CURRENT_MAP_SIZE && newY >= 0 && newX < CURRENT_MAP_SIZE &&
          !closedSet.contains(new_pos) &&
          COLLISIONS[CURRENT_BACK_GROUND[newX][newY]] != C_SOLID) {
        DrawRectangle(newX * 48 + DRAW_X, newY * 48 + DRAW_Y, 48, 48, ORANGE);
        uint16_t tentative_g_cost = current->g_cost + 1;
        uint16_t h_cost = abs(newX - static_cast<int>(target.x)) +
                          abs(newY - static_cast<int>(target.y));

        frontier.emplace(new_pos, tentative_g_cost, h_cost, current);
      }
    }
  }
  return {-1, -1};
}
 */
inline static bool LineOfSightCheck(PointI start, const Point& endPoint) {
  PointI end = {static_cast<int>(endPoint.x_/48),static_cast<int>(endPoint.y_/48)};
  int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
  int dy = -abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
  int err = dx + dy, e2;

  while (true) {
    if (start.x == end.x && start.y == end.y) break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      start.x += sx;
    }
    if (e2 <= dx) {
      err += dx;
      start.y += sy;
    }

    if (CheckTileCollision((int)start.x, (int)start.y)) return false;
  }
  return true;
}

}  // namespace PathFinding
#endif  //MAGEQUEST_SRC_PATHFINDING_PATHFINDER_H_
