#include "hud.h"
#include "renderContext.h"
#include "ioMod.h"
#include "gamedata.h"

Hud::Hud(const std::string& name) :
  startLocX(Gamedata::getInstance().getXmlInt(name+"/startLoc/x")),
  startLocY(Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  hudSprite(name, Vector2f(startLocX, startLocY), Vector2f(0, 0))
{ }

void Hud::stop() {
  hudSprite.setVelocity( Vector2f(0, 0) );
}

void Hud::draw() {
  hudSprite.draw();
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strU"), startLocX+80, startLocY+60);
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strD"), startLocX+80, startLocY+90);
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strL"), startLocX+50, startLocY+90);
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strR"), startLocX+110, startLocY+90);

  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strCol"), startLocX+50, startLocY+120);
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strHUD"), startLocX+80, startLocY+120);
  IoMod::getInstance().writeText(Gamedata::getInstance().getXmlStr("Keys/strPause"), startLocX+110, startLocY+120);
}
