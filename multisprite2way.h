#ifndef MULTISPRITE2WAY__H
#define MULTISPRITE2WAY__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class MultiSprite2way : public Drawable {
public:
  MultiSprite2way(const std::string&);
  MultiSprite2way(const MultiSprite2way&);
  MultiSprite2way(const std::string&, float, float);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
    return images[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks, int);
  MultiSprite2way& operator=(const MultiSprite2way&);
};
#endif
