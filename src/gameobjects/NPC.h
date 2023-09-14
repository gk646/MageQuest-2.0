#ifndef MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_

struct NPC : public Entity {
  NPC_ID id = NPC_ID::RANDOM;
  int show_dial_delay = 300;
  NPC(const Point& pos, const Point& size = {30, 48})
      : Entity(pos, size, ShapeType::RECT) {}
  void draw() override = 0;
  inline void update_npc() noexcept {
    sprite_counter++;
    tile_pos.x = (pos.x_ + size.x_ / 2) / TILE_SIZE;
    tile_pos.y = (pos.y_ + size.y_ / 2) / TILE_SIZE;
  }

  void speak() const noexcept {

  auto&  text =  QuestResources::get_dialogue(id);

  }
  void speak_dynamic(int dist) const noexcept {
    if (PLAYER.tile_pos.dist(tile_pos) < dist) {}
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_ENTITIES_TYPES_NPC_H_
