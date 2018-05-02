#include <string>
#include <iostream>
#include <list>
#include "sprite.h"
#include "bullet.h"

class ShootingSprite : public Sprite {
public:
  ShootingSprite(const std::string& n);
  ShootingSprite(const std::string& n, Vector2f pos);
  ShootingSprite(const ShootingSprite& s);

  virtual void update(Uint32 ticks);
  virtual void draw();
  void shoot();
  int getNumActive() {return active.size();}
  int getNumInactive() {return inactive.size();}
  std::list<Bullet*>& getBulletListActive() {return active;}
  std::list<Bullet*>& getBulletListInactive() {return inactive;}

private:
  std::string bulletName;
  Vector2f startPos;
  std::list<Bullet*> active;
  std::list<Bullet*> inactive;
  float minSpeed;
  float bulletInterval;
  float timeSinceLastFrame;
  ShootingSprite& operator=(const ShootingSprite&);
};
