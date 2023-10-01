#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_

struct Aria final : public NPC {

  Aria(float x, float y, Zone zone) :  NPC({x, y}, &textures::npc::ARIA, zone, NPC_ID::ARIA, 2) {}
  void draw() final {}
  void update() final { NPC::update(); }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_ARIA_H_
