#include <cmath>
#include <random>
#include <functional>
#include "hud.h"
#include "gamedata.h"
#include "renderContext.h"

Vector2f Hud::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);;
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);;
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

Hud::Hud(const string& n, const Vector2f& pos, const Vector2f& vel,
               const Image* img):
  Drawable(n, pos, vel),
  image( img ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Hud::Hud(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(
                    0,
                    0)
           ),
  image( RenderContext::getInstance()->getImage(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Hud::Hud(const Hud& s) :
  Drawable(s),
  image(s.image),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

Hud& Hud::operator=(const Hud& rhs) {
  Drawable::operator=( rhs );
  image = rhs.image;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Hud::draw() const {
  if(getScale() < SCALE_EPSILON) return;
  image->draw(getX(), getY(), getScale());
}

void Hud::update(Uint32 ticks) {
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
}
