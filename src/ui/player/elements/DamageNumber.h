#ifndef MAGEQUEST_SRC_UI_PLAYER_DAMAGENUMBER_H_
#define MAGEQUEST_SRC_UI_PLAYER_DAMAGENUMBER_H_
struct DamageNumber {
  inline static char buffer[10];
  Vector2 pos;
  float val;
  DamageType dType;
  uint8_t visibleTicks = 0;
  bool crit;
  DamageNumber(float val, DamageType dType, const Point& pos, bool crit = false)
      : val(val), dType(dType), pos(pos.x(), pos.y()) , crit(crit){}
  inline void Draw() const noexcept {
    if (val == FLT_MIN) {
      snprintf(buffer, 10, "Dodged");
    } else {
      snprintf(buffer, 10, "%d", (int)val);
    }
    DrawTextExR(EDIT_UNDO, buffer, {pos.x +DRAW_X,pos.y+DRAW_Y}, crit ? 25 : 15, 0.5F, damageTypeToColor[dType]);
  }
  inline bool Update() noexcept {
    pos.y -= 0.25;
    visibleTicks++;
    return visibleTicks == 200;
  }
};
#endif  //MAGEQUEST_SRC_UI_PLAYER_DAMAGENUMBER_H_
