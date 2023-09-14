#ifndef MAGEQUEST_SRC_GAMEOBJECTS_NPCS_DECKARD_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_NPCS_DECKARD_H_

struct Deckard final : public NPC {
  bool flip = false;
  bool moving = false;
  Deckard(int x, int y) : NPC({x, y},&textures::OLD_MAN, NPC_ID::DECKARD) {

  }
  void draw() final{
    if(moving){
      DrawTextureProFastEx(resource->walk[sprite_counter % 60 / 10], pos.x_ + DRAW_X ,
                           pos.y_ + DRAW_Y , -23, 0, flip, WHITE);

    }else{
      DrawTextureProFastEx(resource->idle[sprite_counter % 60 / 15], pos.x_ + DRAW_X ,
                           pos.y_ + DRAW_Y, -23, 0, flip, WHITE);
    }
#ifdef DRAW_HITBOXES
    draw_hitbox();
#endif
  }
  void update() final{
    NPC::update();

  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_NPCS_DECKARD_H_
