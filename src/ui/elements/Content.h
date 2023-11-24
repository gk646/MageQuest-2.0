#ifndef MAGEQUEST_SRC_UI_ELEMENTS_CONTENT_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_CONTENT_H_
//Defines something as possible content of another class (e.g. ScrollPane)
struct Content {
  float lowerLimit = 0, upperLimit = 0;
  //Passes the bounds of the container with the request respect them
  inline virtual void Draw(RectangleR bounds, float scrollOffset) noexcept = 0;
  //Called on the update thread
  inline virtual void Update() noexcept = 0;
  inline virtual float GetWidth() const noexcept = 0;
  inline virtual float GetHeight() const noexcept = 0;
  //Method to update limits
  inline void UpdateLimits(RectangleR& cBounds, float offSet) noexcept {
    lowerLimit = cBounds.y;
    upperLimit = cBounds.y + cBounds.height;
    cBounds.y -= offSet;
    cBounds.y += 5;
  }
  //Returns true if within the content bounds
  [[nodiscard]] inline bool IsInBounds(float y, float height) const noexcept {
    return y >= lowerLimit && y + height <= upperLimit;
  }
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_CONTENT_H_
