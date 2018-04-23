#ifndef PLAYER__H
#define PLAYER__H
#include <SDL.h>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include "multisprite.h"

class SmartSprite;

class Player {
public:
  Player(const std::string&);
  Player(const Player&);

  void draw() const { if (!isDeathOn()) player.draw(); else deathDraw();}
  void update(Uint32 ticks);
  const MultiSprite* getPlayer() const { return &player; }

  const std::string& getName() const { return player.getName(); }
  int getX() const { return player.getX(); }
  int getY() const { return player.getY(); }
  const Image* getImage() const {
    return player.getImage();
  }
  int getScaledWidth()  const {
    return player.getScaledWidth();
  }
  int getScaledHeight()  const {
    return player.getScaledHeight();
  }
  const SDL_Surface* getSurface() const {
    return player.getSurface();
  }

  void attach( SmartSprite* o ) { observers.push_back(o); }
  void detach( SmartSprite* o );

  void right();
  void left();
  void up();
  void down();
  void stop();
  void deathOn() {death = true;}
  void deathDraw() const;
  bool deathReset();
  bool isDeathOn() const {return death;}

  Player& operator=(const Player&) = delete;

private:
  MultiSprite player;
  Vector2f initialVelocity;
  int worldWidth;
  int worldHeight;
  int elapsedFromDeath;
  int deathDelay;
  bool death;

protected:
  std::list<SmartSprite*> observers;
};
#endif
