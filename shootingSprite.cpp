#include "shootingSprite.h"
#include "gamedata.h"

ShootingSprite::ShootingSprite(const std::string& name) :
  Sprite(name),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  startPos(Vector2f(Gamedata::getInstance().getXmlInt(name+"/bulletOff/x"),
           Gamedata::getInstance().getXmlInt(name+"/bulletOff/y"))),
  active(),
  inactive(),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0)
{ }

ShootingSprite::ShootingSprite(const std::string& name, Vector2f pos) :
  Sprite(name),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  startPos(pos),
  active(),
  inactive(),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0)
{ }

ShootingSprite::ShootingSprite(const ShootingSprite& s) :
  Sprite(s),
  bulletName(s.bulletName),
  active(s.active),
  inactive(s.inactive),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastFrame(s.timeSinceLastFrame)
{ }

void ShootingSprite::shoot() {
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()/2;
  // I need to add some minSpeed to velocity:
  if (inactive.empty()) {
    Bullet *bull = new Bullet(bulletName);
    bull->setPosition( getPosition() + startPos +Vector2f(deltaX, deltaY) );
    bull->setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
    active.push_back(bull);
  }
  else {
    Bullet *bull = inactive.front();
    inactive.pop_front();
    bull->reset();
    bull->setPosition( getPosition() + startPos +Vector2f(deltaX, deltaY) );
    bull->setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
    active.push_back(bull);
  }
  timeSinceLastFrame = 0;
}

void ShootingSprite::draw() {
  Sprite::draw();
  std::list<Bullet*>::iterator bullItr = active.begin();
  while (bullItr != active.end()) {
    if (!(*bullItr)->goneTooFar()) {
      (*bullItr)->draw();
    }
    else {
      inactive.push_back(*bullItr);
      bullItr = active.erase(bullItr);
    }
    ++bullItr;
  }
}

void ShootingSprite::update(Uint32 ticks) {
	timeSinceLastFrame += ticks;
  Sprite::update(ticks);
  shoot();
  for ( Bullet* bullet : active ) {
    bullet->update(ticks);
  }
}
