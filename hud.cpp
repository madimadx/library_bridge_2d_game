#include "hud.h"
#include "renderContext.h"
#include "ioMod.h"
#include "gamedata.h"
//#include <sstream>

Hud::Hud(const std::string& name) :
  startLocX(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")),
  startLocY(Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  currOffX(0),
  currOffY(0),
  hudSprite(name, Vector2f(startLocX, startLocY), Vector2f(0, 0))
{ }

void Hud::stop() {
  hudSprite.setVelocity( Vector2f(0, 0) );
}

void Hud::draw() {
  hudSprite.draw();
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strU"), startLocX+90, startLocY+70);
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strD"), startLocX+90, startLocY+100);
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strL"), startLocX+60, startLocY+100);
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strR"), startLocX+120, startLocY+100);

  //IoMod::getInstance().writeText("Collision "+Gamedata::getInstance().getXmlStr("Keys/strCol"), startLocX+50, startLocY+120);
  //IoMod::getInstance().writeText("HUD "+Gamedata::getInstance().getXmlStr("Keys/strHUD"), startLocX+50, startLocY+150);
  //IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strPause")+" Pause", startLocX+50, startLocY+120);
}

void Hud::update(int offX, int offY) {
  int diffX = currOffX - offX;
  int diffY = currOffY - offY;
  currOffX = offX;
  currOffY = offY;
  Vector2f incr = Vector2f(-diffX, -diffY);
  hudSprite.setPosition(hudSprite.getPosition()+incr);
}
