#include "player.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "renderContext.h"


Player::Player( const std::string& name) :
  player(name),
  initialVelocity(player.getVelocity()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  elapsedFromDeath(0),
  moveSizeHorz(Gamedata::getInstance().getXmlInt(name+"/horzJump")),
  moveSizeVert(Gamedata::getInstance().getXmlInt(name+"/vertJump")),
  deathDelay(Gamedata::getInstance().getXmlInt(name+"/deathDelay")),
  death(false),
  observers()
{
  stop();
  lastPos = player.getPosition();
}

void Player::stop() {
  player.setVelocity( Vector2f(0, 0) );
}

void Player::right() {
  lastPos = player.getPosition();
  if ( player.getX() < worldWidth-getScaledWidth() - (moveSizeHorz)) {
    player.setPositionX(getX() + moveSizeHorz);
  }
}
void Player::left()  {
  if ( player.getX() > moveSizeHorz) {
    lastPos = player.getPosition();
    player.setPositionX(getX() - moveSizeHorz);
  }
}
void Player::up()    {
  if ( player.getY() > moveSizeVert) {
    lastPos = player.getPosition();
    player.setPositionY(getY() - moveSizeVert);
  }
}
void Player::down()  {
  if ( player.getY() < worldHeight-getScaledHeight() - moveSizeVert) {
    lastPos = player.getPosition();
    player.setPositionY(getY() + moveSizeVert);
  }
}

/*
void Player::right() {
  if ( player.getX() < worldWidth-getScaledWidth()) {
    player.setVelocityX(initialVelocity[0]);
  }
}
void Player::left()  {
  if ( player.getX() > 0) {
    player.setVelocityX(-initialVelocity[0]);
  }
}
void Player::up()    {
  if ( player.getY() > 0) {
    player.setVelocityY( -initialVelocity[1] );
  }
}
void Player::down()  {
  if ( player.getY() < worldHeight-getScaledHeight()) {
    player.setVelocityY( initialVelocity[1] );
  }
}
*/

void Player::deathDraw() const {
  if (elapsedFromDeath % deathDelay > deathDelay/2) {
    player.draw();
  }
}

bool Player::deathReset() {
  if (elapsedFromDeath >= deathDelay*4) {
    death = false;
    return true;
  }
  return false;
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::update(Uint32 ticks) {
  if (!isDeathOn()) {
    player.update(ticks);
    std::list<SmartSprite*>::iterator ptr = observers.begin();
    while ( ptr != observers.end() ) {
      (*ptr)->setPlayerPos( player.getPosition() );
      ++ptr;
    }
    stop();
  }
  else {
    elapsedFromDeath += ticks;
  }
}
