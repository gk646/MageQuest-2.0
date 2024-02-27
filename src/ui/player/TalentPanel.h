#ifndef MAGEQUEST_SRC_UI_PLAYER_TALENTPANEL_H_
#define MAGEQUEST_SRC_UI_PLAYER_TALENTPANEL_H_

#include "elements/TalentNode.h"
#include "elements/AdjacencyMatrix.h"

struct TalentPanel final : public Window {
  AdjacencyMatrix matrix;
  Point middlePoint{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
  int16_t toolTipID = -1;
  TalentPanel()
      : Window(0, 0, (int)1920, (int)1080, 0, nullptr, KEY_N, sound::EMPTY_SOUND,
               sound::EMPTY_SOUND) {}
  void OnOpen() final { middlePoint = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}; }
  void Draw() noexcept {
    WINDOW_LOGIC()
    DrawBackGround();
    DrawConnections();
    DrawTalentNodes(middlePoint);
    DrawToolTipNode();
  }
  void Update() noexcept {
    WINDOW_DRAW_UPDATE();
    for (auto& node : TALENTS) {
      node.Update();
    }
  }
  //Applies all effects from TALENTS
  inline static void ApplyEffects() noexcept {
    auto& stats = PLAYER_STATS;
    for (const auto& t : TALENTS) {
      if (t.isActivated) {
        for (int i = 0; i < STATS_ENDING; i++) {
          if (i < 9) {
            stats.effects[i] +=
                t.talent.effects[i] * (float)std::sqrt(PLAYER_STATS.level);
            continue;
          }
          stats.effects[i] += t.talent.effects[i];
        }
      }
    }
  }

 private:
  //If a nodes is hovered draws its tooltip // should be on top of other TALENTS
  inline void DrawToolTipNode() noexcept {
    if (toolTipID == -1) {
      DragLogic();
    } else {
      TALENTS[toolTipID].DrawToolTip(
          Util::GetToolTipRect(TalentNode::TOOLTIP_WIDTH, TalentNode::TOOLTIP_HEIGHT));
    }
    toolTipID = -1;
  }
  //Draws the window background tiles
  static inline void DrawBackGround() noexcept {
    auto id = textures::ui::talentpanel::TALENT_BACKGROUND_TILE.id;
    for (int i = 0; i < 42; i++) {
      for (int j = 0; j < 24; j++) {
        DrawTextureProFastUltra(id, i * TILE_SIZE, j * TILE_SIZE);
      }
    }
  }
  //Draw all connections between the nodes
  inline void DrawConnections() const noexcept {
    for (const auto& pair : matrix.connectionsTo) {
      const auto& startNode = TALENTS[pair.first];
      for (const auto& nodeID : pair.second) {
        if (nodeID == -1) break;
        DrawTalentConnection(middlePoint, startNode, TALENTS[nodeID]);
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
  //Checks if any nodes at incoming or outgoing connections are activated -> no tree but graph -> every node unlocks all its surrounding ones
  inline bool IsUsableTalent(const int16_t talentID) noexcept {
    if (talentID < 4) return true;

    auto fromIt = matrix.connectionsFrom.find(talentID);
    if (fromIt != matrix.connectionsFrom.end()) {
      for (int16_t id : fromIt->second) {
        if (id == -1) break;
        if (TALENTS[id].isActivated) {
          return true;
        }
      }
    }

    auto toIt = matrix.connectionsTo.find(talentID);
    if (toIt != matrix.connectionsTo.end()) {
      for (int16_t id : toIt->second) {
        if (id == -1) break;
        if (TALENTS[id].isActivated) {
          return true;
        }
      }
    }

    return false;
  }

 private:
  //Draws all talent-nodes with their correct texture, state and size and captures the hovered nodes ID
  inline void DrawTalentNodes(const Point& mid) noexcept {
    for (auto& node : TALENTS) {
      if (node.sizeType == TalentSize::NORMAL) {
        if (node.isActivated) {
          node.Draw(textures::ui::talentpanel::NODE_GREEN, mid.x(), mid.y(), toolTipID);
        } else if (IsUsableTalent(node.talentID)) {
          node.Draw(textures::ui::talentpanel::NODE_PURPLE, mid.x(), mid.y(), toolTipID);
        } else {
          node.Draw(textures::ui::talentpanel::NODE_ORANGE, mid.x(), mid.y(), toolTipID);
        }
      } else if (node.sizeType == TalentSize::BIG) {
        if (node.isActivated) {
          node.Draw(textures::ui::talentpanel::TALENT_BIG_GREEN, mid.x() - 9, mid.y() - 9,
                    toolTipID);
        } else if (IsUsableTalent(node.talentID)) {
          node.Draw(textures::ui::talentpanel::TALENT_BIG_PURPLE, mid.x() - 9,
                    mid.y() - 9, toolTipID);
        } else {
          node.Draw(textures::ui::talentpanel::TALENT_BIG, mid.x() - 9, mid.y() - 9,
                    toolTipID);
        }
      } else if (node.sizeType == TalentSize::MID) {
        if (node.isActivated) {
          node.Draw(textures::ui::talentpanel::TALENT_MID_GREEN, mid.x() - 7, mid.y() - 7,
                    toolTipID);
        } else if (IsUsableTalent(node.talentID)) {
          node.Draw(textures::ui::talentpanel::TALENT_MID_PURPLE, mid.x() - 7,
                    mid.y() - 7, toolTipID);
        } else {
          node.Draw(textures::ui::talentpanel::TALENT_MID, mid.x() - 7, mid.y() - 7,
                    toolTipID);
        }
      }
    }
  }
  //Draws a correctly colored line of images from on talent to the other
  static inline void DrawTalentConnection(const Point& middlePoint,
                                          const TalentNode& requirement,
                                          const TalentNode& nextOne) noexcept {
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
    int e2 = 2 * err;
    const Texture* connectionImage = &textures::ui::talentpanel::CONNECTION_ORANGE;

    if (!nextOne.isActivated) {
      connectionImage = &textures::ui::talentpanel::CONNECTION_RED;
    } else if (requirement.isActivated && nextOne.isActivated) {
      connectionImage = &textures::ui::talentpanel::CONNECTION_GREEN;
    }

    float drawX, drawY;
    while (true) {
      if (counter == 8) {
        drawX = x0 + middlePoint.x() + 14;
        drawY = y0 + middlePoint.y() + 14;
        DrawTextureProFast(*connectionImage, drawX, drawY, 0, WHITE);
        counter = 0;
      }
      if (x0 == x1 && y0 == y1) break;
      e2 = 2 * err;
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

void EntityStats::ReCalculatePlayerStats() noexcept {
  //Sources: Talents | Effects | Items | UniqueEffects
  auto& stats = PLAYER_STATS.effects;

  //Reset stats
  for (const auto unique : UNIQUE_EFFECTS) {
    unique->OnRemove();  //Called for safety and completion
  }
  std::memset(PLAYER_STATS.effects, 0, sizeof(float) * STATS_ENDING);

  PLAYER_STATS.SetPlayerDefaults();

  //Activate Talents
  TalentPanel::ApplyEffects();

  //Equip Items
  for (int i = 0; i < 10; i++) {
    auto item = PLAYER_EQUIPPED[i].item;
    if (item) {
      for (int j = 0; j < STATS_ENDING; j++) {
        stats[j] += item->effects[j];
      }
    }
  }

  //Activate Unique Effects
  for (const auto unique : UNIQUE_EFFECTS) {
    unique->OnAdd();
  }

  PLAYER_EFFECTS.ApplyEffects();

  effects[MAX_HEALTH] += effects[VITALITY] * 5.0F;
  effects[MAX_MANA] += effects[INTELLIGENCE] * 7.5F;

  effects[HEALTH_REGEN] += effects[ENDURANCE] / 18.0F;
  effects[MANA_REGEN] += effects[WISDOM] / 9.0F;

  effects[SPEED_MULT_P] += (effects[AGILITY] / (float)std::sqrt(level)) / 44.0F;
  effects[DODGE_CHANCE] += (effects[AGILITY] / (float)std::sqrt(level)) / 1.8F;
  effects[CRIT_CHANCE] += (effects[LUCK] / (float)std::sqrt(level)) * 1.3F;
}
#endif  //MAGEQUEST_SRC_UI_PLAYER_TALENTPANEL_H_
