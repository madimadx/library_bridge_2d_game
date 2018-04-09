#ifndef HUD_H
#define HUD_H

#include <string>
#include "drawable.h"

class Hud : public Drawable {
public:
  Hud(const std::string&);
  Hud(const std::string&, const Vector2f& pos, const Vector2f& vel,
         const Image*);
  Hud(const Hud&);
  virtual ~Hud() { }
  Hud& operator=(const Hud&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const {
    return image->getSurface();
  }
  int getScaledWidth()  const { return getScale()*image->getWidth();  }
  int getScaledHeight() const { return getScale()*image->getHeight(); }

private:
  const Image * image;

protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const Hud*) const;
  Vector2f makeVelocity(int, int) const;
};

#endif
