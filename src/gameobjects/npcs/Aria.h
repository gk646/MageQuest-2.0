#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_

struct Aria final : public NPC {

  Aria(int x, int y) : NPC({x, y}, &textures::ARIA) {}
  void draw() final {}
  void update() final { NPC::update(); }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
