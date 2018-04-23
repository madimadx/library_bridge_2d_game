#include <iostream>
#include <cmath>
#include "bullet.h"

void Bullet::update(Uint32 ticks) {
  Vector2f pos = getPosition();
  MultiSprite::update(ticks);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}
