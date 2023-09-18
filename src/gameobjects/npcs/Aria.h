#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_

struct Aria final : public NPC {

  Aria(float x, float y) : NPC({x*48, y*48}, &textures::ARIA) {}
  void draw() final {}
  void update() final { NPC::update(); }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
