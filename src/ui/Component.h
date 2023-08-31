#ifndef DND_SRC_UI_COMPONENT_H_
#define DND_SRC_UI_COMPONENT_H_


struct Component {
  virtual ~Component() = default;
  virtual void draw(float ui_scale, int x, int y) = 0;
  virtual void update(){};
};

#endif  //DND_SRC_UI_COMPONENT_H_
