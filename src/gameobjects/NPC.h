#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_

struct NPC : public Entity {
  NPC_ID id;
  int show_dial_delay = -1;
  float dial_count = 1000;
  std::string* dialogue = nullptr;
  bool flip = false;
  bool moving = false;
  MonsterResource* resource;
  NPC(const Point& pos, MonsterResource* resource, NPC_ID id = NPC_ID::RANDOM,
      const Point& size = {30, 48})
      : Entity(pos, size, ShapeType::RECT), resource(resource), id(id) {}
  void draw() override = 0;
  void draw_dialogue() {
    if (show_dial_delay > 0) {
      DialogueRender::render_npc(pos.x_ + DRAW_X + size.x_ / 2,
                                 pos.y_ + DRAW_Y, dialogue, dial_count);
    }
  }
  void update() override {
    sprite_counter++;
    if (dial_count < 1000) {
      dial_count+=0.4F;
    } else {
      show_dial_delay--;
    }
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
  }
  void update_dialogue(std::string* text) {
    dial_count = 0;
    dialogue = text;
    show_dial_delay = 300;
  }
};

#include "../quest/QuestHandler.h"
#include "npcs/Deckard.h"
#include "npcs/Aria.h"

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
