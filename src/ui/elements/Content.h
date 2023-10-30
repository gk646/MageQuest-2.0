#ifndef MAGEQUEST_SRC_UI_ELEMENTS_CONTENT_H_
#define MAGEQUEST_SRC_UI_ELEMENTS_CONTENT_H_
//Defines something as possible content of another class (e.g. ScrollPane)
struct Content {
  //Passes the bounds of the container with the request respect them
  inline virtual void Draw(RectangleR bounds) noexcept = 0;
  //Called on the update thread
  inline virtual void Update() noexcept = 0;
  inline virtual int GetWidth() const noexcept = 0;
  inline virtual int GetHeight() const noexcept = 0;
};
#endif  //MAGEQUEST_SRC_UI_ELEMENTS_CONTENT_H_
