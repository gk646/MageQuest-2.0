#ifndef MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_

struct Button {
  bool Draw(RectangleR bounds, const char* toolTip = "") noexcept {
    if (CheckCollisionPointRec(MOUSE_POS, bounds)) {
      ToolTip::DrawToolTip(toolTip, ANT_PARTY, 15);
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      } else {

      }
    } else {
    }

    return false;
  }

};

#endif  //MAGEQUEST_SRC_UI_ELEMENTS_BUTTON_H_
