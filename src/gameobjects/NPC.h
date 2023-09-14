#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_

struct NPC : public Entity {
  NPC_ID id ;
  int show_dial_delay = 300;
  MonsterResource* resource;
  NPC(const Point& pos, MonsterResource* resource, NPC_ID id = NPC_ID::RANDOM,const Point& size = {30, 48})
      : Entity(pos, size, ShapeType::RECT), resource(resource),id(id) {}
  void draw() override = 0;
  void update() override{
    sprite_counter++;
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
  }
  inline void update_npc() noexcept {
    sprite_counter++;
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
  }

  void speak() const noexcept { auto& text = QuestResources::get_quest_dialogue(id); }
  void speak_dynamic(int dist) const noexcept {
    if (PLAYER.tile_pos.dist(tile_pos) < dist) {}
  }
};

#include "npcs/Deckard.h"
#include "npcs/Aria.h"

#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
