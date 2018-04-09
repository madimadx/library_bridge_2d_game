#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"

class Player;
class CollisionStrategy;
class SmartSprite;
class Hud;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World bricks;
  World world;
  World clouds;
  Viewport& viewport;

  std::vector<Drawable*> dummies;
  std::vector<SmartSprite*> smarties;
  Player* player;
  std::vector<CollisionStrategy*> strategies;
  Hud* theHud;
  int currentStrategy;
  bool collision;
  bool hudOn;
  int popUpDelay;
  int delayCount;

  bool makeVideo;

  void resetDelay();
  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
