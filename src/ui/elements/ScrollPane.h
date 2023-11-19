#ifndef MAGEQUEST_SRC_UI_ELEMENTS_SCROLLPANE_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_SCROLLPANE_H_

struct ScrollPane {
  inline static constexpr float SCROLL_BUTTON_WIDTH = 20;
  RectangleR bounds;
  RectangleR scrollBounds;
  Content* content;
  float lastContentHeight = 0;
  double logicalScrollY = 5.0F;
  bool isScrollShown = false;
  bool isDragged = false;
  ScrollPane(RectangleR bounds, Content* content)
      : content(content),
        bounds(bounds),
        scrollBounds(bounds.x + bounds.width, 0, SCROLL_BUTTON_WIDTH, 30) {}
  //Draws the scroll pane and its content aligned width the given coordinates
  inline void Draw(float x, float y) noexcept {
    UpdateImpl(x, y);
    if (content) {
      content->Draw({bounds.x, bounds.y, isScrollShown ? bounds.width - 20 : bounds.width,
                     bounds.height},
                    logicalScrollY);
    }
    DrawScrollPanel();
    DrawBounds();
  }
  //Updates the pane and its content
  inline void Update() noexcept {
    if (content) {
      content->Update();
      lastContentHeight = content->GetHeight();
    }

    isScrollShown = lastContentHeight > bounds.height;
    if (isScrollShown) {
      UpdateScrollSlider();
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(MOUSE_POS, {bounds.x + bounds.width - SCROLL_BUTTON_WIDTH,
                                           scrollBounds.y + bounds.y, scrollBounds.width,
                                           scrollBounds.height})) {
      isDragged = true;
    }
    if (isDragged) {
      if (!CheckCollisionPointRec(MOUSE_POS, {bounds})) {
        isDragged = false;
      }
    }

    if (!isScrollShown) {
      scrollBounds.y = 5;
      logicalScrollY = 0;
    }
    if (!WINDOW_FOCUSED) {
      WINDOW_FOCUSED = isDragged;
    }
  }

 private:
  //Called at the start of each draw tick
  inline void UpdateImpl(float x, float y) noexcept {
    bounds.x = x;
    bounds.y = y;

    isScrollShown = lastContentHeight > bounds.height;

    if (isScrollShown && isDragged && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      ChangeScrollValue(GetMouseDelta().y);
    } else {
      isDragged = false;
    }

    if (!isDragged && isScrollShown) {
      ChangeScrollValue(GetMouseWheelMove() * -5.0F);
    }
  }

 private:
  inline void DrawBounds() const noexcept {
    //DrawRectangleRoundedLines(bounds, 0.1F, 30, 1, Colors::rareColor);
    DrawLineEx({bounds.x + bounds.width, bounds.y},
               {bounds.x + bounds.width, bounds.y + bounds.height}, 2,
               Colors::darkBackground);
  }
  inline void DrawScrollPanel() const noexcept {
    if (isScrollShown) {
      DrawRectangleProFast(bounds.x + bounds.width - SCROLL_BUTTON_WIDTH,
                           scrollBounds.y + bounds.y, scrollBounds.width,
                           scrollBounds.height, Colors::mediumLightGreyDarker);
    }
  }

 private:
  //Updates the visuals of the scroll slider
  inline void UpdateScrollSlider() noexcept {
    double ratio = bounds.height / lastContentHeight;
    scrollBounds.height = (float)std::max(30.0, bounds.height * ratio);
    scrollBounds.y = (logicalScrollY / lastContentHeight) * bounds.height;
  }
  inline void ChangeScrollValue(float val) noexcept {
    double scale = bounds.height / lastContentHeight;

    scrollBounds.y = std::min(std::max(scrollBounds.y + val, 5.0F),
                              bounds.height - scrollBounds.height);

    logicalScrollY = std::min(std::max(logicalScrollY + val / scale, 0.0),
                              (double)lastContentHeight - bounds.height);

    if (scrollBounds.y == bounds.height - scrollBounds.height) {}
  }
};

#endif  //MAGEQUEST_SRC_UI_ELEMENTS_SCROLLPANE_H_
