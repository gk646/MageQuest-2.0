#ifndef MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_DUMMY_H_
#define MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_DUMMY_H_
struct Dummy final : Projectile {
  Dummy(const Point& pos, bool from_player, float damage)
      : Projectile(from_player, pos, {25, 25}, ShapeType::RECT, 240, 4,
                   {DamageType::FIRE, damage}, HitType::ONE_HIT, {},
                   {0,0}, 0, nullptr, &textures::projectile::DUMMY, nullptr) {
    float angle =
        std::atan2(MOUSE_POS.y - (pos.y_ + DRAW_Y)-size.y_/2, MOUSE_POS.x - (pos.x_ + DRAW_X)-size.x_/2);
    pov = angle * (180.0f / PI);
    mvmVector = GetMovementVector();
  }

  void draw() final {
    DrawTextureProFastRot(resources->frames[0], pos.x_ + DRAW_X, pos.y_ + DRAW_Y, pov,
                          WHITE);
  }
};
#endif  //MAGEQUEST_SRC_GAMEOBJECTS_PROJECTILES_DUMMY_H_
