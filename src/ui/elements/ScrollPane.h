#ifndef MAGEQUEST_SRC_UI_ELEMENTS_SCROLLPANE_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_SCROLLPANE_H_

struct ScrollPane {
  inline static constexpr float SCROLL_BUTTON_WIDTH = 20;
  inline static constexpr float SCROLL_BUTTON_HEIGHT = 30;
  RectangleR bounds;
  RectangleR scrollBounds;
  Content* content;
  Vector2 previousPos{};
  bool isScrollShown = false;
  bool isDragged = false;
  ScrollPane(RectangleR bounds, Content* content)
      : content(content),
        bounds(bounds),
        scrollBounds(bounds.x + bounds.width, 0, SCROLL_BUTTON_WIDTH,
                     SCROLL_BUTTON_HEIGHT) {}
  //Draws the scroll pane and its content aligned width the given coordinates
  inline void Draw(float x, float y) noexcept {
    UpdateImpl(x, y);
    if (content) {
      content->Draw({bounds.x, bounds.y, isScrollShown ? bounds.width - 20 : bounds.width,
                     bounds.height});
    }
    DrawScrollPanel();
    DrawBounds();
  }
  //Updates the pane and its content
  inline void Update() noexcept {
    isScrollShown = content->GetHeight() > bounds.height;
    if (CheckCollisionPointRec(MOUSE_POS, {scrollBounds.x, scrollBounds.y + bounds.y,
                                           scrollBounds.width, scrollBounds.height}) &&
        IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      isDragged = true;
    }
    if (content) content->Update();

    if (!isScrollShown) {
      scrollBounds.y = 0;
    }
  }

 private:
  inline void DrawBounds() noexcept {
    DrawLineEx({bounds.x + bounds.width, bounds.y},
               {bounds.x + bounds.width, bounds.y + bounds.height}, 2,
               Colors::darkBackground);
  }
  inline void DrawScrollPanel() noexcept {
    if (content && content->GetHeight() > bounds.height) {
      DrawRectangleProFast(scrollBounds.x, scrollBounds.y + bounds.y, scrollBounds.width,
                           scrollBounds.height, Colors::mediumLightGreyDarker);
    }
  }
  inline void UpdateImpl(float x, float y) noexcept {
    bounds.x = x;
    bounds.y = y;

    scrollBounds.x = x + bounds.width - SCROLL_BUTTON_WIDTH;

    if (isScrollShown && isDragged && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      scrollBounds.y += MOUSE_POS.y - previousPos.y;
    } else {
      isDragged = false;
    }
    previousPos = GetMousePosition();
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_SCROLLPANE_H_
