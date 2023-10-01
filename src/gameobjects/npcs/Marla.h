#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_MARLA_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_MARLA_H_
struct Marla final : public NPC {

  Marla(float x, float y, Zone zone) :  NPC({x, y}, &textures::npc::MARLA, zone, NPC_ID::MARLA, 2) {}
  void draw() final {}
  void update() final { NPC::update(); }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_MARLA_H_
