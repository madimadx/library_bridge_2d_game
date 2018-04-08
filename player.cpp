#include "player.h"
#include "smartSprite.h"
#include "gamedata.h"
#include "renderContext.h"


Player::Player( const std::string& name) :
  player(name),
  initialVelocity(player.getVelocity()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  observers()
{ }

void Player::stop() {
  player.setVelocity( Vector2f(0, 0) );
}

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
  player.update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    (*ptr)->setPlayerPos( player.getPosition() );
    ++ptr;
  }
  stop();
}
