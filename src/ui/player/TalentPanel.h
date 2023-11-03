#ifndef MAGEQUEST_SRC_UI_PLAYER_TALENTPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_TALENTPANEL_H_

#include "elements/TalentNode.h"
#include "elements/AdjacencyMatrix.h"

struct TalentPanel final : public Window {
  static AdjacencyMatrix matrix;
  static std::vector<TalentNode> talents;
  Point middlePoint{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
  TalentPanel()
      : Window(0, 0, (int)1920, (int)1080, 0, nullptr, KEY_N, sound::EMPTY_SOUND,
               sound::EMPTY_SOUND) {}
  void OnOpen() final { middlePoint = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}; }
  void Draw() noexcept {
    WINDOW_LOGIC()
    DragLogic();
    cxstructs::now();
    auto id = textures::ui::talentpanel::TALENT_BACKGROUND_TILE.id;
    for (int i = 0; i < 42; i++) {
      for (int j = 0; j < 24; j++) {
        DrawTextureProFastUltra(id, i * TILE_SIZE, j * TILE_SIZE);
      }
    }
    DrawConnections();
    DrawTalentNodes(middlePoint);
    //cxstructs::printTime<std::chrono::microseconds>();
  }
  void Update() noexcept {
    WINDOW_UPDATE();
    for (auto& node : talents) {
      node.Update();
    }
  }
  //Removes the effects of all active talents
  inline void RemoveEffects() noexcept {
    auto& stats = PLAYER_STATS;
    for (const auto& t : talents) {
      if (t.isActivated) {
        stats.UnEquipItem(t.talent.effects);
      }
    }
  }
  //Applies all effects from talents
  inline void ApplyEffects() noexcept {
    auto& stats = PLAYER_STATS;
    for (const auto& t : talents) {
      if (t.isActivated) {
        stats.EquipItem(t.talent.effects);
      }
    }
  }

 private:
  inline void DrawConnections() const noexcept {
    for (const auto& pair : matrix.edgeList) {
      const auto& startNode = talents[pair.first];
      for (const auto& nodeID : pair.second) {
        if (nodeID == -1) break;
        DrawTalentConnection(middlePoint, startNode, talents[nodeID]);
      }
    }
  }
  inline void DragLogic() noexcept {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      auto delta = GetMouseDelta();
      middlePoint.x() += delta.x;
      middlePoint.y() += delta.y;
    }
  }
  static inline bool CheckValidTalent(int16_t talentID) {
    if (talentID < 4) {
      return true;
    }
    const auto& edgeList = matrix.edgeList;
    if(edgeList.contains(talentID)){
      for (const auto tId : edgeList.at(talentID)) {
        if (tId == -1) break;
        if (talents[tId].isActivated) {
          return true;
        }
      }
    }
    for (const auto& [from, toList] : edgeList) {
      if (std::find(toList.begin(), toList.end(), talentID) != toList.end()) {
        if (talents[from].isActivated) {
          return true;
        }
      }
    }
    return false;
  }
  //Draws all talent-nodes with their correct texture, state and size
  static inline void DrawTalentNodes(const Point& mid) noexcept {
    for (auto& node : talents) {
      if (node.sizeType == TalentSize::NORMAL) {
        if (node.isActivated) {
          node.Draw(textures::ui::talentpanel::NODE_GREEN, mid.x(), mid.y());
        } else if (CheckValidTalent(node.nodeID)) {
          node.Draw(textures::ui::talentpanel::NODE_PURPLE, mid.x(), mid.y());
        } else {
          node.Draw(textures::ui::talentpanel::NODE_ORANGE, mid.x(), mid.y());
        }
      } else if (node.sizeType == TalentSize::BIG) {
        if (node.isActivated) {
          node.Draw(textures::ui::talentpanel::TALENT_BIG_GREEN, mid.x() - 9,
                    mid.y() - 9);
        } else if (CheckValidTalent(node.nodeID)) {
          node.Draw(textures::ui::talentpanel::TALENT_BIG_PURPLE, mid.x() - 9,
                    mid.y() - 9);
        } else {
          node.Draw(textures::ui::talentpanel::TALENT_BIG, mid.x() - 9, mid.y() - 9);
        }
      } else if (node.sizeType == TalentSize::MID) {
        if (node.isActivated) {
          node.Draw(textures::ui::talentpanel::TALENT_MID_GREEN, mid.x() - 7,
                    mid.y() - 7);
        } else if (CheckValidTalent(node.nodeID)) {
          node.Draw(textures::ui::talentpanel::TALENT_MID_PURPLE, mid.x() - 7,
                    mid.y() - 7);
        } else {
          node.Draw(textures::ui::talentpanel::TALENT_MID, mid.x() - 7, mid.y() - 7);
        }
      }
    }
  }
  //Draws a correctly colored line of images from on talent to the other
  static inline void DrawTalentConnection(const Point& middlePoint,
                                          const TalentNode& requirement,
                                          const TalentNode& nextOne) {
    int x0 =
        requirement.basePoint.x() - (requirement.sizeType != TalentSize::NORMAL ? 2 : 0);
    int y0 =
        requirement.basePoint.y() - (requirement.sizeType != TalentSize::NORMAL ? 2 : 0);
    int x1 = nextOne.basePoint.x() - (requirement.sizeType != TalentSize::NORMAL ? 2 : 0);
    int y1 = nextOne.basePoint.y() - (requirement.sizeType != TalentSize::NORMAL ? 2 : 0);

    int dx = std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int counter = 0;
    const Texture* connectionImage;

    if (requirement.isActivated && nextOne.isActivated) {
      connectionImage = &textures::ui::talentpanel::CONNECTION_GREEN;
    } else if (requirement.isActivated) {
      connectionImage = &textures::ui::talentpanel::CONNECTION_ORANGE;
    } else if (!nextOne.isActivated) {
      connectionImage = &textures::ui::talentpanel::CONNECTION_RED;
    } else {
      connectionImage = &textures::ui::talentpanel::CONNECTION_ORANGE;
    }

    while (true) {
      if (counter % 8 == 0) {
        float drawX = x0 + middlePoint.x() + 14;
        float drawY = y0 + middlePoint.y() + 14;
        DrawTextureProFast(*connectionImage, drawX, drawY, 0, WHITE);
      }
      if (x0 == x1 && y0 == y1) break;
      int e2 = 2 * err;
      if (e2 >= dy) {
        err += dy;
        x0 += sx;
      }
      if (e2 <= dx) {
        err += dx;
        y0 += sy;
      }
      counter++;
    }
  }
};
std::vector<TalentNode> TalentPanel::talents;
AdjacencyMatrix TalentPanel::matrix;
#endif  //MAGEQUEST_SRC_UI_PLAYER_TALENTPANEL_H_
