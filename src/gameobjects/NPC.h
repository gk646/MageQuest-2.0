#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_

struct NPC : public Entity {
  //TODO npc scripting
  MonsterResource* resource;
  std::string* dialogue = nullptr;
  std::vector<TexturedButton>* choices = nullptr;
  float speed;
  float dialogueProgressCount = 1000;
  int16_t dialogueShowDelayTicks = -1;
  bool flip = false;
  bool moving = false;
  NPC_ID id;
  bool last = false;
  NPC(const Point& pos, MonsterResource* resource, Zone zone, NPC_ID id = NPC_ID::RANDOM,
      float speed = 2, const Point& size = {30, 48})
      : Entity(pos, size, ShapeType::RECT, 0, zone),
        resource(resource),
        id(id),
        speed(speed) {}
  void Draw() override = 0;
  void Update() override {
    ENTITY_UPDATE();
    spriteCounter++;
    if (dialogueProgressCount < 1000) {
      dialogueProgressCount += 0.4F;
    } else {
      dialogueShowDelayTicks--;
    }
  }
  bool MoveToPointI(const PointI& next) noexcept {
    moving = false;
    PointI next_pos;
    if ((next_pos = PathFinding::AStarPathFinding(tile_pos, next)) > 0) {
      decideMovement(next_pos, speed);
      moving = true;
    } else if (next_pos == 0) {
      moving = false;
      return true;
    }
    return false;
  }
  void draw_dialogue() noexcept {
    if (dialogueShowDelayTicks > 0) {
      if (dialogue) {
        TextRenderer::RenderDialogue(pos.x_ + DRAW_X + size.x_ / 2, pos.y_ + DRAW_Y,
                                     dialogue, dialogueProgressCount, last);
      }
      if (choices) {
        float offSet = 0;
        for (auto& b : *choices) {
          b.Draw(pos.x_ + DRAW_X + size.x_ / 2, pos.y_ + DRAW_Y + size.x_ * 1.2F + offSet,
                 TextAlign::LEFT);
          offSet += 21;
        }
      }
    }
  }
  void update_dialogue(std::string* text) {
    dialogueProgressCount = 0;
    dialogue = text;
    dialogueShowDelayTicks = 400;
  }
  inline static NPC* GetNPCInstance(NPC_ID npcID, float absoluteX, float absoluteY,
                                    Zone npcZone) noexcept;
};

#include "../quest/QuestHandler.h"
#define INTERACT_WITH_PLAYER()                                                \
  if (Util::EPressed() && zone == CURRENT_ZONE && this->intersects(PLAYER)) { \
    if (!dialogue) {                                                          \
      PLAYER_QUESTS.InteractWithNPC(this);                                    \
      dialogueProgressCount = 0;                                              \
      dialogueShowDelayTicks = 400;                                           \
    } else if (dialogueShowDelayTicks < 0) {                                  \
      dialogueProgressCount = 0;                                              \
      dialogueShowDelayTicks = 400;                                           \
    } else if (dialogueProgressCount < 1000) {                                \
      dialogueProgressCount = 1000;                                           \
    } else {                                                                  \
      PLAYER_QUESTS.InteractWithNPC(this);                                    \
    }                                                                         \
  }
#define DRAW_NPC_DIALOGUE() \
  for (auto npc : NPCS) {   \
    npc->draw_dialogue();   \
  }                         \
  TextRenderer::RenderPlayerThought();

void Monster::MonsterDiedCallback() noexcept {
  ItemDropHandler::RollForItemDrop(pos.x_ + size.x_ / 2, pos.y_ + size.y_ / 2,
                                   stats.level);
  PLAYER_QUESTS.MonsterKilled(type);
  GAME_STATISTICS.MonsterKilled(type);
}
#include "npcs/Deckard.h"
#include "npcs/Aria.h"
#include "npcs/Marla.h"
NPC* NPC::GetNPCInstance(NPC_ID npcID, float absoluteX, float absoluteY,
                         Zone npcZone) noexcept {
  switch (npcID) {
    case NPC_ID::DECKARD:
      return new Deckard(absoluteX, absoluteY, npcZone);
    case NPC_ID::MARLA:
      return new Marla(absoluteX, absoluteY, npcZone);
    case NPC_ID::ARIA:
      return new Aria(absoluteX, absoluteY, npcZone);
    case NPC_ID::HILLCREST_MAYOR:
      break;
    case NPC_ID::RECEPTIONS:
      break;
    case NPC_ID::DYING_MAN:
      break;
    case NPC_ID::NIETZSCHE:
      break;
    case NPC_ID::VILLAGER:
      break;
    case NPC_ID::TRADER:
      break;
    case NPC_ID::RANDOM:
      break;
    case NPC_ID::NPC_END:
      break;
  }
}
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
