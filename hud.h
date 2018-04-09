#ifndef HUD_H
#define HUD_H

#include <string>
#include "sprite.h"

class Hud {
public:
  Hud(const std::string&);
  Hud(const Hud&);
  const Sprite* getHudSprite() const { return &hudSprite; }
  Hud& operator=(const Hud&) = delete;
  void draw();
  void stop();
private:
  int startLocX;
  int startLocY;
  Sprite hudSprite;
};

#endif
