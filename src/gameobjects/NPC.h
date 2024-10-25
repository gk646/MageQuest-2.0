#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_

struct NPC : public Entity {
  //TODO npc scripting
  std::string name;
  MonsterResource* resource;
  std::string* dialogue = nullptr;
  std::vector<TexturedButton>* choices = nullptr;
  float speed;
  float dialogueProgressCount = 1000;
  int16_t dialogueShowDelayTicks = -1;
  bool isFlipped = false;
  bool moving = false;
  NPC_ID id;
  bool hideContinueButton = false;
  NPC(const Point& pos, MonsterResource* resource, Zone zone, NPC_ID id = NPC_ID::RANDOM,
      float speed = 2, const PointT<int16_t>& size = {25, 45})
      : Entity(pos, size, ShapeType::RECT, 0, false, zone),
        resource(resource),
        id(id),
        speed(speed),
        name(npcIdToStringMap[id]) {}
  void Draw() override {
    Util::DrawCenteredText(VARNISHED, 15, name.c_str(), pos.x_ + DRAW_X + size.x / 2,
                           pos.y_ + DRAW_Y - 20, Colors::LightGrey);
  };
  void Update() override {
    ENTITY_UPDATE()
    spriteCounter++;
    if (dialogueProgressCount < 1000) {
      dialogueProgressCount += 0.4F;
    } else {
      dialogueProgressCount = 1000.0F;
      dialogueShowDelayTicks -= PLAYER.tilePos.dist(tilePos) > 2;
    }
  }
  bool MoveToPointI(const PointI& next) noexcept {
    moving = false;
    PointT<int16_t> next_pos;
    if ((next_pos = PathFinding::AStarPathFinding(tilePos, next)) > 0) {
      isFlipped = next_pos.x < tilePos.x;
      CalculateMovement(next_pos, speed);
      moving = true;
    } else if (next_pos == 0) {
      moving = false;
      isFlipped = false;
      return true;
    }
    return false;
  }
  void DrawDialogue() noexcept {
    if (dialogueShowDelayTicks > 0) {
      if (dialogue) {
        TextRenderer::RenderDialogue(pos.x_ + DRAW_X + size.x / 2, pos.y_ + DRAW_Y,
                                     dialogue, dialogueProgressCount, hideContinueButton);
      }
      if (choices) {
        float offSet = 0;
        for (auto& b : *choices) {
          b.Draw(pos.x_ + DRAW_X + size.x / 2, pos.y_ + DRAW_Y + size.y * 1.1F + offSet,
                 Alignment::MIDDLE, Alignment::MIDDLE, MINECRAFT_BOLD, Colors::LightGrey);
          offSet += 20;
        }
      }
    }
  }
  void UpdateDialogue(std::string* text) {
    dialogueProgressCount = 0;
    dialogue = text;
    dialogueShowDelayTicks = std::max((int)text->size() * 2, 300);
  }
  inline static NPC* GetNewNPC(NPC_ID npcID, float absoluteX, float absoluteY,
                               Zone npcZone) noexcept;
};

#include "../quest/QuestHandler.h"
#include "npcs/Trader.h"

#define INTERACT_WITH_PLAYER()                                                       \
  if (Util::EPressed() && currentZone == CURRENT_ZONE && this->Intersects(PLAYER)) { \
    if (!dialogue) {                                                                 \
      PLAYER_QUESTS.InteractWithNPC(this);                                           \
      dialogueProgressCount = 0;                                                     \
      dialogueShowDelayTicks = 300;                                                  \
    } else if (dialogueShowDelayTicks < 0) {                                         \
      dialogueProgressCount = 0;                                                     \
      dialogueShowDelayTicks = 300;                                                  \
    } else if (dialogueProgressCount < 1000) {                                       \
      dialogueProgressCount = 1000;                                                  \
    } else {                                                                         \
      PLAYER_QUESTS.InteractWithNPC(this);                                           \
    }                                                                                \
  }
#define DRAW_NPC_DIALOGUE() \
  for (auto npc : NPCS) {   \
    npc->DrawDialogue();    \
  }                         \
  TextRenderer::RenderPlayerThought();

void Monster::MonsterDiedCallback() noexcept {
  if (IsBoss(type)) {
    PLAYER_SECOND_STATS.AddPlayerExperience(stats.level * 5);
    if (RANGE_100(RNG_ENGINE) < 15) {
      //TODO drop coins
    }
  } else {
    ItemDropHandler::RollForItemDrop(pos.x_ + size.x / 2, pos.y_ + size.y / 2,
                                     stats.level);
    PLAYER_SECOND_STATS.AddPlayerExperience(stats.level);
  }
  PLAYER_QUESTS.MonsterKilled(type);
  GAME_STATISTICS.MonsterKilled(type);
  ApplyTalentsToMonsterDeath(this);
}

#include "npcs/NPCS.h"

NPC* NPC::GetNewNPC(NPC_ID npcID, float absoluteX, float absoluteY,
                    Zone npcZone) noexcept {
  switch (npcID) {
    case NPC_ID::DECKARD:
      return new Deckard(absoluteX, absoluteY, npcZone);
    case NPC_ID::MARLA:
      return new Marla(absoluteX, absoluteY, npcZone);
    case NPC_ID::ARIA:
      return new Aria(absoluteX, absoluteY, npcZone);
    case NPC_ID::SATIRO:
      return new Satiro(absoluteX, absoluteY, npcZone);
    case NPC_ID::NIETZSCHE:
      return new Nietzsche(absoluteX, absoluteY, npcZone);
    case NPC_ID::GROVE_RECEPTION:
      return new ReceptionGrove(absoluteX, absoluteY, npcZone);
    case NPC_ID::HILLCREST_MAYOR:
    case NPC_ID::DYING_MAN:
    case NPC_ID::VILLAGER:
    case NPC_ID::TRADER:
    case NPC_ID::RANDOM:
    case NPC_ID::NPC_END:
      break;
  }
  std::cout << "MISSING NPC ENUM VALUE AT:" << (int)npcID << std::endl;
}
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
