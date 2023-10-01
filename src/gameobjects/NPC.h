#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_

struct NPC : public Entity {
  //TODO npc scripting
  NPC_ID id;
  bool flip = false;
  bool moving = false;
  float speed;
  MonsterResource* resource;
  /* |-----------------------------------------------------|
   * |                         Dialog                      |
   * |-----------------------------------------------------|
   */
  int show_dial_delay = -1;
  bool last = false;
  float dial_count = 1000;
  std::string* dialogue = nullptr;
  NPC(const Point& pos, MonsterResource* resource, Zone zone, NPC_ID id = NPC_ID::RANDOM,
      float speed = 2, const Point& size = {30, 48})
      : Entity(pos, size, ShapeType::RECT, 0, zone),
        resource(resource),
        id(id),
        speed(speed) {}
  void draw() override = 0;
  void update() override {
    ENTITY_UPDATE();
    sprite_counter++;
    if (dial_count < 1000) {
      dial_count += 0.4F;
    } else {
      show_dial_delay--;
    }
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
  }
  bool move_to(const PointI& next) noexcept {
    moving = false;
    PointI next_pos;
    if ((next_pos = astar_pathfinding(tile_pos, next)) > 0) {
      decideMovement(next_pos, speed);
      moving = true;
    } else if (next_pos == 0) {
      moving = false;
      return true;
    }
    return false;
  }
  void draw_dialogue() noexcept {
    if (show_dial_delay > 0) {
      if (dialogue) {
        DialogueRender::render_npc(pos.x_ + DRAW_X + size.x_ / 2, pos.y_ + DRAW_Y,
                                   dialogue, dial_count, last);
      }
    }
  }
  void update_dialogue(std::string* text) {
    dial_count = 0;
    dialogue = text;
    show_dial_delay = 400;
  }
};

#include "../quest/QuestHandler.h"
#define INTERACT_WITH_PLAYER()                          \
  if (IsKeyPressedU::E() && this->intersects(PLAYER)) { \
    if (!dialogue) {                                    \
      PLAYER_QUESTS.InteractWithNPC(this);              \
      dial_count = 0;                                   \
      show_dial_delay = 400;                            \
    } else if (show_dial_delay < 0) {                   \
      dial_count = 0;                                   \
      show_dial_delay = 400;                            \
    } else if (dial_count < 1000) {                     \
      dial_count = 1000;                                \
    } else {                                            \
      PLAYER_QUESTS.InteractWithNPC(this);              \
    }                                                   \
  }
#define DRAW_NPC_DIALOGUE() \
  for (auto npc : NPCS) {   \
    npc->draw_dialogue();   \
  }
void Monster::MonsterDiedCallback() noexcept {
  ItemDropHandler::RollForItemDrop(pos.x_ + size.x_ / 2, pos.y_ + size.y_ / 2,
                                   stats.level);
  PLAYER_QUESTS.MonsterKilled(type);
  GAME_STATISTICS.MonsterKilled(type);
}
#include "npcs/Deckard.h"
#include "npcs/Aria.h"
#include "npcs/Marla.h"

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
