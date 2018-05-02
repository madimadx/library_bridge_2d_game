#include <sstream>
#include "viewport.h"
#include "ioMod.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() :
  gdata(Gamedata::getInstance()),
  position(0, gdata.getXmlInt("world/height")-gdata.getXmlInt("view/height")),
  worldWidth(gdata.getXmlInt("world/width")),
  worldHeight(gdata.getXmlInt("world/height")),
  viewWidth(gdata.getXmlInt("view/width")),
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0),
  objectToTrack(NULL)
{
  altColor.r = Gamedata::getInstance().getXmlInt("altColor/r");
  altColor.g = Gamedata::getInstance().getXmlInt("altColor/g");
  altColor.b = Gamedata::getInstance().getXmlInt("altColor/b");
  altColor.a = Gamedata::getInstance().getXmlInt("altColor/a");
}

void Viewport::setObjectToTrack(const Drawable *obj) {
  objectToTrack = obj;
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw() const {
  IoMod::getInstance().
      writeText(gdata.getXmlStr("me"), 30, viewHeight-30-gdata.getXmlInt("font/size"), altColor);
}

void Viewport::drawFPS(const int FPS) {
  std::stringstream ss;
  ss << "FPS: " << FPS;
  IoMod::getInstance().
      writeText(ss.str(), 30, viewHeight-30-2*gdata.getXmlInt("font/size"), altColor);
  ss.clear();
}

void Viewport::update() {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();

  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
}
